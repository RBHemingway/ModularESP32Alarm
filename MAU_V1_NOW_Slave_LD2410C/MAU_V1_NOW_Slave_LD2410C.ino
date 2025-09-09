/*
NEXT VERSION ...

THIS VERSION ...
  V1 10Sep25 adding NOW to MASTER ESP32, NO Deepsleep (This is the slave or receiver code, 'The Master ESP32')
  https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/

  On the receiver side, the sketch should include:
  1. Initialize ESP-NOW;
  2. Register for a receive callback function (OnDataRecv). This is a function that will be executed when a message is received.
  3. Inside that callback function, save the message into a variable to execute any task with that information.
  
  Hardware Connections
  ======================
  An led is connected to GPIO 14 to indicate receiving an alarm signal

  NOTE:
  ======
  For the development, this ESP32's MAC is 58 BF 25 93 E6 F0
*/
#include <esp_now.h>
#include <WiFi.h>

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

#define ALARMON 10  // the int from sensor unit that an alarm has triggered
// Structure example to receive data // Must match the sender structure
typedef struct struct_message {
    int codeSent;  // what message is being sent
    int fromESP;  // index of sensor unit 
    int bootnumber; // number of the sending units messgae
} struct_message;

const int ledAlarm = 14;
// Create a struct_message called myData to receive the NOW message
struct_message myData;
bool newAlarmReceived = false;

void setup() {
  Serial.begin(115200);
  delay(1000);  //Take some time to open up the Serial Monitor
  pinMode(ledAlarm, OUTPUT);
  digitalWrite(ledAlarm, LOW);

  // NOW code; Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Once ESPNow is successfully Init, we will register for recv CB to get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

}

void loop() {
  if (newAlarmReceived) {
    digitalWrite(ledAlarm, HIGH);
    delay(5000);
    newAlarmReceived = false;
    digitalWrite(ledAlarm, LOW);
  }
}
// ---------------------------------------------
// ---------------------------------------------
void print_wakeup_reason() {
  // Method to print the reason by which ESP32 has been awaken from sleep
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:     Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1:     Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER:    Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP:      Serial.println("Wakeup caused by ULP program"); break;
    default:                        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

void sendAlarmToMaster() {
  // 4now simply put an LED on for 5s
  digitalWrite(ledAlarm, HIGH);
  delay(15000);
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: "); Serial.println(len);
  Serial.print("Unit sending is "); Serial.println(myData.fromESP);
  Serial.print("Message sent is "); Serial.println(myData.codeSent);
  Serial.print("Boot number  is "); Serial.println(myData.bootnumber);
  Serial.println();
  if (myData.codeSent==ALARMON) newAlarmReceived = true;
}
// ==============================================
