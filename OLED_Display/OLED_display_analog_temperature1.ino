#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>



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

#define tempPin A2

void setup() {
  Serial.begin(115200);     //start Serial communication

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

 String data = String(getTemp());
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE, BLACK);
  display.setCursor(10, 0);
  display.println("Temperature is:");
 display.println(data);
 Serial.println(data);
  display.display();      // Updates the display on the OLED
  delay(100);
}

//function to read the temp sensor and return the current temp in C
float getTemp() {
  return ((analogRead(tempPin)*(5.0/1024)-.5)/.01);// FINDS WHAT THE TEMP IS
}
