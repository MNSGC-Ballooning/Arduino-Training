#include <SD.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <TinyGPS++.h>


//Place for definitions//
//#define redLED 3
//#define greenLED 4
#define RX 7
#define TX 6
#define chipSelect 10


TinyGPSPlus GPS;
SoftwareSerial gps_serial(7,6);


//File datalog;                     //File object for datalogging
//char filename[] = "COPGPS00.csv"; //Template for file name to save data 
//bool SDactive = false;            //used to check for SD card before attempting to log


//functions to manage the GPS in updateGPS().
unsigned long lastGPS = 0;
unsigned long GPSstartTime = 0;    //when the GPS starts, time in seconds of last GPS update.
uint8_t days = 0;                  //if we're flying overnight
unsigned long timer = 0;           //timer to ensure GPS prints data once every second
unsigned long fixtimer = 0;        //timer to ensure that if the GPS has a fix, and LED flashes once every 10 seconds
unsigned long LEDlocktimer = 0;    //timer to ensure that the green LED that goes on if there is a GPS lock stays on for a specified amount of time
bool fixLED = false;               //bool that communicates if the green LED is on or not

void updateGPS() {                    //Function that updates GPS every second and accounts for
  static bool firstFix = false;       //clock rollover at midnight (23:59:59 to 00:00:00)
  while (gps_serial.available() > 0) {
    GPS.encode(gps_serial.read());
    }
  if (GPS.altitude.isUpdated() || GPS.location.isUpdated()) {
    if (!firstFix && GPS.Fix) {     //gps.fix
      GPSstartTime = GPS.time.hour() * 3600 + GPS.time.minute() * 60 + GPS.time.second();
      firstFix = true;
      }
    }
  if (getGPStime() > lastGPS) {    //if it's been more than a second
        lastGPS = GPS.time.hour() * 3600 + GPS.time.minute() * 60 + GPS.time.second();
    }
}
   
int getGPStime() {
  return (GPS.time.hour() * 3600 + GPS.time.minute() * 60 + GPS.time.second());
}

int getLastGPS() { 
  //returns time in seconds between last successful fix and initial fix. Used to match with altitude data
  static bool newDay  = false;           //variable in case we're flying late at night (clock rollover)
  if (!newDay && lastGPS < GPSstartTime) {
    days++;
    newDay = true;
  }
  else if (newDay && lastGPS > GPSstartTime)
    newDay = false;
  return days * 86400 + lastGPS;
}


void SerialprintGPS() { //function that returns the GPS data to the serial port.
                        //can serve as another check to be sure GPS has a lock before powering it with a separate battery
  String data="";
  
  String datestring = "01/01/2000";
  String timestamp = "00:00:00";
  String coordinates = "00.0000, 00.0000, 0.00";
  
  data += datestring + ", " + timestamp + ",  " + coordinates + "  "; //string that is printed in case no data is received
  
 // if (GPS.Fix && GPS.altitude.feet() != 0) {
   /*Serial.print(GPS.date.month());  //prints the date
   Serial.print("/");
   Serial.print(GPS.date.day());
   Serial.print("/");
   Serial.print(GPS.date.year());
   Serial.print(", ");
   Serial.println(GPS.date.value());
   Serial.print(","); */
   

   Serial.print("HR:");
   Serial.print(GPS.time.hour()-5);   //prints the time
   Serial.print(" MIN:");
   Serial.print(GPS.time.minute());
   Serial.print(" S: ");
   Serial.print(GPS.time.second());
   
    Serial.print(", ");
    Serial.print(" Lat: ");
   Serial.print(GPS.location.lat(), 6); //prints the latitude
   Serial.print(" Degrees, ");

   Serial.print(" Long: ");
   Serial.print(GPS.location.lng(), 6); //prints the longitude
   Serial.print(" Degrees, ");

    Serial.print(" Altitude: ");
   Serial.print(GPS.altitude.feet());   //prints the altitude
   Serial.print(" FT,"); 

   Serial.print(" num of satellites,"); 
   Serial.println(GPS.satellites.value()); // Number of satellites in use (u32)
   

   //Serial.print(get.value()
   Serial.print("\n");  
   // }

  //else {
  //  Serial.println(data); //if there is no GPS lock (GPS.Fix), then the data string is printed
  //  }
}


/*void SDprintGPS() { //function that returns the GPS data to the SD card
  String data="";
  
  String datestring = "01/01/2000";
  String timestamp = "00:00:00";
  String coordinates = "00.0000, 00.0000, 0.00";
  
  data += datestring + ", " + timestamp + ",  " + coordinates + "  "; //string that is printed in case no data is received

  if (GPS.Fix && GPS.altitude.feet() != 0) {
  // datalog.print(GPS.date.month());   //prints the date
   datalog.print("/");
   datalog.print(GPS.date.day());
   datalog.print("/");
   datalog.print(GPS.date.year());
   datalog.print(", ");

   datalog.print(GPS.time.hour());    //prints the time
   datalog.print(":");
   datalog.print(GPS.time.minute());
   datalog.print(":");
   datalog.print(GPS.time.second());
   datalog.print(", ");
    
   datalog.print(GPS.location.lat(), 6); //prints the latitude
   datalog.print(", ");

   datalog.print(GPS.location.lng(), 6); //prints the longitude
   datalog.print(", ");
    
   datalog.print(GPS.altitude.feet());   //print the altitude
   datalog.print("\n");   
      }


   else {
    datalog.println(data); //if there is no GPS lock (GPS.Fix), then the data string is printed
      }
}*/



void setup() {
  Serial.begin(9600);
 // pinMode(redLED, OUTPUT);
 // pinMode(greenLED, OUTPUT);
 // pinMode(10, OUTPUT);    //This always needs to be an output when using SD

  gps_serial.begin(4800);

 /* Serial.print("Initializing SD card...");
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

  String header = "GPS Date, GPS time, Latitude, Longitude, Altitude(ft)";  //setup data format, and print it to monitor and SD card
  Serial.println(header);
  if (SDactive) {
    datalog.println(header);
    datalog.close();
  }*/
}


void loop() {
   if (millis() - fixtimer > 10000 && GPS.Fix) {
     fixtimer = millis(); //both fixtimer and LEDlocktimer set every 10 seconds
    // LEDlocktimer = millis();
    // digitalWrite(greenLED, HIGH);
    // fixLED = true; 
     }
  
   updateGPS(); //time-keeping stuff for the GPS

   if (millis() - timer > 1000) {
     timer = millis();  //every second data is logged to the serial monitor and SD card

     SerialprintGPS(); //data printed to serial
    
    /* if (SDactive) {
       digitalWrite(redLED, HIGH); //if SD is active, then the red LED flases everytime data is logged
       datalog = SD.open(filename, FILE_WRITE);
       SDprintGPS(); //data printed to SD
       datalog.close();
       digitalWrite(redLED, LOW);
       }*/
     }

   /*if (fixLED && (millis() - LEDlocktimer > 1000)) { //when the GPS has a fix, this ensures that the
    digitalWrite(greenLED, LOW);                     //green LED is on for a full second every 10 seconds
    fixLED = false;  
   }*/
}
