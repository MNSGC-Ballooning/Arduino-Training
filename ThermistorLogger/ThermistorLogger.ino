/*   Code to read a thermistor, and record both to an SD card.
 *   Author: Ethan Thompson-Jewell
 *   For use with Introduction to Arduino Activity 7
 */

#include <SD.h>
#include <PT_Thermistor.h>    // Custom Library to use Thermistors. https://github.com/MNSGC-Ballooning/Arduino-Training/blob/371a4ea215232b718c0e399c07738652866bdf11/Thermistor%20Library.zip 

#define chipSelect 8  //Sparkfun micro-SD shield

#define ledPin 5
#define tempPin A2 //pin that the thermistor should be read from

File datalog;                     //File object for datalogging
char filename[] = "LOGGER00.csv"; //Template for file name to save data
bool SDactive = false;            //used to check for SD card before attempting to log

//set up thermistor 
  Thermistor Thermistor(tempPin); 

void setup() {
  Serial.begin(115200);     //start Serial communication
  pinMode(ledPin, OUTPUT);  //setup LED pin
  pinMode(10, OUTPUT);      //Needed for SD library, regardless of shield used
  
  //setting up SD card
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
  
  Thermistor.begin(12); //12 bits of analog resolution on arduino nano 33

  String header = "Temp (C),Temp(F)";  //setup data format, and print it to the monitor and SD card
  Serial.println(header);
  if (SDactive) {
    datalog.println(header);
    datalog.close();
  }
}

void loop() {
 

    String data = String(getTempC());  //get temp and write to data string
    data = data + "," + String(getTempF());
    
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

//function to read the temp sensor and return the current temp in F
float getTempF() {
  Thermistor.update();
  return Thermistor.getTempF();
}
//function to read the temp sensor and return the current temp in C
float getTempC() {
  Thermistor.update();
  return Thermistor.getTempC();
}
