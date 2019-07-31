#include  <RelayXBee.h>
#include <SoftwareSerial.h>

#define led 8
#define relayPin 9
#define temperature_pin A0
String ID = "ID";
String command;

SoftwareSerial ss(4,5);  //Pins to communicate with xBee
RelayXBee xBee = RelayXBee(&ss, ID);


unsigned long int timer = 0;
unsigned int numberOn = 0;  //variables to track system activity
unsigned long timeOn = 0, turnedOn;
bool isOn = false;
bool relay = false;


void setup() {
    pinMode(relayPin, INPUT_PULLUP);
    pinMode(led, OUTPUT);
    xBee.init('A');
    Serial.begin(9600); 
    ss.begin(XBEE_BAUD);
    Serial.println("XBEE INITIALIZED");//Open the Serial line to the computer
}


double temperature(){ //for a tmp36 sensor ******************
  double tempSensor = analogRead(temperature_pin);
  double voltage = (tempSensor/1024.0)*5.0;
  double temp = (voltage - .5) * 100.0; 
  return temp;
  }
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

void ledBlink(int timess) {
  for (; timess > 0; timess--) {
    ledOn();
    delay(500);
    ledOff();
    delay(500);
  }
}

// the loop routine runs over and over again forever:
void loop() {

  if(millis()-10000>timer){
    ss.print("ping");
    timer=millis();
  //Serial.println("in loop");
    if(ss.available()>0){
    command = xBee.receive(); 
    Serial.println(command);//check xBee for incoming messages that match ID        //Either no data was received or ID didn't match
    //Command responses
    if (command.startsWith("T")) {   //toggle command
      xBee.send("\n Temperature (C) = " + String(temperature()));}

    else if (command.startsWith("A")) {   //toggle command
      if (isOn) ledOff();
      else ledOn();
    }
    else if (command.startsWith("+")){ //report total time on command
      int times = (command.substring(1, command.length())).toInt();
      ledBlink(times);}
    else if (!command.equals(""))     //if a command was received, but not one of the above, display an error
      xBee.send("\nError - " + command + ": Command not recognized");
  }//}


//blinks led a specified number of times
