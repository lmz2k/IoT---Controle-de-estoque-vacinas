#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

typedef struct struct_message {
  float temp;
  String id;
} struct_message;

constexpr uint8_t ESP_NOW_RECEIVER[] = { 0xc8, 0xf0, 0x9e, 0xf7, 0x6d, 0x14 };
esp_now_peer_info_t peerInfo;
uint32_t last;

struct_message data;

void initWiFi() {
    WiFi.mode(WIFI_MODE_STA);
}

void initEspNow() {
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP NOW failed to initialize");
        while (1);
    }

    memcpy(peerInfo.peer_addr, ESP_NOW_RECEIVER, 6);
    peerInfo.ifidx   = WIFI_IF_STA;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("ESP NOW pairing failure");
        while (1);
    }
}

void setup() {
    Serial.begin(115200);

    initWiFi();
    initEspNow();

    data.id = "Container 1";
}

void generateRandomData() {
  int randomNumber = random(1024);
  float randomTemp = (float)randomNumber / 10.0;
  data.temp = randomTemp;
}

void loop() {

    if (millis() - last > 2000) {
        generateRandomData();
        esp_now_send(ESP_NOW_RECEIVER, (uint8_t *) &data, sizeof(data));
        Serial.printf("Temp: %f\n", data.temp);
        Serial.printf("Id: %s\n", data.id);
        Serial.printf("Sent to channel: %u\n", WiFi.channel());
        last = millis();
    }
}