#include <TFT_eSPI.h>
#include <SPI.h>
#include "imgs.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 80

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.setRotation(3);

  tft.pushImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, fox);
  delay(7000);
  tft.setCursor(50, 30);
  tft.setTextFont(2);
  tft.fillScreen(TFT_BLACK);
  tft.println("connect!");
  delay(3000);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  // put your main code here, to run repeatedly/Users/myry/Docum/Users/myry/Documents/MyArduinoProject/AMW/01_Screen/ST7735/card.h /Users/myry/Documents/MyArduinoProject/AMW/01_Screen/ST7735/carl.h /Users/myry/Documents/MyArduinoProject/AMW/01_Screen/ST7735/carr.hents/MyArduinoProject/AMW/01_Screen/ST7735/caru.h:
  tft.setCursor(0, 0);
  tft.setTextFont(2);
  tft.println("x: ");
  tft.println("y: ");

  tft.pushImage(90, 55, 25, 25, card);
  tft.pushImage(90, 5, 25, 25, caru);
  tft.pushImage(60, 30, 25, 25, carl);
  tft.pushImage(120, 30, 25, 25, carr);
}