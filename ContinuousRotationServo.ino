//  Continuous Rotation Servo
//  Author: Zoe Foster

#include <Servo.h>    // Use Servo library

Servo myServo;        // Create Servo object to control the servo 

void setup() { 
  Serial.begin(9600); // Start serial monitor at speed of 9600 baud
  myServo.attach(9);  // Servo object connected to pin 9 
  myServo.write(90);  // Start with no rotation
} 

void loop() {

  /*//Basic rotation sequence
  myServo.write(180); // Counter-clockwise rotation
  delay(2000);
  myServo.write(90);  // No rotation
  delay(2000);
  myServo.write(0);   // Clockwise rotation
  delay(2000);
  */

  //Enter desired speed/rotation into serial input
  Serial.println("Enter the speed");
  while(!Serial.available()) {}   // Wait for keyboard input
  String input = Serial.readStringUntil(10);  // Read keyboard input
  Serial.println("Speed entered: "+input);
  myServo.write(input.toInt());   // Change rotation to input
}
