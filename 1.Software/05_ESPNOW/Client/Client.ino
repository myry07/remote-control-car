#include <WiFi.h>
#include <esp_now.h>

typedef struct test_struct {
  int x;
  int y;
  int state;
} test_struct;

test_struct myData;

// 回调函数
void OnDataRecv(const uint8_t * mac, const uint8_t * incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.print(len);
  Serial.print(", x: ");
  Serial.print(myData.x);
  Serial.print(", y: ");
  Serial.print(myData.y);
  Serial.print(", state: ");
  Serial.println(myData.state);
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if(esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // 注册接收回调函数
  if (esp_now_register_recv_cb(OnDataRecv) != ESP_OK) {
    Serial.println("Error registering recv callback");
    return;
  }
}

void loop() {
  // 延迟一段时间以便处理接收到的数据
  delay(1000);

  // 打印接收到的数据
  Serial.print("Received x: ");
  Serial.print(myData.x);
  Serial.print(", y: ");
  Serial.print(myData.y);
  Serial.print(", state: ");
  Serial.println(myData.state);
}
