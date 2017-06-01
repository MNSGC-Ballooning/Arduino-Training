/*  Code to print pressure and tempereature measurements from the Sparkfun MPL3115A2 digital sensor.
 *  Author: Ryan Bowers
 *  For use with Intro to Arduino Training Activity 6
 */

#include <Wire.h>
#include "SparkFunMPL3115A2.h"

MPL3115A2 sensor;       //create pressure sensor object
float pressure, temp;   //variables for measured data

void setup() {
  Wire.begin();         //Begin I2C communication
  Serial.begin(115200); //Open Serial communication
  sensor.begin();       //Connect sensor to I2C bus

  sensor.setModeBarometer();    //Read pressure directly in Pa
  sensor.setOversampleRate(7);  //Recommended setting
  sensor.enableEventFlags();    //Recommended setting
}

void loop() {
  pressure = sensor.readPressure();
  temp = sensor.readTempF();

  Serial.println("Pressure (Pa): " + String(pressure) + "  Temp (F): " + String(temp)); //print data to serial monitor
  delay(1000);  //take measurements approx once per second
}
