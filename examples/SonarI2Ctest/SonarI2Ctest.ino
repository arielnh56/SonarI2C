

/*
 *  Test script for SonaI2C
 *  
 *  This uses the LiquidCrystal_PCF8574 lubrary to drive an 2004 LCD display
 *  mm results are displayed anti-clockwise around the display to match the OctoSonar board layout
 *  Values are updated every 500ms
 *  
 */
// #define I2C_MASTER_ADDRESS 9   // for author's dual brain test bot
#define I2C_LCD_ADDRESS 0x27      // default for the current flavor of cheap I2C backpack
#define I2C_SONAR_ADDRESS 0x20    // change to 0x3B for default OctoSonar
#include "SonarI2C.h"
// #include <LiquidCrystal_I2C.h>   // old NewLiquidCrystal library
#include <LiquidCrystal_PCF8574.h>

// LiquidCrystal_I2C lcd(I2C_LCD_ADDRESS, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // // old NewLiquidCrystal library
LiquidCrystal_PCF8574 lcd(I2C_LCD_ADDRESS); 

uint8_t numsonars = 8;

// Numeric Sequence
SonarI2C sonars[] = {
  SonarI2C (I2C_SONAR_ADDRESS, 0, 4000),
  SonarI2C (I2C_SONAR_ADDRESS, 1, 4000),
  SonarI2C (I2C_SONAR_ADDRESS, 2, 4000),
  SonarI2C (I2C_SONAR_ADDRESS, 3, 4000),
  SonarI2C (I2C_SONAR_ADDRESS, 4, 4000),
  SonarI2C (I2C_SONAR_ADDRESS, 5, 4000),
  SonarI2C (I2C_SONAR_ADDRESS, 6, 4000),
  SonarI2C (I2C_SONAR_ADDRESS, 7, 4000),
};




void setup() {
  lcd.begin(20, 4);              // initialize the lcd
  lcd.setBacklight(255);         // turn on the backlight
  lcd.clear();                   // go home
  Serial.begin(115200);
  SonarI2C::begin();   // initialize bus, pins etc
  // SonarI2C::begin(2);   // alternate form, set interrupt pin, 2 is the default
  // SonarI2C::begin(2, 200);   // alternate form also sets spacing, 50ms is the default
  SonarI2C::inverse = false;
  // SonarI2C::maxOOR = 5;
  for (uint8_t i = 0; i < numsonars; i++) {
    sonars[i].init();
  }
}

uint32_t last_print = 0;
void loop() {
  char buffer[20];
  SonarI2C::doSonar();  // call every cycle, SonarI2C handles the spacing

  if (last_print + 500 < millis()) {
    last_print = millis();
    for (uint8_t i = 0; i < numsonars; i++) {
      //      Serial.print(sonars[i].mm()); Serial.print("   ");
    }
    //    Serial.println();
    sprintf(buffer, "0 %5d      %5d 7 ", sonars[0].mm(), sonars[7].mm());
    lcd.setCursor(0, 0); lcd.print(buffer);
    sprintf(buffer, "1 %5d      %5d 6", sonars[1].mm(), sonars[6].mm());
    lcd.setCursor(0, 1); lcd.print(buffer);
    sprintf(buffer, "2 %5d      %5d 5", sonars[2].mm(), sonars[5].mm());
    lcd.setCursor(0, 2); lcd.print(buffer);
    sprintf(buffer, "3 %5d      %5d 4", sonars[3].mm(), sonars[4].mm());
    lcd.setCursor(0, 3); lcd.print(buffer);
  }
}



