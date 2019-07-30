#include <UbloxGPS.h>
#include <SoftwareSerial.h>

SoftwareSerial gpsSer(2,3); //Do not include if using teensy

UbloxGPS gps = UbloxGPS(&gpsSer); //Or &Serial1 if using teensy
boolean fixU = false;

String data;

void setup() {
  //Start serial
  Serial.begin(9600);
  //Start ublox
  gpsSer.begin(UBLOX_BAUD); //Serial1.begin(UBLOX.BAUD);
  
  while(!Serial){
    ; //Wait for serial port to connect
  }
  
  //Start GPS 
  gps.init();
  //Attempt to set to airborne 3 times. If successful, records result and breaks loop. If unsuccessful, saves warning and moves on
  byte i = 0;
  while (i<50) {
    i++;
    if (gps.setAirborne()) {
      Serial.println("Air mode successfully set.");
      break;
    }
    else if (i ==50)
      Serial.println("WARNING: Failed to set to air mode (3 attemtps). Altitude data may be unreliable.");
    else
      Serial.println("Error: Air mode set unsuccessful. Reattempting...");
  }
  Serial.println("GPS configured");
}
void getUbloxData(){
  gps.update();

  //log data once every second
  if(millis()%1000 == 0) {
    //All data is returned as numbers (int or float as appropriate), so values must be converted to strings before logging
    String data = String(gps.getMonth()) + "/" + String(gps.getDay()) + "/" + String(gps.getYear()) + ","
                  + String(gps.getHour()-5) + ":" + String(gps.getMinute()) + ":" + String(gps.getSecond()) + ","
                  + String(gps.getLat(), 4) + "," + String(gps.getLon(), 4) + "," + String(gps.getAlt_meters(), 1) + ","
                  + String(gps.getSats()) + ",";
    //GPS should update once per second, if data is more than 2 seconds old, fix was likely lost
    if(gps.getFixAge() > 2000)
      data += "No Fix,";
    else
      data += "Fix,";
    Serial.println(data);
  }
}

void loop() {
  getUbloxData();
  //Serial.write(Serial1.read());
}
