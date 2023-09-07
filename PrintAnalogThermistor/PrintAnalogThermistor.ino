/*   Code to read a thermistor, and record both to an SD card.
 *   Author: Ethan Thompson-Jewell
 *   For use with Introduction to Arduino Activity 7
 */

#include <PT_Thermistor.h>    // Custom Library to use Thermistors. https://github.com/MNSGC-Ballooning/Arduino-Training/blob/371a4ea215232b718c0e399c07738652866bdf11/Thermistor%20Library.zip 

#define ledPin 5
#define tempPin A2 //pin that the thermistor should be read from


//set up thermistor 
  Thermistor Thermistor(tempPin); 

void setup() {
  Serial.begin(115200);     //start Serial communication
  pinMode(ledPin, OUTPUT);  //setup LED pin
  
  Thermistor.begin(12); //12 bits of analog resolution on arduino nano 33

  String header = "Temp (C),Temp(F)";  //setup data format, and print it to the monitor and SD card
  Serial.println(header);
}

void loop() {
 

    String data = String(getTempC());  //get temp and write to data string
    data = data + "," + String(getTempF());
    
    digitalWrite(ledPin, HIGH);       //flash LED to show data being logged
    Serial.println(data);             //print data to monitor...
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
