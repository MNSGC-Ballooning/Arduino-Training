#include <Wire.h>  // Include Wire if you're using I2C
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

//The library assumes a reset pin is necessary. The Qwiic OLED has RST hard-wired, so pick an arbitrarty IO pin that is not being used
#define PIN_RESET 9  
//The DC_JUMPER is the I2C Address Select jumper. Set to 1 if the jumper is open (Default), or set to 0 if it's closed.
#define DC_JUMPER 1 
#define dallasPin 7

MicroOLED oled(PIN_RESET, DC_JUMPER);    // I2C declaration

OneWire oneWireOne(dallasPin); //For Dallas sensor
DallasTemperature dallasOne(&oneWireOne); //For Dallas sensor
float dallasOneF;

void setup() {

  delay(100);
  Wire.begin();
  oled.begin();    // Initialize the OLED
  oled.clear(ALL); // Clear the display's internal memory
  oled.display();  // Display what's in the buffer (splashscreen)
  delay(1000);     // Delay 1000 ms
  oled.clear(PAGE); // Clear the buffer.

  randomSeed(analogRead(A0) + analogRead(A1));
}

void loop() {
  // put your main code here, to run repeatedly:

    dallasOne.requestTemperatures();
    dallasOneF = dallasOne.getTempFByIndex(0);
    
    oled.setFontType(1);
    oled.setCursor(0, 0);
    oled.println("Temp(F)");
    oled.display();
    delay(1000);
    oled.clear(PAGE);  
  
}
