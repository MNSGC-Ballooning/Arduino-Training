#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <SparkFun_ADXL345.h>
#define LEDpin 13

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   7
#define OLED_CLK   6
#define OLED_DC    5
#define OLED_CS    3
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

int pressureSensor = 0;
float pressureSensorV = 0;
float psi = 0; 

void setup()
{
  Serial.begin(9600); // Opens the communication between Arduino and computer serial port
  Serial.println("Pressure in psi measured with analog pressure sensor:"); // Print header

// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 1 seconds

  // Clear the buffer
  display.clearDisplay();
  
}


void loop()
{ 
  // Pressure Sensor
  pressureSensor = analogRead(A0);                // Read the analog pin
  pressureSensorV = pressureSensor*(5.0/1024);    // Convert the digital number to voltage
  psi = (pressureSensorV - (0.1*5.0))/(4.0/15.0); // Convert the voltage to proper units
  Serial.print(psi,2);                            // Output the value
  Serial.println();                               // New line

  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE, BLACK);
  display.setCursor(10, 0);
  display.println("Pressure is:");
 display.println(String(psi));
  display.display();      // Show initial text
  delay(100);
}
