#include <SD.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define redLED 3
#define greenLED 4
#define chipSelect 8  //Sparkfun
//#define chipSelect 10 //Adafruit

//variables for storing gps data
float lat, lon, alt;
int sats, year;
byte month, day, hour, minute, second, hundredth;
unsigned long fixAge;

File datalog;                     //File object for datalogging
char filename[] = "GPSLOG00.csv"; //Template for file name to save data
bool SDactive = false;            //used to check for SD card before attempting to log

TinyGPS gps;              //gps object
SoftwareSerial ss(5, 6);  //extra serial line to get data from gps
unsigned long timer = 0;  //used to keep track of datalog cycles
bool newData = false;     //becomes true when gps object processes new data

void setup() {
  Serial.begin(115200);   //start serial communication
  ss.begin(9600);         //start gps communication
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(10, OUTPUT);

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
      filename[6] = char(i/10);
      filename[7] = char(i%10);
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
  bool newData = false;
  while(ss.available() > 0) {   //if there's data on the gps serial line, read and process it
    if(gps.encode(ss.read())) {
      newData = true;
    }
  }
  if (newData) {                //if new data is processed, save it to program variables
    gps.f_get_position(&lat, &lon, &fixAge);
    alt = gps.f_altitude();
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredth, &fixAge);
    sats = gps.satellites();
  }
  if (millis() - timer > 1000) {  //log data once per second. This is a more accurate method than delay(), as it includes
    timer = millis();             //time taken for the program to execute, but requires a bit of setup
    String data = String(month) + "/" + String(day) + "/" + String(year) + ",";
    data += String(hour) + ":" + String(minute) + ":" + String(second) + ",";
    data += String(lat, 4) + "," + String(lon, 4) + "," + String(alt, 1) + "," + String(sats) + ",";
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
