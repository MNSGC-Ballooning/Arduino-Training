/*   Code to read and log temp data from two Dallas digital temperature sensors
 *   Author: Ryan Bowers
 *   For use with Introduction to Arduino Activity 8
 */

#include <SD.h>
#include <DallasTemperature.h>

#define ledPin 4
#define oneWireBus 2

#define chipSelect 8  //Sparkfun
//#define chipSelect 10 //Adafruit

File datalog;                     //File object for datalogging
char filename[] = "SDTEMP00.csv"; //Template for file name to save data
bool SDactive = false;            //used to check for SD card before attempting to log

OneWire oneWire = OneWire(oneWireBus);                    //set up a oneWire bus
DallasTemperature sensors = DallasTemperature(&oneWire);  //create a DallasTemp object for both sensors using that instance
DeviceAddress inThermometer, outThermometer;              //Objects for each individual oneWire device
float inTemp, outTemp;                                    //variables for temperature data

void setup() {
  Serial.begin(115200);
  sensors.begin();

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
  
  if (!sensors.getAddress(inThermometer, 0)) {  //get address of first sensor; display error if not found
    Serial.println("Unable to find address for Device 0");
    if (SDactive) datalog.println("Unable to find address for Device 0");
  }
  if (!sensors.getAddress(outThermometer, 1)) { //repeat for second sensor
    Serial.println("Unable to find address for Device 1");
    if (SDactive) datalog.println("Unable to find address for Device 1");
  }

  sensors.setResolution(inThermometer, 9);  //set resolution for both sensors to 9 bits
  sensors.setResolution(outThermometer, 9);

  String header = "In Temp (C),Out Temp (C),";  //setup data format, and print it to monitor and SD card
  Serial.println(header);
  if (SDactive) {
    datalog.println(header);
    datalog.close();
  }
}

void loop() {
  sensors.requestTemperatures();                              //read most recent temp data for all sensors
  inTemp = sensors.getTempC(inThermometer);                   //get values from each sensor
  outTemp = sensors.getTempC(outThermometer);

  String data = String(inTemp) + "," + String(outTemp) + ","; //build string to hold data
  digitalWrite(ledPin, HIGH);                                 //flash LED to show data being logged
  Serial.println(data);                                       //print data to monitor...
  if (SDactive) {
    datalog = SD.open(filename, FILE_WRITE);
    datalog.println(data);                                    //and to SD card if available
    datalog.close();                                          //close file afterward to ensure data is saved properly
  }
  delay(250);                                                 //have LED blink for quarter-second
  digitalWrite(ledPin, LOW);                                  //then turn it off
  delay(750);                                                 //complete a full second cycle before logging again
}
