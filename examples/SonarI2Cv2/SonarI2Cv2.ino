/*
 * This demo goes with the fritzing breadboard example
 * two sensors on pins 4 and 5 with the outputs multiplexed back to pin 2
 * via transistor NOR gate
 */


#include "SonarI2C.h"

SonarI2C frontSonar(0x20, 4, 4000);
SonarI2C backSonar(0x20, 5, 4000);

void setup() {
  Serial.begin(115200);
  SonarI2C::begin();   // initialize bus, pins etc
  SonarI2C::inverse = true;  // NOR logic
  // SonarI2C::begin(2);   // alternate form, set interrupt pin, 2 is the default
  // SonarI2C::begin(2, 50);   // alternate form also sets spacing, 50ms is the default
  
  frontSonar.init();   // clear expander and add to list
  backSonar.init();    // clear expander and add to list
}

uint32_t last_print = 0;
void loop() {
  SonarI2C::doSonar();  // call every cycle, SonarI2C handles the spacing

  if (last_print + 100 < millis()) {
    last_print=millis();
    Serial.print(frontSonar.mm()); Serial.print("          ");
    Serial.println(backSonar.mm());
  }
}



