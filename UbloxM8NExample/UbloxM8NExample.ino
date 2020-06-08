#include <SD.h>
#include <SPI.h>
#include <UbloxGPS.h>
#include <XBee.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial gps_Serial(2,3); // Use with arduino uno
//#define gps_Serial Serial1 // Use with arduino mega or teensy
String data;
boolean fix = false;
long int timer = 0;

UbloxGPS gps(&gps_Serial); //= TinyGPSPlus(&gps_Serial);

void setup() {
  Serial.begin(9600);
  gps_Serial.begin(UBLOX_BAUD);
  gps.init();
  Serial.println("gps initialized");
  delay(50);
  gps.setAirborne();
  byte i=0;
  while(i<20){
    i++;
    if(gps.setAirborne()){Serial.println("Airborne Mode Set");}
    break;
  }

  Serial.println("time \t Latitude \t Longitude \t alt \t sats \t ms \t Fix?");
  timer = millis();
}

void loop() {
    while(millis()<1000){;}
    gps.update();
    if(millis()-1000>timer){
    timer = millis();
    
    data =       String(gps.getHour()-5) + ":" + String(gps.getMinute()) + ":" + String(gps.getSecond()) + "\t "
                + String(gps.getLat(), 4) + "\t " + String(gps.getLon(), 4) + "\t " + String(gps.getAlt_meters()) + "\t "
                + String(gps.getSats()) + "\t " + String(timer);
    
    if(gps.getFixAge() > 2000){
      data += "\t No Fix";
      fix = true;
      }
    else
    {
      data += "\t Fix";
      fix = true;
    }
    Serial.println(data);
    }
}
