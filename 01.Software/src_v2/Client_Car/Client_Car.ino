#include <WiFi.h>
#include <esp_now.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "imgs.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128

#define leftfont 13
#define leftback 12
#define rightfont 14
#define rightback 27

TFT_eSPI tft = TFT_eSPI();

typedef struct Car {
  int x;
  int y;
  int state;
} Car;

Car c = { 1800, 1800, 0 };

int temp = 0;

// 回调函数
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
  memcpy(&c, incomingData, sizeof(c));
}

void control() {
  if (c.x > 3000 && c.state == 1) {  //前
    digitalWrite(leftfont, 1);
    digitalWrite(rightfont, 1);
    digitalWrite(leftback, 0);
    digitalWrite(rightback, 0);

    tft.fillCircle(92, 30, 3, TFT_PURPLE);
    tft.fillCircle(144, 64, 3, TFT_BLACK);
    tft.fillCircle(92, 105, 3, TFT_BLACK);
    tft.fillCircle(44, 64, 3, TFT_BLACK);
  }

  if (c.x < 1000 && c.state == 1) {  //后
    digitalWrite(leftfont, 0);
    digitalWrite(rightfont, 0);
    digitalWrite(leftback, 1);
    digitalWrite(rightback, 1);

    tft.fillCircle(92, 30, 3, TFT_BLACK);
    tft.fillCircle(144, 64, 3, TFT_BLACK);
    tft.fillCircle(92, 105, 3, TFT_PURPLE);
    tft.fillCircle(44, 64, 3, TFT_BLACK);
  }

  if (c.y > 3000 && c.state == 1) {  //右
    digitalWrite(leftfont, 1);
    digitalWrite(rightfont, 0);
    digitalWrite(leftback, 0);
    digitalWrite(rightback, 1);

    tft.fillCircle(92, 30, 3, TFT_BLACK);
    tft.fillCircle(144, 64, 3, TFT_PURPLE);
    tft.fillCircle(92, 105, 3, TFT_BLACK);
    tft.fillCircle(44, 64, 3, TFT_BLACK);
  }

  if (c.y < 1000 && c.state == 1) {  //左
    digitalWrite(leftfont, 0);
    digitalWrite(rightfont, 1);
    digitalWrite(leftback, 1);
    digitalWrite(rightback, 0);


    tft.fillCircle(92, 30, 3, TFT_BLACK);
    tft.fillCircle(144, 64, 3, TFT_BLACK);
    tft.fillCircle(92, 105, 3, TFT_BLACK);
    tft.fillCircle(44, 64, 3, TFT_PURPLE);
  }

  if (c.x > 1500 && c.x < 2000 && c.y > 1500 && c.y < 2000 && c.state == 1) {
    digitalWrite(leftfont, 0);
    digitalWrite(rightfont, 0);
    digitalWrite(leftback, 0);
    digitalWrite(rightback, 0);

    tft.fillCircle(92, 30, 3, TFT_BLACK);
    tft.fillCircle(144, 64, 3, TFT_BLACK);
    tft.fillCircle(92, 105, 3, TFT_BLACK);
    tft.fillCircle(44, 64, 3, TFT_BLACK);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(2, OUTPUT);
  pinMode(leftfont, OUTPUT);
  pinMode(leftback, OUTPUT);
  pinMode(rightfont, OUTPUT);
  pinMode(rightback, OUTPUT);

  tft.init();
  tft.setRotation(3);
  tft.setTextFont(1);

  tft.pushImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, fox);
  tft.setCursor(0, 112);

  tft.print("initializing ");

  for (int i = 0; i < 3; i++) {
    delay(1000);
    tft.print(".");
  }

  while (esp_now_init() != ESP_OK) {
    tft.print(".");
    delay(1000);
  }

  // 注册接收回调函数
  if (esp_now_register_recv_cb(OnDataRecv) != ESP_OK) {
    tft.println("Error");
    return;
  }

  tft.setTextFont(2);

  delay(1000);
  tft.setCursor(5, 34);
  tft.fillScreen(TFT_BLACK);
  tft.println("initialized!");
  tft.setCursor(5, 54);
  tft.print("MAC: ");
  tft.print(WiFi.macAddress());
  delay(3000);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  if (c.state == 1) {

    if (temp == 0) {
      tft.pushImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, fox);
      tft.setCursor(0, 112);

      tft.print("controller conneting ");

      for (int i = 0; i < 3; i++) {
        delay(1000);
        tft.print(".");
      }
      tft.fillScreen(TFT_BLACK);
    }

    temp = -1;
    digitalWrite(2, 1);
  } else {
    temp = 0;
    digitalWrite(2, 0);
  }

  tft.setCursor(0, 0);
  tft.print("x: ");
  tft.println(c.x);
  tft.print("y: ");
  tft.println(c.y);

  tft.pushImage(60, 45, 70, 50, car1);

  control();
}
