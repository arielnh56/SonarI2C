
#include "SonarI2C.h"

uint8_t numsonars = 10;
SonarI2C sonars[] = {
  SonarI2C (0x20, 0, 4000),
  SonarI2C (0x20, 1, 4000),
  SonarI2C (0x20, 2, 4000),
  SonarI2C (0x20, 3, 4000),
  SonarI2C (0x20, 4, 4000),
  SonarI2C (0x20, 5, 4000),
  SonarI2C (0x20, 6, 4000),
  SonarI2C (0x20, 7, 4000),
  SonarI2C (0x21, 0, 4000),
  SonarI2C (0x21, 1, 4000),
};

void setup() {
  Serial.begin(115200);
  SonarI2C::begin();   // initialize bus, pins etc
  // SonarI2C::begin(2);   // alternate form, set interrupt pin, 2 is the default
  // SonarI2C::begin(2, 50);   // alternate form also sets spacing, 50ms is the default
  SonarI2C::inverse = false;
  for (uint8_t i = 0; i < numsonars; i++) {
    sonars[i].init();
  }
}

uint32_t last_print = 0;
void loop() {
  SonarI2C::doSonar();  // call every cycle, SonarI2C handles the spacing

  if (last_print + 100 < millis()) {
    last_print = millis();
    for (uint8_t i = 0; i < numsonars; i++) {
      Serial.print(sonars[i].mm()); Serial.print("   ");
    }
    Serial.println();
  }
}



