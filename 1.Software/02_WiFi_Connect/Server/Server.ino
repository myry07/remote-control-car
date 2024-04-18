#include <Wire.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

const char* ssid = "myry_ESP32";
const char* password = "wjq110422";

AsyncWebServer server(80);  //Create an asynchronous web server on port 80.

int getNum() {
  int num = 7;
  return num;
}

void setup() {
  Serial.begin(115200);
  Serial.print("Setting AP");
  WiFi.softAP(ssid, password);  //Set your ESP32 as an access point with the SSID name and password defined earlier.

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/num", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", String(getNum()).c_str());
  });

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}
