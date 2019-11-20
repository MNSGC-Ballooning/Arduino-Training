//tells the program that it needs to have the following two libraries
#include <OneWire.h> 
#include <DallasTemperature.h>

// places tempature sensor to be on pin twp
#define ONE_WIRE_BUS 2

//sets up communication with that pin via one wire library
OneWire oneWire1(ONE_WIRE_BUS);

//sets up connection between one wire and the Dalllas tempature sensor
DallasTemperature temperature(&oneWire1);

void setup() {
  //starts the arduino talking to the serial monitor
  Serial.begin(9600);
  
  //Initiates the tempature sensor 
  temperature.begin();
}

void loop() {
  //gets the current tempature from the sensot
  temperature.requestTemperatures();

  //saves this value as a variable
  double temp = temperature.getTempCByIndex(0);

  //print the tempature it just revived to the serial monitor
  Serial.println("Temp (C) = " + String(temp));
  delay(1000);
}
