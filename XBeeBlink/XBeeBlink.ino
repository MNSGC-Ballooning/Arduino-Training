/*   Code to control the activity of an LED via xBee communication and transmit basic data back
 *   Author: Ryan Bowers
 *   For use with Introduction to Arduino Activity 10
 */

#include <SoftwareSerial.h>
#include <RelayXBee.h>

#define ledPin 13
#define relayPin 7

String ID = "ID";   //Choose an ID for your XBee - 2-4 character string, A-Z and 0-9 only please

SoftwareSerial xBee_ser = SoftwareSerial(2,3);  //Pins to communicate with xBee
RelayXBee xBee = RelayXBee(&xBee_ser, ID);           //XBee object - connect to serial line

unsigned int numberOn = 0;  //variables to track system activity
unsigned long timeOn = 0, turnedOn;
bool isOn = false;
bool relay = false;

void setup() {
  pinMode(relayPin, INPUT_PULLUP);
  xBee_ser.begin(XBEE_BAUD);
  xBee.init('A');
  if (digitalRead(relayPin) == LOW) {     //ground pin 7 before startup to put the Arduino in Relay Mode
    relay = true;
    xBee.enterATmode();       //Configure XBee as a relay
    xBee.atCommand("ATDL1");
    xBee.atCommand("ATMY0");
    xBee.exitATmode();

    Serial.begin(9600);                 //Open the Serial line to the computer
    Serial.println("Relay Mode active");  //Tell user that this was done successfully
  }
  else
    pinMode(ledPin, OUTPUT);
}

void loop() {
  //Loop for relay system
  if (relay) {
    //Just pipe data from computer to xBee and vice versa (ignore xBee object)
    while (Serial.available() > 0) {
      xBee_ser.write(Serial.read());
    }
    while (xBee_ser.available() > 0) {
      Serial.write(xBee_ser.read());
    }
  }
  //Loop for payload system
  else {
    if(xBee_ser.available()>0){
      String command = xBee.receive();
    //check xBee for incoming messages that match ID
    if (command == "") return;        //Either no data was received or ID didn't match

    //Command responses
    if (command.equals("FLIP")) {   //toggle command
      if (isOn) ledOff();
      else ledOn();
    }
    else if ((command.substring(0,2)).equals("BL")) { //blink command
      byte times = (command.substring(2, command.length())).toInt();  //check to see how many times to blink
      ledBlink(times);
    }
    else if (command.equals("TIME"))  //report total time on command
      xBee.send("\nTime on (s): " + String(getTimeOn(), 3));
    else if (command.equals("NUM"))   //report times turned on command
      xBee.send("\nTurned on " + String(numberOn) + " times");
    else if (!command.equals(""))     //if a command was received, but not one of the above, display an error
      xBee.send("\nError - " + command + ": Command not recognized");
  }
}}

//turns led on and saves time and number
void ledOn() {
  if (!isOn) {
    digitalWrite(ledPin, HIGH);
    turnedOn = millis();
    isOn = true;
    numberOn++;
  }
}

//turns led off and adds time to total
void ledOff() {
  if (isOn) {
    digitalWrite(ledPin, LOW);
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
