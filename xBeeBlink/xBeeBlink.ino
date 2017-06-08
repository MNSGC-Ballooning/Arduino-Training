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
  while (xBee.available() > 1) {
    command += xBee.read();
  }
  if (command.equals("ON")) 
    ledOn();
  else if (command.equals("OFF"))
    ledOff();
  else if (command.equals("TIME"))
    xBee.println("Time on (s): " + String(timeOn/1000.0, 3));
  else if (command.equals("NUM"))
    xBee.println("Turned on " + String(numberOn) + " times.");
  else if (!command.equals(""))
    xBee.println("Error - " + command + ": Command not recognized");
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


