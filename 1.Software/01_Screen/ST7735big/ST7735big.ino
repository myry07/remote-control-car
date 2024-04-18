#include <TFT_eSPI.h>
#include <SPI.h>
#include "imgs.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.setRotation(3);
  tft.setTextFont(2);

  tft.pushImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, fox);
  delay(7000);
  tft.setCursor(50, 54);
  tft.fillScreen(TFT_BLACK);
  tft.println("connected!");
  delay(3000);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  // put your main code here, to run repeatedly/Users/myry/Docum/Users/myry/Documents/MyArduinoProject/AMW/01_Screen/ST7735/card.h /Users/myry/Documents/MyArduinoProject/AMW/01_Screen/ST7735/carl.h /Users/myry/Documents/MyArduinoProject/AMW/01_Screen/ST7735/carr.hents/MyArduinoProject/AMW/01_Screen/ST7735/caru.h:
  tft.setCursor(0, 0);
  tft.println("x: ");
  tft.println("y: ");
}