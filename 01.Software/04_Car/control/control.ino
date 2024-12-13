#include <TFT_eSPI.h>
#include <SPI.h>
#include "imgs.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128

#define leftfont 34
#define leftback 35
#define rightfont 32
#define rightback 33

TFT_eSPI tft = TFT_eSPI();


void setup() {
  Serial.begin(115200);

  pinMode(leftfont, OUTPUT);
  pinMode(leftback, OUTPUT);
  pinMode(rightfont, OUTPUT);
  pinMode(rightback, OUTPUT);

  tft.init();
  tft.setRotation(3);
  tft.setTextFont(1);

  tft.pushImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, fox);
  tft.setCursor(0, 112);

  tft.print("connecting ");

  for (int i = 0; i < 7; i++) {
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

  tft.fillCircle(92, 30, 3, TFT_PURPLE);

  digitalWrite(leftfont, 1);
  digitalWrite(rightfont, 1);
  digitalWrite(leftback, 0);
  digitalWrite(rightback, 0);

  if (Serial.available() > 0) {         // 检查是否有可用的串口数据
    char incomingByte = Serial.read();  // 从串口读取一个字节的数据
    Serial.print("Received: ");
    Serial.println(incomingByte);

    if (incomingByte == 'w') {
      Serial.println("向前");
      //up
      tft.fillCircle(92, 30, 3, TFT_PURPLE);

      digitalWrite(leftfont, 1);
      digitalWrite(rightfont, 1);
      digitalWrite(leftback, 0);
      digitalWrite(rightback, 0);
    }

    if (incomingByte == 's') {
      Serial.println("向后");
      //down
      tft.fillCircle(92, 105, 3, TFT_PURPLE);

      digitalWrite(leftfont, 0);
      digitalWrite(rightfont, 0);
      digitalWrite(leftback, 1);
      digitalWrite(rightback, 1);
    }
  }

  //right
  // tft.fillCircle(144, 64, 3, TFT_PURPLE);


  //left
  // tft.fillCircle(44, 64, 3, TFT_PURPLE);
}
