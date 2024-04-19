#include <TFT_eSPI.h>
#include <SPI.h>
#include "imgs.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128

#define TX_Port 1
#define RX_Port 3

TFT_eSPI tft = TFT_eSPI();

void setup() {
  pinMode(TX_Port, OUTPUT);
  digitalWrite(TX_Port, 1);

  pinMode(RX_Port, OUTPUT);
  digitalWrite(RX_Port, 1);

  tft.init();
  tft.setRotation(3);
  tft.setTextFont(1);

  tft.pushImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, fox);
  tft.setCursor(0, 112);

  tft.print("connecting ");

  for(int i = 0; i < 7; i++) {
    delay(1000);
    tft.print(".");
  } 

  tft.setTextFont(2);

  delay(1000);
  tft.setCursor(50, 54);
  tft.fillScreen(TFT_BLACK);
  tft.println("connected!");
  delay(3000);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  // put your main code here, to run repeatedly
  tft.setCursor(0, 0);
  tft.println("x: ");
  tft.println("y: ");

  tft.pushImage(60, 45, 70, 50, car1);

  //up
  tft.fillCircle(92, 30, 3, TFT_PURPLE);

  //right
  tft.fillCircle(144, 64, 3, TFT_PURPLE);

  //down
  tft.fillCircle(92, 105, 3, TFT_PURPLE);

  //left
  tft.fillCircle(44, 64, 3, TFT_PURPLE);
};

