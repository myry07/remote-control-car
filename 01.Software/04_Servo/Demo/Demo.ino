#include <ESP32Servo.h>

Servo servo_x;  //定义舵机对象
Servo servo_y;

int minUs = 500;   //0度时脉宽 单位us
int maxUs = 2500;  //180度

int servo_x_Pin = 2;
int servo_y_Pin = 14;

int pos = 30;

void setup() {
  Serial.begin(115200);

  ESP32PWM::allocateTimer(1);  //指定使用的硬件定时器
  servo_x.setPeriodHertz(50);   //PWM频率
  servo_y.setPeriodHertz(50);

  servo_x.attach(servo_x_Pin, minUs, maxUs);
  servo_y.attach(servo_y_Pin, minUs, maxUs);

  servo_x.write(0);
  servo_y.write(0);

  //servo1.detach();  //不用的时候与引脚分离
}

void loop() {

  servo_x.write(pos);

  delay(10000);

  servo_y.write(pos);

  delay(10000);

  pos *= -1;
}
