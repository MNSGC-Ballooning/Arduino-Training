int ledPin = 8;
int ledPin2 = 13;
int inPin = 2;
int inPin2 = 4;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(inPin, INPUT);
  pinMode(inPin2, INPUT);
}

void loop() {
  if(digitalRead(inPin) == HIGH)
  {
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, LOW);
  }
  if(digitalRead(inPin2) == HIGH)
  {
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin, LOW);
  }
}
