

//tell the program what librarys need to be included
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SD.h>

#define chipSelect 8  //sets pins for SD card
#define ledPin 5 //sets pin for LED
#define tempPin A2 //sets pin for analog tempature sensor
#define ONE_WIRE_BUS 2 //sets pin for digital tempature sensor

//initiates digital tempature sensor
OneWire oneWire1(ONE_WIRE_BUS);
DallasTemperature temperature(&oneWire1);

File datalog;                     //File object for datalogging
char filename[] = "LOGGER00.csv"; //Template for file name to save data
bool SDactive = false;            //used to check for SD card before attempting to log



void setup() {
  Serial.begin(115200);     //start Serial communication
  temperature.begin(); //more setup for the digital tempature sensor
  pinMode(ledPin, OUTPUT);  //setup LED pin
  pinMode(10, OUTPUT);      //Needed for SD library
  
  Serial.print("Initializing SD card...");
  if(!SD.begin(chipSelect))                                 //attempt to start SD communication
    Serial.println("Card failed, or not present");          //print out error if failed; remind user to check card
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
    if (!SDactive) Serial.println("No available file names; clear SD card to enable logging");
  }
  

  String header = "Analog Temp (C),Digital Temp (C),";  //setup data format, and print it to the monitor and SD card
  Serial.println(header);
  if (SDactive) {
    datalog.println(header);
    datalog.close();
  }
}

void loop() {
  String Atemp = String(getTemp());  //gets data from analog temp sensor

  temperature.requestTemperatures(); //reads digital temp sensor
  double Dtemp = temperature.getTempCByIndex(0); //saves temp as variable

  String data = String(Atemp) + "," + String(Dtemp) + ",";
  
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

//function to read the temp sensor and return the current temp in C
float getTemp() {
  return (analogRead(tempPin)*(5.0/1024)-.5)/.01;
}

