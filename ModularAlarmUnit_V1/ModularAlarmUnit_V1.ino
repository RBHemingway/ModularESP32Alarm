#include <esp_now.h>
#include <WiFi.h>

#define PIR_PIN GPIO_NUM_33
#define PRESENCE_PIN GPIO_NUM_32
uint8_t masterAddress[] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC}; // Replace with master MAC

typedef struct struct_message {
  bool presenceDetected;
} struct_message;

struct_message myData;

void sendESPNowMessage() {
  myData.presenceDetected = true;
  esp_now_send(masterAddress, (uint8_t *)&myData, sizeof(myData));
}

void setup() {
  Serial.begin(115200);
  pinMode(PRESENCE_PIN, INPUT);
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_add_peer(masterAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    bool presence = digitalRead(PRESENCE_PIN);
    if (presence) {
      sendESPNowMessage();
    }
  }

  delay(100); // Optional buffer
  esp_deep_sleep_start();
}

void loop() {}