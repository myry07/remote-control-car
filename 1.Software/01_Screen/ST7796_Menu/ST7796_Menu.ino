#include <TFT_eSPI.h>
#include <SPI.h>
#include "main_menu.h"
#include "game_menu.h"
#include "game1_img.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

#define pinXl 32  // 遥感x轴
#define pinYl 33  // 遥感y轴
#define pinXr 34
#define pinYr 35

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

int valxl = 0;
int valyl = 0;
int valxr = 0;
int valyr = 0;

unsigned long startTime;  // 用于记录循环开始的时间
unsigned long endTime;    // 用于记录循环结束的时间

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

//天天酷跑菜单
typedef struct Game1 {
  int x;
  int y;
  bool state;
} Game1;

Menu main_menu = { 18, 90, true };  // 定义全局的 Menu 结构体变量
Game game_menu = { 18, 90, false };
Car car_menu = { 18, 90, false };

Game1 game1 = {0, 0, false};

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

  if(main_menu.x == 250) {
    tft.setTextColor(TFT_RED);
    tft.print("3. Ctrl Balance");
  }

  if (main_menu.x == 366) {
    tft.setTextColor(TFT_WHITE);
    tft.print("4. Get Author");
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

  tft.pushImage(0, 0, 90, 90, game);  // 使用正确的变量名 game.x
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

void drawGame1() {
  valxl = analogRead(pinXl);  // 读取x轴的值并转换为变量
  valyl = analogRead(pinYl);  // 读取y轴的值并转换为变量
  valxr = analogRead(pinXr);
  valyr = analogRead(pinYr);

  tft.pushImage(0, 0, 48, 32, bg);

  if (valxr < 200) {
    tft.fillScreen(TFT_BLACK);
    game1.state = false;
    game_menu.state = true;
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

  tft.pushImage(0, 0, 90, 90, car);  // 使用正确的变量名 game.x

  //退出小车菜单
  if (valxr < 200) {
    tft.fillScreen(TFT_BLACK);
    car_menu.state = false;
    main_menu.state = true;
  }
}

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT); // 在内存中创建一个 240x240 的虚拟画布
  sprite.fillScreen(TFT_BLACK); // 在虚拟画布上填充黑色背景
  sprite.pushSprite(0, 0); // 将整个虚拟画布一次性地绘制到实际的 TFT 显示屏上
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

  if (car_menu.state) {
    drawCarMenu();
  }

  if(game1.state) {
    drawGame1();
  }

  endTime = millis();  // 记录循环结束的时间
  unsigned long loopTime = endTime - startTime;

  unsigned long delayTime = 1000 / 60;  // 假设希望循环以每秒 60 次的频率执行
  if (loopTime < delayTime) {
    delay(delayTime - loopTime);  // 计算需要延迟的时间并等待
  }
}