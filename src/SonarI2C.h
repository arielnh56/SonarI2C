/*
  SonarI2C.cpp
  Copyright (c) 2016,2017 Alastair Young.
  This project is licensed under the terms of the MIT license.
*/
#ifndef SONARI2C_H
#define SONARI2C_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define SONAR_INT_PIN 2
#define SONAR_SPACING 50 // millis to avoid echo

class SonarI2C {
  public:

    SonarI2C(uint8_t address, uint8_t pin, uint16_t max_mm); // constructor

    void init();     // member initializer. Call for each member in setup()
    int16_t mm();    // return distance in mm
    int16_t cm();    // return distance in cm
    int16_t inch();  // return distance in inches
    int16_t us();    // return echo time in microseconds
    boolean enabled();  // query if enabled
    void enable(boolean enabled);   // set _enable
    static boolean inverse;                                  // is logic inverted from sensor?
    static void begin();                                     // call from setup(). Defaults to pin 2 and 50ms
    static void begin(uint8_t interrupt);                    // same thing but set the pin
    static void begin(uint8_t interrupt, uint16_t spacing);  // same thing but set the pin and spacing
    static void doSonar();                                   // call every loop()
    static uint8_t maxOOR;                                   // how many OOR to skip. Raise this in noisy environments

  private:
    uint8_t  _address;                   // PCF8574 0x20 - 0x27 or PCF8574A 0x38 - 0x3F
    uint8_t  _pin;                       // expander pin 0 -7
    uint16_t _max_micros;                // limit in microseconds. Above this return zero.
    uint16_t _micros;                    // last good response time in microseconds
    SonarI2C *_nextSonar;                // link to the next SonarI2C in the list
    boolean  _enabled;                   // skip if enabled = FALSE
    void     _send_ping();               // trigger the sensor and set the interrupt
    uint8_t  _OORcount;                  // count how many out of range pings in a row
    static uint32_t _pulseBegin;         // to remember when the pulse started
    static uint16_t _spacing;            // time between pings - default 50ms
    static uint8_t  _interrupt;          // interrupt pin - default 2
    static uint32_t _last_sonar_millis;  // timestamp of last ping sent
    static void     _startPulse();       // interrupt callout when a pulse starts
    static void     _endPulse();         // interrupt callout when a pulse ends
    static SonarI2C *_currentSonar;      // pointer to the active sensor
};

#endif
