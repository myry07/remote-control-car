#include <TFT_eSPI.h>
#include <SPI.h>
#include "main.h"
#include "car.h"
#include "me.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

//遥感
#define pinXl 32  // 遥感x轴
#define pinYl 33  // 遥感y轴
#define pinXr 34
#define pinYr 35


//主菜单
#define LENGTH 60  //主菜单 长度
#define SPACE 16   //主菜单 间隔
#define Y 90       //主菜单Y坐标

#define C_Y LENGTH + Y + 20  //圆 Y坐标
#define C_r 7                //圆半径
#define C_move 76            //小球位移


TFT_eSPI tft = TFT_eSPI();

int valxl = 0;  //遥感坐标
int valyl = 0;
int valxr = 0;
int valyr = 0;

int x = 1 * SPACE + 0 * LENGTH + LENGTH / 2;  //圆球坐标
//圆的四个X坐标
//1 * SPACE + 0 * LENGTH + LENGTH / 2
//2 * SPACE + 1 * LENGTH + LENGTH / 2
//3 * SPACE + 2 * LENGTH + LENGTH / 2
//4 * SPACE + 3 * LENGTH + LENGTH / 2

int main_state = 1;  //主菜单状态

int car_state = 0;  //二级车菜单状态
int me_state = 0;   //二级me菜单状态

unsigned long startTime;  // 用于记录循环开始的时间
unsigned long endTime;    // 用于记录循环结束的时间

void drawMainMenu() {  //主菜单
  tft.setCursor(120, 30);
  tft.setTextColor(TFT_GREEN);
  tft.setTextFont(4);
  tft.println("Menu");

  tft.pushImage(1 * SPACE + 0 * LENGTH, Y, LENGTH, LENGTH, car);
  tft.pushImage(2 * SPACE + 1 * LENGTH, Y, LENGTH, LENGTH, plane);
  tft.pushImage(3 * SPACE + 2 * LENGTH, Y, LENGTH, LENGTH, balance);
  tft.pushImage(4 * SPACE + 3 * LENGTH, Y, LENGTH, LENGTH, man);

  if (valyl > 3000 && x <= 4 * SPACE + 3 * LENGTH + LENGTH / 2) {  //小球左移
    x += C_move;
    delay(200);
  }

  if (valyl < 200 && x >= 1 * SPACE + 0 * LENGTH + LENGTH / 2) {  //小球右移
    x -= C_move;
    delay(200);
  }

  //绘制小球
  tft.fillCircle(x, C_Y, 7, TFT_GREEN);

  //文字位置
  tft.setCursor(100, 195);
  tft.setTextFont(4);

  //小车命令 x = 46
  if (x == 1 * SPACE + 0 * LENGTH + LENGTH / 2) {
    tft.setTextColor(TFT_RED);
    tft.println("Ctrl Car");

    if (valxr > 3000) {  //开启车菜单
      tft.fillScreen(TFT_BLACK);
      main_state = 0;
      car_state = 1;
      delay(200);
    }
  }

  //小车Touch
  if (touch(1 * SPACE + 0 * LENGTH, Y)) {
    tft.fillScreen(TFT_BLACK);
    main_state = 0;
    car_state = 1;
    delay(200);
  }

  //飞机命令 x = 122
  if (x == 2 * SPACE + 1 * LENGTH + LENGTH / 2) {
    tft.setTextColor(TFT_BLUE);
    tft.println("Ctrl Plane");
  }

  //平衡车命令 x = 274
  if (x == 3 * SPACE + 2 * LENGTH + LENGTH / 2) {
    tft.setTextColor(TFT_PURPLE);
    tft.println("Ctrl Balance");
  }

  //我命令 x = 350
  if (x == 4 * SPACE + 3 * LENGTH + LENGTH / 2) {
    tft.setTextColor(TFT_WHITE);
    tft.println("Get Author");

    if (valxr > 3000) {  //开启me菜单
      tft.fillScreen(TFT_BLACK);
      main_state = 0;
      me_state = 1;
      delay(200);
    }
  }

  //meTouch
  if (touch(4 * SPACE + 3 * LENGTH, Y)) {
    tft.fillScreen(TFT_BLACK);
    main_state = 0;
    me_state = 1;
    delay(200);
  }
}


void drawCarMenu() {  //二级车菜单
  tft.setCursor(120, 30);
  tft.setTextColor(TFT_GREEN);
  tft.setTextFont(4);
  tft.println("Car Menu");

  tft.pushImage(1 * SPACE + 0 * LENGTH, Y, LENGTH, LENGTH, wheel);
  tft.pushImage(2 * SPACE + 1 * LENGTH, Y, LENGTH, LENGTH, auto_drive);

  if (valyl > 3000 && x <= 4 * SPACE + 3 * LENGTH + LENGTH / 2) {  //小球左移
    x += C_move;
    delay(200);
  }

  if (valyl < 200 && x >= 1 * SPACE + 0 * LENGTH + LENGTH / 2) {  //小球右移
    x -= C_move;
    delay(200);
  }

  //绘制小球
  tft.fillCircle(x, C_Y, 7, TFT_RED);

  //文字位置
  tft.setCursor(100, 195);
  tft.setTextFont(4);

  //我驾驶
  if (x == 1 * SPACE + 0 * LENGTH + LENGTH / 2) {
    tft.setTextColor(TFT_RED);
    tft.println("I drive");
  }

  //自动驾驶
  if (x == 2 * SPACE + 1 * LENGTH + LENGTH / 2) {
    tft.setTextColor(TFT_RED);
    tft.println("self drive");
  }

  if (valxr < 200) {
    tft.fillScreen(TFT_BLACK);
    main_state = 1;
    car_state = 0;
    delay(200);
  }
}

void drawMeMenu(int valy, int x) {
  tft.setCursor(70, 20);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(4);
  tft.println("Author - MYRY");
  tft.pushImage(20, 80, 100, 100, me);

  tft.setTextFont(2);
  tft.setCursor(130, 110);
  tft.println("hello I'm Jiaqi, a handsome");
  tft.setCursor(130, 130);
  tft.println("man from East Asian - China");

  if (valxr < 200) {
    tft.fillScreen(TFT_BLACK);
    me_state = 0;
    main_state = 1;
    delay(200);
  }
}


void updateMenu(int valy, int x) {
  if (x >= 1 * SPACE + 0 * LENGTH + LENGTH / 2 && x <= 4 * SPACE + 3 * LENGTH + LENGTH / 2) {
    if (valy < 200 || valy > 3000) {
      tft.fillCircle(1 * SPACE + 0 * LENGTH + LENGTH / 2, C_Y, 7, TFT_BLACK);
      tft.fillCircle(2 * SPACE + 1 * LENGTH + LENGTH / 2, C_Y, 7, TFT_BLACK);
      tft.fillCircle(3 * SPACE + 2 * LENGTH + LENGTH / 2, C_Y, 7, TFT_BLACK);
      tft.fillCircle(4 * SPACE + 3 * LENGTH + LENGTH / 2, C_Y, 7, TFT_BLACK);
      tft.fillRect(100, 195, 150, 150, TFT_BLACK);
    }
  }
}

// 定义触摸检测函数
bool touch(int x, int y) {
  uint16_t touchX = 0, touchY = 0;
  bool pressed = tft.getTouch(&touchX, &touchY);
  if (pressed) {
    if (touchX >= x && touchX <= (x + LENGTH) && touchY >= y && touchY <= (y + LENGTH)) {
      return true;
    }
  }
  return false;
}


void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  //屏幕校准
  uint16_t calData[5] = { 372, 3412, 319, 3386, 1 };
  tft.setTouch(calData);
}

void loop() {
  startTime = millis();       // 记录循环开始的时间
  valxl = analogRead(pinXl);  // 读取x轴的值并转换为变量
  valyl = analogRead(pinYl);  // 读取y轴的值并转换为变量
  valxr = analogRead(pinXr);
  valyr = analogRead(pinYr);

  if (main_state == 1) {  //绘制主菜单
    drawMainMenu();
    updateMenu(valyl, x);
    Serial.print("x: ");
    Serial.println(x);
  }

  if (car_state == 1) {  //绘制二级菜单
    drawCarMenu();
    updateMenu(valyl, x);
  }

  if (me_state == 1) {  //绘制me二级菜单
    drawMeMenu(valyl, x);
  }


  endTime = millis();  // 记录循环结束的时间
  unsigned long loopTime = endTime - startTime;

  unsigned long delayTime = 1000 / 60;  // 假设希望循环以每秒 60 次的频率执行
  if (loopTime < delayTime) {
    delay(delayTime - loopTime);  // 计算需要延迟的时间并等待
  }
}
