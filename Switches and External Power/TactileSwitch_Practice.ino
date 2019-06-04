int ledPin = 8;
int inPin = 7;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(inPin, INPUT);
}

void loop() {
  if( digitalRead(inPin) == HIGH )
  {
    while(true){
      digitalWrite(ledPin, HIGH);
      delay(1000);
      digitalWrite(ledPin, LOW);
      delay(1000);
      /*if(digitalRead(inPin) == HIGH)    If you want to be able to reset the loop
      {
        break;
      }*/
    }
    //delay(5000);
  }
}
