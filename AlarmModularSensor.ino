/*
 08Sep2025 : initial code entry
 V1 will be to get an ESP32 into deep sleep and to wake up by
    externally triggering an AM312 PIR
    Parts...
    ESP32 
    AM312 - GPIO 33
    HLK-LD2410C
    LEDs

    My Onedrive Deep Sleep notes
    https://onedrive.live.com/view.aspx?resid=AF5CDF486EAF148A%21717409&id=documents&wd=target%28ESP32.one%7C2B67EE99-FA3C-4FA7-834B-1E3FA7F8FABA%2FDeep%20Sleep%20test%7C3DC48E4D-534F-4792-AA5E-2A9F0C9356A5%2F%29&wdpartid={7609B815-4998-00F4-229D-B2E2BFCB85DF}{1}&wdsectionfileid=AF5CDF486EAF148A!717751

    My Joplin note
    https://joplin://x-callback-url/openNote?id=50c24b80035e46bd9e58b46a55180681

    
 */

 #define PIR_PIN GPIO_NUM_33

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  esp_sleep_enable_ext0_wakeup(PIR_PIN, 1); // Wake on HIGH
  Serial.println("Going to sleep...");
  delay(1000);
  esp_deep_sleep_start();
}

void loop() {
  // Not used in deep sleep
}