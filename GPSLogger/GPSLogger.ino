#include <SD.h>
#include <SoftwareSerial.h>
#include <FlightGPS.h>

#define redLED 3
#define greenLED 4
#define chipSelect 8  //Sparkfun
//#define chipSelect 10 //Adafruit

File datalog;                     //File object for datalogging
char filename[] = "GPSLOG00.csv"; //Template for file name to save data
bool SDactive = false;            //used to check for SD card before attempting to log

SoftwareSerial ss(5, 6);        //extra serial line to get data from gps
FlightGPS gps = FlightGPS(&ss); //GPS object - connect to serial line
unsigned long timer = 0;        //used to keep track of datalog cycles

void setup() {
  Serial.begin(115200);   //start serial communication
  gps.initialize();       //start gps communication
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(10, OUTPUT);    //This always needs to be an output when using SD

  Serial.print("Initializing SD card...");
  if(!SD.begin(chipSelect)) {                               //attempt to start SD communication
    Serial.println("Card failed, or not present");          //print out error if failed; remind user to check card
    for (byte i = 0; i < 10; i++) {                         //also flash error LED rapidly for 2 seconds, then leave it on
      digitalWrite(redLED, HIGH);
      delay(100);
      digitalWrite(redLED, LOW);
      delay(100);
    }
    digitalWrite(redLED, HIGH);
  }
  else {                                                    //if successful, attempt to create file
    Serial.println("Card initialized.\nCreating File...");
    for (byte i = 0; i < 100; i++) {                        //can create up to 100 files with similar names, but numbered differently
      filename[6] = '0' + i/10;
      filename[7] = '0' + i%10;
      if (!SD.exists(filename)) {                           //if a given filename doesn't exist, it's available
        datalog = SD.open(filename, FILE_WRITE);            //create file with that name
        SDactive = true;                                    //activate SD logging since file creation was successful
        Serial.println("Logging to: " + String(filename));  //Tell user which file contains the data for this run of the program
        break;                                              //Exit the for loop now that we have a file
      }
    }
    if (!SDactive) {
      Serial.println("No available file names; clear SD card to enable logging");
      for (byte i = 0; i < 4; i++) {                        //flash LED more slowly if error is too many files (unlikely to happen)
        digitalWrite(redLED, HIGH);
        delay(250);
        digitalWrite(redLED, LOW);
        delay(250);
      }
      digitalWrite(redLED, HIGH);
    }
  }

  String header = "GPS Date,GPS time,Lat,Lon,Alt (m),# Sats,";  //setup data format, and print it to monitor and SD card
  Serial.println(header);
  if (SDactive) {
    datalog.println(header);
    datalog.close();
  }
}

void loop() {
  gps.update();   //check for incoming gps strings and parse them for data
  
  if (millis() - timer > 1000) {  //log data once per second. This is a more accurate method than delay(), as it includes
    timer = millis();             //time taken for the program to execute, but requires a bit of setup
    
    //All data is returned as numbers (int or float as appropriate), so values must be converted to strings before logging
    String data = String(gps.getMonth()) + "/" + String(gps.getDay()) + "/" + String(gps.getYear()) + ","
                  + String(gps.getHour()) + ":" + String(gps.getMinute()) + ":" + String(gps.getSecond()) + ","
                  + String(gps.getLat(), 4) + "," + String(gps.getLon(), 4) + "," + String(gps.getAlt(), 1) + ","
                  + String(gps.getSats());
    digitalWrite(greenLED, HIGH); //flasch green LED briefly when writing data
    Serial.println(data);
    if (SDactive) {
      datalog = SD.open(filename, FILE_WRITE);
      datalog.println(data);
      datalog.close();
    }
    digitalWrite(greenLED, LOW);
  }
}
