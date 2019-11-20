#define ledPin 13    //built-in LED
#define tempPin A2   //monitor TMP36 on analog pin 2

void setup() {
  Serial.begin(9600);
}

void loop() {
  String data = String(getTemp());
  Serial.println(data);             //print data to monitor
  digitalWrite(ledPin, HIGH);       //flash LED to show data being logged
  delay(250);                       //have LED blink for quarter-second
  digitalWrite(ledPin, LOW);        //then turn it off
  delay(750);                       //complete a full second cycle before logging again
}

//function to read the temp sensor and return the current temp in C
float getTemp() {
  return (analogRead(tempPin)*(5.0/1024)-.5)/.01;
}
