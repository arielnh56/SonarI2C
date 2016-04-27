# SonarI2C library for Arduino

Version: 1.0.0<br>
Release date: 2016 April 28<br>
[redhunter.com](http://redhunter.com/blog/category/technology/arduino/)


## Summary

This is a library for the Arduino IDE that allows the polling of multiple ultrasonic distance sensors using the I2C bus and a single hardware interrupt pin. It assumes a PCF8574(A) type port expander to trigger the sensors, and external logic circuitry to multiplex the echo signals.

It has been tested with the HC-SR04 sensor - the cheapest and most widely available at this time. Up to 10 units have been tested on a breadboard, using two PCF8574 for triggers and two CD4078BE 8-inpout OR/NOR chips.

## Supported Platforms

This library is designed to work with the Arduino IDE versions 1.6.x or later; it is not tested it with earlier versions. It should work with any Arduino compatible board with an available hardware interrupt pin. Note that other interrupt activity may interfere with the accuracy and reliability of this code.

## Getting Started

### Hardware

The minimum setup to use this usefully requires

* a PCF8574 or PCF8574A IC
* two HC-SR04 sensors
* two NPN transistors and some resistors to make a NOR gate
* example sketch SonarI2Cv2

![basic breadboard test](https://github.com/arielnh56/SonarI2C/blob/master/extras/sonarI2Cdemo1_bb.png) 

### Software

If you are using version 1.6.2 or later of the [Arduino software (IDE)](http://www.arduino.cc/en/Main/Software), you can use the Library Manager to install this library:

1. In the Arduino IDE, open the "Sketch" menu, select "Include Library", then "Manage Libraries...".
2. Search for "SonarI2C".
3. Click the SonarI2C entry in the list.
4. Click "Install".

If this does not work, you can manually install the library:

1. Download the [latest release archive from GitHub](https://github.com/arielnh56/SonarI2C/releases) and decompress it.
2. Rename the folder "SonarI2C-master" to "SonarI2C".
3. Move the "SonarI2C" folder into the "libraries" directory inside your Arduino sketchbook directory.  You can view your sketchbook location by opening the "File" menu and selecting "Preferences" in the Arduino IDE.  If there is not already a "libraries" folder in that location, you should make the folder yourself.
4. After installing the library, restart the Arduino IDE.

(note - the above instructions adapted from a Pololu readme)

## Examples

Two examples are included showing the use of two and ten sensors.

## Library reference

* ```SonarI2C(uint8_t address, uint8_t pin, uint16_t max_mm)```
   Constructor. Sets the I2C address of the pin expander (0x20 - 0x27, 0x38 - 0x3F), the pin on the expander (0-7) and the maximum range in mm (0-4000). 

* ```static void begin();                                     // call from setup(). Defaults to pin 2 and 50ms
   static void begin(uint8_t interrupt);                    // same thing but set the pin
   static void begin(uint8_t interrupt, uint16_t spacing);  // same thing but set the pin and spacing```
   call one of the begin() functions from setup() before you call init() on the individual SonarI2C members. This sets the interrupt pin to use and the spacing between sonar pings. The default is 50ms to ensure no interference from old echoes. Reduce this for faster but less reliable operation.

*  ```static boolean inverse```
   by default the interrupt pin is looking for a positive echo pulse. If you are using NOR logic (as in the breadboard example) set this to
   'true'
* ```void init()```
   call this on each SonarI2C from setup(). It initilizes some values and places it in the polling queue

* ```boolean enabled();  // query if enabled
   void enable(boolean enabled);   // set _enable```
   Disabling sensors that you are not interested in right now allows the other sensors to poll more often.

* ```static void doSonar()```
  Call this every loop(). If it is time, it will poll the next enabled sensor.

*
    ```
       int16_t mm();    // return distance in mm
       int16_t cm();    // return distance in cm
       int16_t inch();  // return distance in inches
       int16_t us();    // return echo time in microseconds
    ```
    currently uses floats, which is inefficient

## Version history

* 1.0.0 (2016 April 26): Original release.





