#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire1(ONE_WIRE_BUS);
DallasTemperature temperature(&oneWire1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  temperature.begin();
}

void loop() {
  temperature.requestTemperatures();
  double temp = temperature.getTempCByIndex(0);
  Serial.println("Temp (C) = " + String(temp));
  delay(1000);
}
