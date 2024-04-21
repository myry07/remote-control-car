#include <TFT_eSPI.h>
#include <SPI.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <TJpg_Decoder.h>
#include <Wire.h>
#include "ESPAsyncWebServer.h"

#include "main_menu.h"
#include "game_menu.h"
#include "car_menu.h"
#include "game1_img.h"
#include "me.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

#define pinXl 32  // 遥感x轴
#define pinYl 33  // 遥感y轴
#define pinXr 34
#define pinYr 35

const char* ssid = "myry_ESP32";
const char* password = "wjq110422";

using namespace websockets;
WebsocketsServer server;
WebsocketsClient client;

AsyncWebServer asyncServer(80);  //Create an asynchronous web server on port 80.

unsigned long startTime;  // 用于记录循环开始的时间
unsigned long endTime;    // 用于记录循环结束的时间

TFT_eSPI tft = TFT_eSPI();

int valxl = 0;
int valyl = 0;
int valxr = 0;
int valyr = 0;

//主菜单
typedef struct Menu {
  int x;
  int y;
  bool state;
} Menu;

//游戏菜单
typedef struct Game {
  int x;
  int y;
  bool state;
} Game;

//小车菜单
typedef struct Car {
  int x;
  int y;
  bool state;
} Car;

typedef struct DriveCar {
  int x;
  int y;
  bool state;
  int drive;
} DriveCar;


//天天酷跑菜单
typedef struct Game1 {
  int x;
  int y;
  bool state;
} Game1;

typedef struct Me {
  int x;
  int y;
  bool state;
} Me;

Menu main_menu = { 18, 90, true };  // 定义全局的 Menu 结构体变量
Game game_menu = { 18, 90, false };
Car car_menu = { 18, 90, false };
DriveCar dc = { 0, 0, false, 0 };
Game1 game1 = { 0, 0, false };
Me m = { 0, 100, false };

void drawMenu() {
  valxl = analogRead(pinXl);  // 读取x轴的值并转换为变量
  valyl = analogRead(pinYl);  // 读取y轴的值并转换为变量
  valxr = analogRead(pinXr);
  valyr = analogRead(pinYr);

  tft.setCursor(200, 30);
  tft.setTextColor(TFT_GREEN);
  tft.setTextFont(4);
  tft.println("Menu");

  tft.pushImage(24, 95, 90, 90, game);
  tft.pushImage(138, 95, 90, 90, car);
  tft.pushImage(253, 95, 90, 90, balance);
  tft.pushImage(368, 95, 90, 90, man);

  tft.drawRect(main_menu.x, main_menu.y, 100, 100, TFT_GREEN);  // 使用正确的变量名 menu.x

  if (valyl > 3000 && main_menu.x < 466) {  // 使用正确的变量名 menu.x
    main_menu.x += 116;
    delay(200);
  }

  if (valyl < 200 && main_menu.x > 18) {  // 使用正确的变量名 menu.x
    main_menu.x -= 116;
    delay(200);
  }

  tft.setCursor(170, 250);
  tft.setTextFont(4);

  if (main_menu.x == 18) {
    tft.setTextColor(TFT_BLUE);
    tft.print("1. Play Games");

    //打开游戏菜单
    if (valxr > 3000) {
      tft.fillScreen(TFT_BLACK);
      main_menu.state = false;
      game_menu.state = true;
      delay(200);
    }
  }

  if (main_menu.x == 134) {
    tft.setTextColor(TFT_RED);
    tft.print("2. Ctrl Car");

    //打开小车菜单
    if (valxr > 3000) {
      tft.fillScreen(TFT_BLACK);
      main_menu.state = false;
      car_menu.state = true;
      delay(200);
    }
  }

  if (main_menu.x == 250) {
    tft.setTextColor(TFT_PURPLE);
    tft.print("3. Ctrl Balance");
  }

  if (main_menu.x == 366) {
    tft.setTextColor(TFT_WHITE);
    tft.print("4. Get Author");

    if (valxr > 3000) {
      tft.fillScreen(TFT_BLACK);
      main_menu.state = false;
      m.state = true;
      delay(200);
    }
  }
}

void updateMenu(int valy, int x) {
  if (x >= 18 && x <= 366) {
    if (valy < 200 || valy > 3000) {
      tft.drawRect(18, 90, 100, 100, TFT_BLACK);
      tft.drawRect(134, 90, 100, 100, TFT_BLACK);
      tft.drawRect(250, 90, 100, 100, TFT_BLACK);
      tft.drawRect(366, 90, 100, 100, TFT_BLACK);
      tft.fillRect(170, 250, 200, 100, TFT_BLACK);
    }
  }
}

void drawGameMenu() {
  valxl = analogRead(pinXl);  // 读取x轴的值并转换为变量
  valyl = analogRead(pinYl);  // 读取y轴的值并转换为变量
  valxr = analogRead(pinXr);
  valyr = analogRead(pinYr);

  tft.setCursor(200, 30);
  tft.setTextColor(TFT_BLUE);
  tft.setTextFont(4);
  tft.println("Game Menu");

  tft.pushImage(23, 95, 90, 90, ttkp);
  tft.pushImage(138, 95, 90, 90, plane);

  tft.drawRect(game_menu.x, game_menu.y, 100, 100, TFT_BLUE);

  if (valyl > 3000 && game_menu.x < 134) {
    game_menu.x += 116;
    delay(200);
  }

  if (valyl < 200 && game_menu.x > 18) {
    game_menu.x -= 116;
    delay(200);
  }

  tft.setCursor(170, 250);
  if (game_menu.x == 18) {
    tft.print("1. Parkour");

    if (valxr > 3000) {
      tft.fillScreen(TFT_BLACK);
      game_menu.state = false;
      game1.state = true;
      delay(200);
    }
  }

  if (game_menu.x == 134) {
    tft.print("2. Plane");
  }

  //退出游戏菜单
  if (valxr < 200) {
    tft.fillScreen(TFT_BLACK);
    game_menu.state = false;
    main_menu.state = true;
    delay(200);
  }
}

void drawCarMenu() {
  valxl = analogRead(pinXl);  // 读取x轴的值并转换为变量
  valyl = analogRead(pinYl);  // 读取y轴的值并转换为变量
  valxr = analogRead(pinXr);
  valyr = analogRead(pinYr);

  tft.setCursor(200, 30);
  tft.setTextColor(TFT_RED);
  tft.setTextFont(4);
  tft.println("Car Menu");

  tft.pushImage(23, 95, 90, 90, wheel);
  tft.pushImage(138, 95, 90, 90, dragon);

  tft.drawRect(car_menu.x, car_menu.y, 100, 100, TFT_RED);

  if (valyl > 3000 && car_menu.x < 134) {
    car_menu.x += 116;
    delay(200);
  }

  if (valyl < 200 && car_menu.x > 18) {
    car_menu.x -= 116;
    delay(200);
  }

  tft.setCursor(170, 250);
  if (car_menu.x == 18) {
    tft.print("1. Drive Car");

    if (valxr > 3000) {
      tft.fillScreen(TFT_BLACK);
      car_menu.state = false;
      dc.drive = 1;
      dc.state = true;
      delay(200);
    }
  }

  if (car_menu.x == 134) {
    tft.print("2. Play Game");
  }

  //退出游戏菜单
  if (valxr < 200) {
    tft.fillScreen(TFT_BLACK);
    car_menu.state = false;
    main_menu.state = true;
    delay(200);
  }
}

void drawMeMenu() {
  valxl = analogRead(pinXl);  // 读取x轴的值并转换为变量
  valyl = analogRead(pinYl);  // 读取y轴的值并转换为变量
  valxr = analogRead(pinXr);
  valyr = analogRead(pinYr);

  tft.setCursor(100, 30);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(4);
  tft.println("Author - MYRY");
  tft.pushImage(100, 200, 90, 90, me2);


  tft.setCursor(m.x, m.y);
  tft.println("hello I'm Jiaqi, a handsome");
  tft.println("man from East Asian - China");

  if (valxr < 200) {
    tft.fillScreen(TFT_BLACK);
    m.state = false;
    main_menu.state = true;
    delay(200);
  }
}

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  // Stop further decoding as image is running off bottom of screen
  if (y >= tft.height()) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // This might work instead if you adapt the sketch to use the Adafruit_GFX library
  // tft.drawRGBBitmap(x, y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;
}

void getCamera() {
  if (server.poll()) {
    client = server.accept();
  }

  if (client.available()) {
    client.poll();

    WebsocketsMessage msg = client.readBlocking();

    uint32_t t = millis();

    // Get the width and height in pixels of the jpeg if you wish
    uint16_t w = 0, h = 0;
    TJpgDec.getJpgSize(&w, &h, (const uint8_t*)msg.c_str(), msg.length());
    Serial.print("Width = ");
    Serial.print(w);
    Serial.print(", height = ");
    Serial.println(h);

    // Calculate offsets to center the image
    int16_t xOffset = (480 - w) / 2;
    int16_t yOffset = (320 - h) / 2;

    // Draw the image centered on the screen
    TJpgDec.drawJpg(xOffset, yOffset, (const uint8_t*)msg.c_str(), msg.length());

    // How much time did rendering take (ESP8266 80MHz 271ms, 160MHz 157ms, ESP32 SPI 120ms, 8bit parallel 105ms
    t = millis() - t;
    Serial.print(t);
    Serial.println(" ms");
  }
}

int getXl() {
  return valxl;
}

int getYl() {
  return valyl;
}

int getState() {
  return dc.drive;
}

void drawDriveCar() {
  valxl = analogRead(pinXl);  // 读取x轴的值并转换为变量
  valyl = analogRead(pinYl);  // 读取y轴的值并转换为变量
  valxr = analogRead(pinXr);
  valyr = analogRead(pinYr);

  getCamera();

  if (valxl > 3000) {
    tft.fillCircle(236, 20, 8, TFT_PURPLE);
    tft.fillCircle(236, 300, 8, TFT_BLACK);
    tft.fillCircle(20, 156, 8, TFT_BLACK);
    tft.fillCircle(460, 156, 8, TFT_BLACK);
  }

  if (valxl < 200) {
    tft.fillCircle(236, 20, 8, TFT_BLACK);
    tft.fillCircle(236, 300, 8, TFT_PURPLE);
    tft.fillCircle(20, 156, 8, TFT_BLACK);
    tft.fillCircle(460, 156, 8, TFT_BLACK);
  }

  if (valyl > 3000) {
    tft.fillCircle(236, 20, 8, TFT_BLACK);
    tft.fillCircle(236, 300, 8, TFT_BLACK);
    tft.fillCircle(20, 156, 8, TFT_BLACK);
    tft.fillCircle(460, 156, 8, TFT_PURPLE);
  }

  if (valyl < 200) {
    tft.fillCircle(236, 20, 8, TFT_BLACK);
    tft.fillCircle(236, 300, 8, TFT_BLACK);
    tft.fillCircle(20, 156, 8, TFT_PURPLE);
    tft.fillCircle(460, 156, 8, TFT_BLACK);
  }

  if (valxl > 1500 && valxl < 2000 && valyl > 1500 && valyl < 2000) {
    tft.fillCircle(236, 20, 8, TFT_BLACK);
    tft.fillCircle(236, 300, 8, TFT_BLACK);
    tft.fillCircle(20, 156, 8, TFT_BLACK);
    tft.fillCircle(460, 156, 8, TFT_BLACK);
  }

  if (valxr < 200) {
    tft.fillScreen(TFT_BLACK);
    dc.drive = 0;
    dc.state = false;
    car_menu.state = true;
    delay(200);
  }
}

void drawGame1() {
  valxl = analogRead(pinXl);  // 读取x轴的值并转换为变量
  valyl = analogRead(pinYl);  // 读取y轴的值并转换为变量
  valxr = analogRead(pinXr);
  valyr = analogRead(pinYr);

  if (valxr < 200) {
    tft.fillScreen(TFT_BLACK);
    game1.state = false;
    game_menu.state = true;
    delay(200);
  }
}

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);

  Serial.println();
  Serial.println("Setting AP...");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP Address : ");
  Serial.println(IP);

  server.listen(8888);

  asyncServer.on("/x_left", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", String(getXl()).c_str());
  });

  asyncServer.on("/y_left", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", String(getYl()).c_str());
  });

  asyncServer.on("/state", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", String(getState()).c_str());
  });


  asyncServer.begin();
}

void loop() {
  startTime = millis();  // 记录循环开始的时间

  if (main_menu.state) {
    drawMenu();
    updateMenu(valyl, main_menu.x);  // 使用正确的变量名 menu.x
  }

  if (game_menu.state) {
    drawGameMenu();
    updateMenu(valyl, game_menu.x);
  }

  if (game1.state) {
    drawGame1();
  }

  if (car_menu.state) {
    drawCarMenu();
    updateMenu(valyl, car_menu.x);
  }

  if (dc.state) {
    drawDriveCar();
  }

  if (m.state) {
    drawMeMenu();
  }

  endTime = millis();  // 记录循环结束的时间
  unsigned long loopTime = endTime - startTime;

  unsigned long delayTime = 1000 / 60;  // 假设希望循环以每秒 60 次的频率执行
  if (loopTime < delayTime) {
    delay(delayTime - loopTime);  // 计算需要延迟的时间并等待
  }
}