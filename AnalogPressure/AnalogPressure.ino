// This code will output the pressure in psi using an analog pressure sensor (Honeywell Pressure Sensor: SSCSANN015PAAA5, Digi-Key: 480-3600-ND).
// By: Spencer McDonald
// 2/11/2014

// To connect the sensor: Flat side facing towards you, pins pointed down, starting at the left, GND - OUTPUT - 5V - NOTHING.
// This code has the OUTPUT connected to A0.

// Initalize Variables

#define LEDpin 13

int pressureSensor = 0;
float pressureSensorV = 0;
float psi = 0; 

void setup()
{
  Serial.begin(115200); // Opens the communication between Arduino and computer serial port
  Serial.println("Pressure in psi measured with analog pressure sensor:"); // Print header
  pinMode(LEDpin, OUTPUT);
}


void loop()
{ 
  // Pressure Sensor
  pressureSensor = analogRead(A0);                // Read the analog pin
  pressureSensorV = pressureSensor*(5.0/1024);    // Convert the digital number to voltage
  psi = (pressureSensorV - (0.1*5.0))/(4.0/15.0); // Convert the voltage to proper units
  Serial.print(psi,2);                            // Output the value
  Serial.println();                               // New line
   
  digitalWrite(LEDpin,HIGH);                      // Flash LED to show sensor running
  delay(1000);                                    // Print about once per second
  digitalWrite(LEDpin,LOW);
  
}

