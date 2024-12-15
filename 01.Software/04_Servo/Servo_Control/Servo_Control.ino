#include <ESP32Servo.h>

Servo servo1;  //定义舵机对象
Servo servo2;

int minUs = 500;   //0度时脉宽 单位us
int maxUs = 2500;  //180度

int servo1Pin = 15;
int servo2Pin = 2;

int pos = -1;
bool up = true;

void setup() {
  ESP32PWM::allocateTimer(1);  //指定使用的硬件定时器
  servo1.setPeriodHertz(50);   //PWM频率
  servo2.setPeriodHertz(50);

  servo1.attach(servo1Pin, minUs, maxUs);
  servo2.attach(servo2Pin, minUs, maxUs);

  servo1.write(pos);  //转到指定的角度 0-180

  //servo1.detach();  //不用的时候与引脚分离
}

void loop() {
  if (pos == 181) {
    up = false;
  } else if (pos == -1) {
    up = true;
  }

  if (up) {
    pos++;
  } else {
    pos--;
  }

  servo1.write(pos);
  servo2.write(180 - pos);

  delay(15);
}
