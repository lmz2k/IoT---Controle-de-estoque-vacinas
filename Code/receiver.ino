#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

typedef struct struct_message {
  float temp;
  String id;
} struct_message;


struct_message myData;

int ledPin = 2;

// constexpr char WIFI_SSID[] = "iPhone";
// constexpr char WIFI_PASS[] = "12345678";

void initWiFi() {

    WiFi.mode(WIFI_MODE_APSTA);
    // WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Serial.printf("Connecting to %s .", WIFI_SSID);
    // while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(200); }
    // Serial.println(" ok");

    // IPAddress ip = WiFi.localIP();

    // Serial.printf("SSID: %s\n", WIFI_SSID);
    // Serial.printf("Channel: %u\n", WiFi.channel());
    // Serial.printf("IP: %u.%u.%u.%u\n", ip & 0xff, (ip >> 8) & 0xff, (ip >> 16) & 0xff, ip >> 24);
}

void onReceive(const uint8_t *mac_addr, const uint8_t *data, int len) {
    memcpy(&myData, data, sizeof(struct_message));
    Serial.printf("[%s] - Temperatura: %f\n", myData.id, myData.temp);
}

void initEspNow() {

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP NOW failed to initialize");
        while (1);
    }

    esp_now_register_recv_cb(onReceive);
}

void ledBlink(int interval) {
  pinMode(ledPin, OUTPUT); // Define o pino do LED como saída
  
  while (true) {
    digitalWrite(ledPin, HIGH); // Liga o LED
    delay(interval); // Aguarda o intervalo
    digitalWrite(ledPin, LOW); // Desliga o LED
    delay(interval); // Aguarda o intervalo novamente
  }
}

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println(WiFi.macAddress());
    initWiFi();
    initEspNow();
    ledBlink(500);
}

void loop() {}