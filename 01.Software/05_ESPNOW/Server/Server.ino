#include <WiFi.h>
#include <esp_now.h>

//Serial.print(WiFi.macAddress());//A8:42:E3:AE:D2:30
uint8_t broadcastAddress[] = { 0xA8, 0x42, 0xE3, 0xAE, 0xD2, 0x30 }; 

#define pinXl 32  // 遥感x轴
#define pinYl 33  // 遥感y轴
#define pinXr 34
#define pinYr 35

int valxl = 0;
int valyl = 0;
int valxr = 0;
int valyr = 0;

// 定义结构体 发送 xy 信息
typedef struct Car {
  int x;
  int y;
  int state;
} Car;

Car c = { 0, 0 };  // 在此初始化结构体成员变量

esp_now_peer_info_t peerInfo;

// 定义一个回调函数 发送信息时检测有没有发送成功
void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Error");
    return;
  }
}


void loop() {
  c.x = analogRead(pinXl);
  c.y = analogRead(pinYl);
  c.state = analogRead(pinXr);

   // 将数据封装到缓冲区
  uint8_t *buffer = (uint8_t *) &c;
  esp_err_t result = esp_now_send(broadcastAddress, buffer, sizeof(c));
  
  // 检查发送状态
  if (result == ESP_OK) {
    Serial.println("Data sent successfully");
  } else {
    Serial.println("Error sending data");
  }

  delay(100); // 可选的延迟
}