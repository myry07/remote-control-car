#include "esp_camera.h"
#include <WiFi.h>
#include <ArduinoWebsockets.h>

// FreeRTOS 相关
TaskHandle_t cameraTaskHandle = NULL;
TaskHandle_t wifiSendTaskHandle = NULL;
SemaphoreHandle_t xSemaphore = NULL;
camera_fb_t *fb = NULL;

// WiFi 配置信息
const char* ssid = "myry_ESP32";
const char* password = "wjq110422";
const char* websockets_server_host = "192.168.4.1";
const uint16_t websockets_server_port = 8888;

using namespace websockets;
WebsocketsClient client;

// 摄像头配置
#define CAMERA_MODEL_AI_THINKER
// #define CAMERA_MODEL_ESP32S3_EYE
#include "camera_pins.h"

void setupCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    // 如果有PSRAM，使用较低的分辨率和较高的JPEG质量
    if (psramFound()) {
        config.frame_size = FRAMESIZE_QVGA; // 320x240
        config.jpeg_quality = 12;           // JPEG质量数值越高，质量越低
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    // 摄像头初始化
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }
}

// 摄像头任务，负责采集图像
void cameraTask(void *pvParameters) {
    while (true) {
        fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Camera capture failed");
            continue;
        }

        if (xSemaphore != NULL) {
            // 等待互斥信号量
            if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
                // 摄像头帧缓冲区可用
                xSemaphoreGive(xSemaphore); // 释放信号量
            }
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);  // 控制帧率
    }
}

// Wi-Fi发送任务，负责发送图像
void wifiSendTask(void *pvParameters) {
    while (true) {
        if (fb) {
            if (xSemaphore != NULL) {
                // 等待互斥信号量
                if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
                    // 发送图像数据
                    if (fb->format == PIXFORMAT_JPEG) {
                        client.sendBinary((const char *)fb->buf, fb->len);
                    }
                    esp_camera_fb_return(fb);
                    fb = NULL; // 释放帧缓冲区
                    xSemaphoreGive(xSemaphore); // 释放信号量
                }
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);  // 控制发送速率
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println();

    // 初始化摄像头
    setupCamera();

    // 初始化Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");

    // WebSocket客户端连接
    while (!client.connect(websockets_server_host, websockets_server_port, "/")) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Socket Connected!");

    // 创建信号量
    xSemaphore = xSemaphoreCreateMutex();

    // 创建摄像头采集任务
    xTaskCreatePinnedToCore(
        cameraTask,    // 任务函数
        "Camera Task", // 任务名称
        8192,          // 堆栈大小
        NULL,          // 任务输入参数
        1,             // 优先级
        &cameraTaskHandle, // 任务句柄
        1              // 运行在核心1上
    );

    // 创建Wi-Fi发送任务
    xTaskCreatePinnedToCore(
        wifiSendTask,     // 任务函数
        "WiFi Send Task", // 任务名称
        8192,             // 堆栈大小
        NULL,             // 任务输入参数
        1,                // 优先级
        &wifiSendTaskHandle, // 任务句柄
        1                 // 运行在核心1上
    );
}

void loop() {
    // 空的loop，因为所有任务都在FreeRTOS的多任务中运行
}