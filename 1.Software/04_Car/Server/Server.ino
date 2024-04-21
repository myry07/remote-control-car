#include <Wire.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <TFT_eSPI.h>
#include <SPI.h>

const char* ssid = "myry_ESP32";
const char* password = "wjq110422";

TFT_eSPI tft = TFT_eSPI();

#define pinXl 32  // 遥感x轴
#define pinYl 33  // 遥感y轴
#define pinXr 34
#define pinYr 35

int valxl = 0;
int valyl = 0;
int valxr = 0;
int valyr = 0;

int state = 0;

AsyncWebServer server(80);  //Create an asynchronous web server on port 80.

int getXl() {
  return valxl;
}

int getYl() {
  return valyl;
}

int getState() {
  return state;
}

void draw() {
  if(valxl > 3000) {
    tft.fillRect(0, 0, 480, 80, TFT_PURPLE);
  }

  if(valxl < 1000) {
    tft.fillRect(0, 320, 480, 80, TFT_PURPLE);
  }

  if(valyl > 3000) {
    tft.fillRect(0, 0, 80, 320, TFT_PURPLE);
  }

  if(valyl < 1000) {
    tft.fillRect(400, 0, 80, 320, TFT_PURPLE);
  }

  if(valxl > 1500 && valxl < 2000 && valyl > 1500 && valyl < 2000) {
    tft.fillScreen(TFT_BLACK);
  }
}




void setup() {
  Serial.begin(115200);
  Serial.print("Setting AP");
  WiFi.softAP(ssid, password);  //Set your ESP32 as an access point with the SSID name and password defined earlier.

  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  // tft.setCursor(100, 100);
  // IPAddress IP = WiFi.softAPIP();
  // tft.print("IP: ");
  // tft.println(IP);


  server.on("/x_left", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", String(getXl()).c_str());
  });

  server.on("/y_left", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", String(getYl()).c_str());
  });

  server.on("/state", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", String(getState()).c_str());
  });


  server.begin();
}

void loop() {
  valxl = analogRead(pinXl);  // 读取x轴的值并转换为变量
  valyl = analogRead(pinYl);  // 读取y轴的值并转换为变量
  valxr = analogRead(pinXr);
  valyr = analogRead(pinYr);

  if (valxr > 3000) {
    state = 1;
    draw();
  }

  if (valxr < 1000) {
    state = 0;
  }
}