// This code will output the relative humidity using an analog humidity sensor (HIH-4030 Breakout, Sparkfun: SEN-09569).
// By: Spencer McDonald
// 2/11/2014

// To connect the sensor: See labels on the breakout board.
// The OU(T) in this code is connected to A1.

// Initalize Variables

#define LEDpin 13

int humiditySensor = 0;
float humiditySensorV = 0;
float humiditySensorRH = 0;

void setup()
{
  Serial.begin(115200); // Opens the communication between Arduino and computer serial port
  Serial.println("Relative humidity in % measured with analog humidity sensor:"); // Print header
  pinMode(LEDpin, OUTPUT);
}


void loop()
{ 
// Humidity Sensor
  humiditySensor = analogRead(A1);                                // Read the analog pin
  humiditySensorV = humiditySensor*(5.0/1024);                    // Convert the digital number to voltage
  humiditySensorRH = (humiditySensorV - (5.0*0.16))/(5.0*0.0062); // Convert the voltage to proper units
  Serial.print(humiditySensorRH,2);                               // Output the value
  Serial.println();                                               // New line
  
  digitalWrite(LEDpin,HIGH);                                      // Flash LED to show sensor running
  delay(1000);                                                    // Print about once per second
  digitalWrite(LEDpin,LOW);
}

