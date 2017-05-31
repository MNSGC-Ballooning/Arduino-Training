/*  Code to print acceleration measurements from 3-axis accelerometer (Sparkfun ADXL345). Values are in g's.
 *  Author: Ryan Bowers
 *  For use with Intro to Arduino Training Activity 5
 */


#include <SparkFun_ADXL345.h>

ADXL345 adxl = ADXL345(10); //accelerometer object, on pin 10
int x, y, z;                //variables to store accels

void setup() {
  Serial.begin(115200);

  adxl.powerOn();           //Initializes accelerometer
  adxl.setRangeSetting(16); //Will detect accels up to 16g. Can be lessened to 8, 4, or 2 to get increased precision
  adxl.setSpiBit(0);        //Using 4 wire SPI setup
}

void loop() {
  adxl.readAccel(&x, &y, &z); //write data to variables already defined
  Serial.println("x: " + String(x) + ", y: " + String(y) + ", z: " + String(z));  //Print data to serial monitor
  delay(250); //record data approx. every quarter second
}
