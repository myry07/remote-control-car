#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
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

const char* ssid = "myry_ESP32";
const char* password = "wjq110422";

const char* serverX_left = "http://192.168.4.1/x_left";
const char* serverY_left = "http://192.168.4.1/y_left";
const char* serverState = "http://192.168.4.1/state";

unsigned long previousMillis = 0;
const long interval = 10; 

String numx;
String numy;
String state;

int valxl = 0;
int valyl = 0;
int s = 0;

String httpGETRequest(const char* serverName) {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(serverName);

  // Send HTTP GET request
  int httpResponseCode = http.GET();

  String payload = "--";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void control(int valxl, int valyl) {
  if (valxl > 3000 && s == 1) {  //前
    digitalWrite(leftfont, 1);
    digitalWrite(rightfont, 1);
    digitalWrite(leftback, 0);
    digitalWrite(rightback, 0);

    tft.fillCircle(92, 30, 3, TFT_PURPLE);
    tft.fillCircle(144, 64, 3, TFT_BLACK);
    tft.fillCircle(92, 105, 3, TFT_BLACK);
    tft.fillCircle(44, 64, 3, TFT_BLACK);
  }

  if (valxl < 1000 && s == 1) {  //后
    digitalWrite(leftfont, 0);
    digitalWrite(rightfont, 0);
    digitalWrite(leftback, 1);
    digitalWrite(rightback, 1);

    tft.fillCircle(92, 30, 3, TFT_BLACK);
    tft.fillCircle(144, 64, 3, TFT_BLACK);
    tft.fillCircle(92, 105, 3, TFT_PURPLE);
    tft.fillCircle(44, 64, 3, TFT_BLACK);
  }

  if (valyl > 3000 && s == 1) {  //右
    digitalWrite(leftfont, 1);
    digitalWrite(rightfont, 0);
    digitalWrite(leftback, 0);
    digitalWrite(rightback, 1);

    tft.fillCircle(92, 30, 3, TFT_BLACK);
    tft.fillCircle(144, 64, 3, TFT_PURPLE);
    tft.fillCircle(92, 105, 3, TFT_BLACK);
    tft.fillCircle(44, 64, 3, TFT_BLACK);
  }

  if (valyl < 1000 && s == 1) {  //左
    digitalWrite(leftfont, 0);
    digitalWrite(rightfont, 1);
    digitalWrite(leftback, 1);
    digitalWrite(rightback, 0);


    tft.fillCircle(92, 30, 3, TFT_BLACK);
    tft.fillCircle(144, 64, 3, TFT_BLACK);
    tft.fillCircle(92, 105, 3, TFT_BLACK);
    tft.fillCircle(44, 64, 3, TFT_PURPLE);
  }

  if (valxl > 1500 && valxl < 2000 && valyl > 1500 && valyl < 2000 && s == 1) {
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

  pinMode(leftfont, OUTPUT);
  pinMode(leftback, OUTPUT);
  pinMode(rightfont, OUTPUT);
  pinMode(rightback, OUTPUT);
  digitalWrite(leftfont, 0);
  digitalWrite(rightfont, 0);
  digitalWrite(leftback, 0);
  digitalWrite(rightback, 0);

  WiFi.begin(ssid, password);

  tft.init();
  tft.setRotation(3);
  tft.setTextFont(1);

  tft.pushImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, fox);
  tft.setCursor(0, 112);
  Serial.println("Connecting");

  tft.print("connecting ");

  for (int i = 0; i < 3; i++) {
    delay(1000);
    tft.print(".");
  }

  while (WiFi.status() != WL_CONNECTED) {
    tft.print(".");
    delay(1000);
  }

  tft.setTextFont(2);

  delay(1000);
  tft.setCursor(5, 34);
  tft.fillScreen(TFT_BLACK);
  tft.println("connected!");
  tft.setCursor(5, 54);
  tft.print("IP: ");
  tft.println(WiFi.localIP());
  delay(3000);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // 检查 WiFi 连接状态
    if (WiFi.status() == WL_CONNECTED) {
      // 调用 httpGETRequest 函数并将结果存储在 num 变量中
      numx = httpGETRequest(serverX_left);
      numy = httpGETRequest(serverY_left);
      state = httpGETRequest(serverState);

      valxl = numx.toInt();
      valyl = numy.toInt();
      s = state.toInt();
    }

    tft.setCursor(0, 0);
    tft.print("x: ");
    tft.println(valxl);
    tft.print("y: ");
    tft.println(valyl);

    tft.pushImage(60, 45, 70, 50, car1);

    if (s == 1) {
      control(valxl, valyl);
    }
    previousMillis = currentMillis;
  }
}
