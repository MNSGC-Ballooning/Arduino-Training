
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <SparkFun_ADXL345.h>



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   7
#define OLED_CLK   6
#define OLED_DC    5
#define OLED_CS    4
#define OLED_RESET 3
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

ADXL345 adxl = ADXL345(10); //accelerometer object, on pin 10
int x, y, z;                //variables to store accels

void setup() {
  Serial.begin(9600);

  adxl.powerOn();           //Initializes accelerometer
  adxl.setRangeSetting(16); //Will detect accels up to 16g. Can be lessened to 8, 4, or 2 to get increased precision
  adxl.setSpiBit(0);        //Using 4 wire SPI setup
  
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

void loop() {
  adxl.readAccel(&x, &y, &z); //write data to variables already defined
  Serial.println("x: " + String(x) + ", y: " + String(y) + ", z: " + String(z));  //Print data to serial monitor
 String Excel= "x: " + String(x) + ", y: " + String(y) + ", z: " + String(z);

  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE, BLACK);
  display.setCursor(10, 0); // Location of where the text begins
  display.println("Accelecaration is:");
 display.println(Excel);
  display.display();      // uploads the display on the OLED
  delay(100);
}
