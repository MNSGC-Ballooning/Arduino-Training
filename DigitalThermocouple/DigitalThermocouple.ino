/*    Code for use with Adafruit Type K Thermocouple
      Author: Billy Straub
      

   Arduino Uno pin connections for this example code:
   -------------------------------------------------------------------------------------------------------------------------
     Component                    | Pins used               | Notes
     -                              -                         -
     Adafruit Thermocouple #1     | D13 D12 D11 D10         | SCK, SDO, SDI, CS pins in that order
     Adafruit Thermocouple #2     | D13 D12 D11 D9          | SCK, SDO, SDI, CS pins in that order
   -------------------------------------------------------------------------------------------------------------------------


Note: 
When wiring the thermocouples, the SCK, SDO, and SDI lines can all be connected and run into the same digital ports.
The cs pins, on the other hand, have to be given their own individual digital pins.

For example, if there were two thermocouples, their SCK pins could be wired together and both run to D13.
Also, their SDO pins could be wired together and both run to D12.
Additionally, their SDI pins could be wired together and both run to D11.
On the other hand, the cs (chip select) pins require their own digital pins, so thermocouple #1 might have cs run to D10.
Since thermocouple #2 must use a different cs pin, it could run to D9.

This wiring setup is known as an SPI (Serial Peripheral Interface) connection and is required by specific devices, such as this Adafruit thermocouple.
*/


#include <Adafruit_MAX31856.h>  //Adafruit thermocouple library


//For the use of software SPI, uncomment the following line.
//Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10, 11, 12, 13);

//For the use of hardware SPI, uncomment the following line.
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10);
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(9);



void setup() {
  
  Serial.begin(9600);                                 //Starts the serial monitor

  maxthermo1.begin();                                 //Starts the thermocouple
  maxthermo2.begin();
  maxthermo1.setThermocoupleType(MAX31856_TCTYPE_K);  //Sets the thermocouple to be type K from the library
  maxthermo2.setThermocoupleType(MAX31856_TCTYPE_K);
      
  Serial.print("Thermocouple type K: ");              //Simply prints the words in quotation marks when started up

  delay(2000);                                        //Waits two seconds before the loop starts
  
}



void loop() {
  
  Serial.print("Thermocouple #1 Temp: ");                         //Simply prints the words in quotation marks before listing the follow temperature from the line below.
  Serial.println(maxthermo1.readThermocoupleTemperature());     //Prints the thermocouple temperature
  Serial.print("Thermocouple #2 Temp: ");
  Serial.println(maxthermo2.readThermocoupleTemperature());

  delay(1000);                                                 //Waits one second before restarting the loop
  
}
