#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>

const char* ssid = "myry_ESP32";
const char* password = "wjq110422";

const char* serverNum = "http://192.168.4.1/num";

String num;

// Set the time interval between each request.
// By default, it’s set to 5 seconds, but you can change it to any other interval.
unsigned long previousMillis = 0;
const long interval = 5000;

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

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //如果连接成功 那么2号引脚亮起
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // 检查 WiFi 连接状态
  if (WiFi.status() == WL_CONNECTED) {
    // 调用 httpGETRequest 函数并将结果存储在 num 变量中
    num = httpGETRequest(serverNum);
    // 打印获取到的数据
    Serial.println(num);
  }
}
