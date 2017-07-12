/*   Code to control the activity of an LED via xBee communication and transmit basic data back
 *   Author: Ryan Bowers
 *   For use with Introduction to Arduino Activity 10
 */

#include <SoftwareSerial.h>

#define led 13

SoftwareSerial xBee = SoftwareSerial(2,3);  //Pins to communicate with xBee
unsigned int numberOn = 0;  //variables to track system activity
unsigned long timeOn = 0, turnedOn;
bool isOn = false;

void setup() {
  pinMode(led, OUTPUT);
  xBee.begin(9600);
}

void loop() {
  String command = "";            //start with an empty string
  while (xBee.available() > 0) {
    command += char(xBee.read()); //add to it char by char when receiving xBee transmissions
    delay(10);                    //wait briefly for more data to arrive before checking
  }
  if (command.equals("FLIP")) {   //toggle command
    if (isOn) ledOff();
    else ledOn();
  }
  else if ((command.substring(0,2)).equals("BL")) { //blink command
    byte times = (command.substring(2, command.length())).toInt();  //check to see how many times to blink
    ledBlink(times);
  }
  else if (command.equals("TIME"))  //report total time on command
    xBee.println("\nTime on (s): " + String(getTimeOn(), 3));
  else if (command.equals("NUM"))   //report times turned on command
    xBee.println("\nTurned on " + String(numberOn) + " times.");
  else if (!command.equals(""))     //if a command was received, but not one of the above, display an error
    xBee.println("\nError - " + command + ": Command not recognized");
}

//turns led on and saves time and number
void ledOn() {
  if (!isOn) {
    digitalWrite(led, HIGH);
    turnedOn = millis();
    isOn = true;
    numberOn++;
  }
}

//turns led off and adds time to total
void ledOff() {
  if (isOn) {
    digitalWrite(led, LOW);
    timeOn += millis() - turnedOn;
    isOn = false;
  }
}

//blinks led a specified number of times
void ledBlink(byte times) {
  for (; times > 0; times--) {
    ledOn();
    delay(500);
    ledOff();
    delay(500);
  }
}

//returns time LED has been on, in seconds
float getTimeOn() {
  if (!isOn) return timeOn/1000.0;
  else return (timeOn + millis() - turnedOn)/1000.0;
}

