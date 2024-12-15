#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#include "square.h"
#include "imgs.h"

// 初始化 TB6612FNG 引脚
#define AIN1 14  // A 通道方向控制引脚 1
#define AIN2 27  // A 通道方向控制引脚 2
#define PWMA 25  // A 通道 PWM 输入引脚
#define BIN1 12  // B 通道方向控制引脚 1
#define BIN2 13  // B 通道方向控制引脚 2
#define PWMB 26  // B 通道 PWM 输入引脚
#define STBY 33  // 待机引脚

// PWM 通道设置
const int PWMChannelA = 0;
const int PWMChannelB = 1;
const int PWMFrequency = 5000;
const int PWMResolution = 8;  // 8 位分辨率 (0-255)

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128

const char* ssid = "myry_ESP32";
const char* password = "wjq110422";

const char* server_start = "http://192.168.4.1/start";
const char* server_fb = "http://192.168.4.1/fontback";
const char* server_lr = "http://192.168.4.1/leftright";
const char* server_pwm = "http://192.168.4.1/pwm";

String start;
String fb;
String lr;
String pwm;

int change = 1;

TFT_eSPI tft = TFT_eSPI();  // 初始化 TFT 显示屏


String httpGETRequest(const char* serverName) {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(serverName);

  // Send HTTP GET request
  int httpResponseCode = http.GET();

  String payload = "--";

  // if (httpResponseCode > 0) {
  //   Serial.print("HTTP Response code: ");
  //   Serial.println(httpResponseCode);
  //   payload = http.getString();
  // } else {
  //   Serial.print("Error code: ");
  //   Serial.println(httpResponseCode);
  // }
  // Free resources
  http.end();

  return payload;
}

void motorInit() {
  // 设置 TB6612FNG 引脚
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);  // 使能 TB6612FNG

  // 设置 PWM 通道
  ledcSetup(PWMChannelA, PWMFrequency, PWMResolution);
  ledcSetup(PWMChannelB, PWMFrequency, PWMResolution);

  // 绑定 PWM 通道到引脚
  ledcAttachPin(PWMA, PWMChannelA);
  ledcAttachPin(PWMB, PWMChannelB);
}

// 控制电机 A 方向与速度
void motorA(int speed, bool direction) {
  digitalWrite(AIN1, direction);   // 设置方向
  digitalWrite(AIN2, !direction);  // 相反方向
  ledcWrite(PWMChannelA, speed);   // 设置速度 (0-255)
}

// 控制电机 B 方向与速度
void motorB(int speed, bool direction) {
  digitalWrite(BIN1, direction);   // 设置方向
  digitalWrite(BIN2, !direction);  // 相反方向
  ledcWrite(PWMChannelB, speed);   // 设置速度 (0-255)
}

//绘制初始画面
void drawInit() {
  tft.pushImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, fox);
  tft.setCursor(0, 112);

  tft.print("initializing ");

  for (int i = 0; i < 3; i++) {
    delay(1000);
    tft.print(".");
  }

  while (true) {
    delay(1000);
    tft.print(".");
    if (WiFi.status() == WL_CONNECTED) {
      break;
    }
  }
}

void drawMain() {
  tft.pushImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, fox);
  tft.setCursor(45, 112);
  tft.print("wait for connect");
  change = 1;
}

void moveCar(int x, int y, int pwm) {
  if (x == 1) {  //font
    motorA(pwm, true);
    motorB(pwm, true);
    Serial.println("font");
  } else if (x == 2) {  //back
    motorA(pwm, false);
    motorB(pwm, false);
    Serial.println("back");
  } else if (y == 1) {  //left
    motorA(pwm, false);
    motorB(pwm, true);
    Serial.println("left");
  } else if (y == 2) {  //right
    motorA(pwm, true);
    motorB(pwm, false);
    Serial.println("right");
  } else {
    motorA(0, true);
    motorB(0, true);
    Serial.println("stop");
  }
}

void drawDrive(int x, int y, int pwm) {
  if (change == 1) {
    tft.fillScreen(TFT_BLACK);
  }
  change = 0;

  tft.pushImage(60, 45, 70, 50, car1);
  tft.setCursor(10, 20);
  tft.print("x: ");
  tft.println(x);
  tft.setCursor(10, 30);
  tft.print("y: ");
  tft.println(y);
  tft.setCursor(10, 40);
  tft.print("pwm: ");
  tft.println(pwm);

  // tft.fillRect(30, 20, 20, 10, TFT_BLACK);
  // tft.fillRect(30, 30, 20, 10, TFT_BLACK);
  moveCar(x, y, pwm);

  // delay(10);
}

void setup() {
  tft.init();
  tft.setRotation(3);
  tft.setTextFont(1);
  tft.fillScreen(TFT_BLACK);
  WiFi.mode(WIFI_STA);

  Serial.begin(115200);  // 初始化 USB 串口监视器

  WiFi.begin(ssid, password);

  drawInit();
  motorInit();

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(30, 60);
  tft.print("Wi-Fi connected!");
  delay(4000);
}

void loop() {
  start = httpGETRequest(server_start);
  int current_start = start.toInt();

  fb = httpGETRequest(server_fb);
  int current_fb = fb.toInt();

  lr = httpGETRequest(server_lr);
  int current_lr = lr.toInt();

  pwm = httpGETRequest(server_pwm);
  int current_pwm = pwm.toInt();

  Serial.print("start: ");
  Serial.println(current_start);

  if (current_start == 0) {
    drawMain();
  } else if (current_start == 1) {
    drawDrive(current_fb, current_lr, current_pwm);
  }
}
