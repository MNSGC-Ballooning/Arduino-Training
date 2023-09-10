/*   Code to read a thermistor, and record both to an SD card.
 *   Author: Ethan Thompson-Jewell
 *   For use with Introduction to Arduino Activity 7
 */

//#include <PT_Thermistor.h>  // Custom Library to use Thermistors. https://github.com/MNSGC-Ballooning/Arduino-Training/blob/371a4ea215232b718c0e399c07738652866bdf11/Thermistor%20Library.zip

#define ledPin 5
#define tempPin A2          //pin that the thermistor should be read from
int analogResolution = 10;  //10 for arduino uno
int currentTempC = -99;
int currentTempF = -99;
float adcMax;                 // The maximum adc value given to the thermistor
float A = 0.001125308852122;  // A, B, and C are constants used for a 10k resistor and 10k thermistor for the steinhart-hart equation
float B = 0.000234711863267;
float C = 0.000000085663516;
float R1 = 10000;  // 10k Ω resistor
float Tinv;
float adcVal;
float logR;
float T;  // these three variables are used for the calculation from adc value to temperature


#define ledPin 5
#define tempPin A2 //pin that the thermistor should be read from

void setup() {
  Serial.begin(9600);     //start Serial communication
  pinMode(ledPin, OUTPUT);  //setup LED pin

  String header = "Temp (C),Temp(F)";  //setup data format, and print it to the monitor and SD card
  Serial.println(header);
}

void loop() {

  String data = String(getTempC());  //get temp and write to data string
  data = data + "," + String(getTempF());

  digitalWrite(ledPin, HIGH);  //flash LED to show data being logged
  Serial.println(data);        //print data to monitor...
  delay(250);                  //have LED blink for quarter-second
  digitalWrite(ledPin, LOW);   //then turn it off
  delay(750);                  //complete a full second cycle before logging again
 
}

//function to read the temp sensor and return the current temp in F
float getTempF() {
  update();
  return currentTempF;
}
//function to read the temp sensor and return the current temp in C
float getTempC() {
  update();
  return currentTempC;
}

void update() {

  adcMax = pow(2, analogResolution) - 1.0;
  adcVal = analogRead(tempPin);
  logR = log(((adcMax / adcVal) - 1) * R1);
  Tinv = A + B * logR + C * logR * logR * logR;
  T = 1 / Tinv;
  currentTempC = T - 273.15;  // converting to celcius
  currentTempF = currentTempC * 9 / 5 + 32;
}

