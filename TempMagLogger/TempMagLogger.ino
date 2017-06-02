/*   Code to read a 3-axis digital magnetometer and analog temp sensor, and record both to an SD card.
 *   Author: Ryan Bowers
 *   For use with Introduction to Arduino Activity 7
 */

#include <SD.h>
#include <SparkFun_MAG3110.h>

#define chipSelect 8  //Sparkfun micro-SD shield
//#define chipSelect 10 //Adafruit SD shield
#define ledPin 5
#define tempPin A2

File datalog;                     //File object for datalogging
char filename[] = "LOGGER00.csv"; //Template for file name to save data
bool SDactive = false;            //used to check for SD card before attempting to log

MAG3110 mag = MAG3110();  //create magnetometer object
int x, y, z;              //variables to store magnetometer data

void setup() {
  Serial.begin(115200);     //start Serial communication
  pinMode(ledPin, OUTPUT);  //setup LED pin
  pinMode(10, OUTPUT);      //Needed for SD library, regardless of shield used
  
  Serial.print("Initializing SD card...");
  if(!SD.begin(chipSelect))                                 //attempt to start SD communication
    Serial.println("Card failed, or not present");          //print out error if failed; remind user to check card
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
    if (!SDactive) Serial.println("No available file names; clear SD card to enable logging");
  }
  
  mag.initialize(); //initializes mag sensor settings
  mag.start();      //activate the sensor

  String header = "Mag_x,Mag_y,Mag_z,Temp (C),";  //setup data format, and print it to the monitor and SD card
  Serial.println(header);
  if (SDactive) {
    datalog.println(header);
    datalog.close();
  }
}

void loop() {
  if(mag.dataReady()) {               //check to see if new data is available (always should be)
    mag.readMag(&x, &y, &z);          //save mag data to variables
    String data = String(x) + "," + String(y) + "," + String(z) + ",";
    data += String(getTemp()) + ",";  //build a string to hold mag and temp data
    digitalWrite(ledPin, HIGH);       //flash LED to show data being logged
    Serial.println(data);             //print data to monitor...
    if (SDactive) {
      datalog = SD.open(filename, FILE_WRITE);
      datalog.println(data);          //...and to SD card if available
      datalog.close();                //close file afterward to ensure data is saved properly
    }
    delay(250);                       //have LED blink for quarter-second
    digitalWrite(ledPin, LOW);        //then turn it off
    delay(750);                       //complete a full second cycle before logging again
  }
}

//function to read the temp sensor and return the current temp in C
float getTemp() {
  return (analogRead(tempPin)*(5.0/1024)-.5)/.01;
}

