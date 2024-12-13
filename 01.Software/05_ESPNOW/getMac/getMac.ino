#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  Serial.print(WiFi.macAddress());//A8:42:E3:AE:D2:30
}

void loop() {

}
