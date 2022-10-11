/*   Code to print an analog temp sensor.
 *   Author: Billy Straub
 *   For use with Introduction to Arduino Activity 
 *   https://github.com/MNSGC-Ballooning/AEM4490-Example-Code/blob/master/Print_Analog_Temp_Sensor/Print_Analog_Temp_Sensor.ino
 *   
 */
 
 //TMP36 Pin Variables
int sensorPin = A2; //the analog pin the TMP36's Vout (sense) pin is connected to

 
/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup()                        // This cond runs on 9600 baud rate. if your code does not work CHECK YOUR BAUDRATE in bottom right of your Serial monitor!!!
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor 
  Serial.println("Beginning Serial monitor...");
}
 
void loop()                     // run over and over again
{
 //getting the voltage reading from the temperature sensor
 int reading = analogRead(sensorPin);  
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 5.0;
 voltage /= 1024.0; 

 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
 Serial.print(temperatureC); Serial.println(" degrees C");
 
 // now convert to Fahrenheit
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 Serial.print(temperatureF); Serial.println(" degrees F");
 
 delay(1000);                                     //waiting a second
}
