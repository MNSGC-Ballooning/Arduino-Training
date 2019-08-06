// Arduino code to monitor one RM-60
// Will count hits in 5-second interval

//begin SD initialzie variables/library
#include <SD.h> 

//resume SD stuff - set up to create files
File datalog;
char filename[] = "LOGGER00.csv";

const int chipSelect = 10; // specific to Adafruit datashield
//end SD initialize variables

//begin Geiger initialize variables
long unsigned int timer = 0; 
long unsigned int Logtime = 5000; // Logging time in milliseconds
long unsigned int LocalTime = 0;
long unsigned int LoopLog = 150;
long int counter1 = 0; // Local Counter for Geiger counter sensor 1 hits
//end Geiger initalize variables 


void setup() {

// connect at 115200 
  Serial.begin(115200); 
  

//Initialize SD Card
  Serial.print("Initializing SD Card...");

  pinMode(SS, OUTPUT);

if(!SD.begin(10)) // for Mega and Adafruit datashield
  {
    Serial.println("Card failed, or not present");
    //digitalWrite(led, HIGH);
    //delay(1000);
    return;
  }
  Serial.println("Card initialized.");  
  Serial.print("Creating File...");

  // Make a new file each time the arduino is powered
  for (uint8_t i = 0; i < 100; i++) 
  {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) 
    {
      // only open a new file if it doesn't exist
      datalog = SD.open(filename, FILE_WRITE); 
      break;
    }  
}

Serial.print("Logging to: ");
  Serial.println(filename);

  if(!datalog)
  {  
    Serial.println("Couldn't Create File"); 
    delay(1000);
    Serial.println("Couldn't Create File");
    return;
  }

 // Print Header
  String Header =  "S1 Hit Count";

  datalog = SD.open(filename, FILE_WRITE);
  datalog.println(Header);
  Serial.println(Header); 
 
  datalog.close(); 
}


void loop() {
  
  // Begin Geiger loop
  timer = millis();

while((millis()-timer) < Logtime)
  {
    LocalTime = micros();
    int sensor1 = digitalRead(4); // Read in the pin for sensor 1. Duplicate for multiple geiger counters
        
    if(sensor1==LOW)
    {
      counter1++;
       
    }
    
      
   while((micros()-LocalTime)<LoopLog)
    {
      delayMicroseconds(5);  //slow code down if needed, to let Geiger counters reset. 
    }    
  }

  datalog = SD.open(filename, FILE_WRITE); //starts writing on the SD Card
  datalog.print(counter1);
  datalog.println();
  datalog.close();
  delay(100);


//This is stuff printed on the Serial Monitor  
  Serial.print(counter1);  
  Serial.println();

  // Reset short-term counters but not cumulative counters 
  counter1=0;
  
  }// end loop()


