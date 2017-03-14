/*
  SonarI2C.cpp
  Copyright (c) 2016,2017 Alastair Young.
  This project is licensed under the terms of the MIT license.
*/
#include "SonarI2C.h"
#include <Wire.h>

// constructor
SonarI2C::SonarI2C(uint8_t address, uint8_t pin, uint16_t max_mm) {
  _address = (address < 0x38) ? constrain(address, 0x20, 0x27) : constrain(address, 0x38, 0x3F);
  _pin = constrain(pin, 0, 7);
  _max_micros = constrain(max_mm, 20, 4000) / 0.170145; // constrain to spec sheet
  _enabled = true;
  _OORcount = 0;
}

// member initializer - call from setup()
void SonarI2C::init() {
  Wire.beginTransmission(_address);
  Wire.write(0); // clear the expander
  Wire.endTransmission();
  _micros = 0;
  // insert into circular linked list
  if (_currentSonar) {
    _nextSonar = _currentSonar->_nextSonar;
    _currentSonar->_nextSonar = this;
  } else {  // new list
    _currentSonar = this;
    _nextSonar = this;
  }
}

// private member functions
// send the trigger signal and attach the interrupt
void SonarI2C::_send_ping() {
  _pulseBegin = 0; // cleared until we see it
  attachInterrupt(digitalPinToInterrupt(_interrupt), _startPulse, inverse ? FALLING : RISING); // NOR gate - pulse is inverted
  Wire.beginTransmission(_address);
  Wire.write(1 << _pin); // set the pin
  Wire.endTransmission();
  // at 100kHZ this seems to be about 240us pulse. More than the spec but works fine.
  Wire.beginTransmission(_address);
  Wire.write(0); // clear all pins
  Wire.endTransmission();
}

void SonarI2C::_startPulse() {
  _pulseBegin = micros(); // pulse is starting now
  attachInterrupt(digitalPinToInterrupt(_interrupt), _endPulse, inverse ? RISING : FALLING); // now look for pulse end
}

void SonarI2C::_endPulse() {
  uint32_t now = micros();
  detachInterrupt(digitalPinToInterrupt(_interrupt)); // clean up after ourselves
  // ignore wacko values
  if (now < _pulseBegin) return; // we started in the future - micros rollover or the like
  uint16_t pulseLen = now - _pulseBegin; // calculate length of pulse
  if (pulseLen > _currentSonar->_max_micros) { // took too long 0 = out of range
    _currentSonar->_OORcount++;
    if (_currentSonar->_OORcount > maxOOR) {
      _currentSonar->_micros = 0;
    } else {
      return;  //skip this time
    }
  } else {
    _currentSonar->_OORcount = 0;
    _currentSonar->_micros = pulseLen;
  }
}

// public member functions
int16_t SonarI2C::us() {
  return _micros;
}

int16_t SonarI2C::mm() {
  return _micros * 0.170145;
}

int16_t SonarI2C::cm() {
  return _micros * 0.0170145;
}

int16_t SonarI2C::inch() {
  return _micros * 0.00669862;
}

void SonarI2C::enable(boolean enabled) {
  _enabled = enabled;
  if (!enabled) _micros = 0;
}

// declare private static class variables
SonarI2C *SonarI2C::_currentSonar = NULL;
uint32_t SonarI2C::_last_sonar_millis = 0;
uint32_t SonarI2C::_pulseBegin = 0;
uint8_t SonarI2C::_interrupt = SONAR_INT_PIN;
uint16_t SonarI2C::_spacing = SONAR_SPACING;
boolean SonarI2C::inverse = false;
uint8_t SonarI2C::maxOOR = 1;

// public static class functions
// call a begin() from setup()

void SonarI2C::begin(uint8_t interrupt, uint16_t spacing) {
  _interrupt = interrupt;
  _spacing = spacing;
  begin();
}

void SonarI2C::begin() {
  Wire.begin();
  pinMode(_interrupt, INPUT);
}

void SonarI2C::begin(uint8_t interrupt) {
  _interrupt = interrupt;
  begin();
}

// call from loop() every cycle
void SonarI2C::doSonar() {
  if (!_currentSonar) return; // no sonars initiated
  if (digitalRead(_interrupt) == inverse ? LOW : HIGH) return; // interrupt pin is active - skip
  if (_last_sonar_millis + _spacing < millis()) {
    // look for next enabled sonar
    SonarI2C *thisSonar = _currentSonar->_nextSonar;
    while (!thisSonar->_enabled) {
      if (thisSonar == _currentSonar) return; // no enables found - skip
      thisSonar = thisSonar->_nextSonar;
    }

    _last_sonar_millis = millis();
    _currentSonar = thisSonar;
    _currentSonar->_send_ping();
  }
}

