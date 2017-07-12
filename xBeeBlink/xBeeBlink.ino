/*   Code to control the activity of an LED via xBee communication and transmit basic data back
 *   Author: Ryan Bowers
 *   For use with Introduction to Arduino Activity 10
 */

#include <SoftwareSerial.h>

#define led 13

SoftwareSerial xBee = SoftwareSerial(2,3);
unsigned int numberOn = 0;
unsigned long timeOn = 0, turnedOn;
bool isOn = false;

void setup() {
  pinMode(led, OUTPUT);
  xBee.begin(9600);
}

void loop() {
  String command = "";
  while (xBee.available() > 0) {
    command += char(xBee.read());
    delay(10);
  }
  if (command.equals("FLIP")) {
    if (isOn) ledOff();
    else ledOn();
  }
  else if ((command.substring(0,2)).equals("BL")) {
    byte times = (command.substring(2, command.length())).toInt();
    ledBlink(times);
  }
  else if (command.equals("TIME"))
    xBee.println("\nTime on (s): " + String(timeOn/1000.0, 3));
  else if (command.equals("NUM"))
    xBee.println("\nTurned on " + String(numberOn) + " times.");
  else if (!command.equals(""))
    xBee.println("\nError - " + command + ": Command not recognized");
}

void ledOn() {
  if (!isOn) {
    digitalWrite(led, HIGH);
    turnedOn = millis();
    isOn = true;
    numberOn++;
  }
}

void ledOff() {
  if (isOn) {
    digitalWrite(led, LOW);
    timeOn += millis() - turnedOn;
    isOn = false;
  }
}

void ledBlink(byte times) {
  for (; times > 0; times--) {
    ledOn();
    delay(500);
    ledOff();
    delay(500);
  }
}


