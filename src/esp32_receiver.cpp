#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel LED_RGB(1, 48, NEO_GRBW + NEO_KHZ800);

#define ESPNOW_PACKET_SIZE 6

// ================= CONFIG =================
// Hub uses index 0
// Receivers use 1–5
static const uint8_t MY_PLAYER_INDEX = 1;

// ================= STATE =================
uint8_t incomingStates[ESPNOW_PACKET_SIZE];

// ================= FORWARD DECLARATIONS =================
void onDataReceived(const uint8_t *mac, const uint8_t *incomingData, int len);
void handleState(uint8_t state);



// ================= ESP-NOW CALLBACK =================
void onDataReceived(const uint8_t *mac, const uint8_t *incomingData, int len) {
  if (len != ESPNOW_PACKET_SIZE) {
    Serial.print("Unexpected packet size: ");
    Serial.println(len);
    return;
  }

  uint8_t myState = incomingData[MY_PLAYER_INDEX];
  handleState(myState);
}



// ================= STATE HANDLER =================
void handleState(uint8_t state) {
  switch (state) {
            case 0x00: LED_RGB.setPixelColor(0, LED_RGB.Color(0,0,0)); 
            Serial.println("OFF");
            break;
            case 0x01: LED_RGB.setPixelColor(0, LED_RGB.Color(0,255,0)); 
            Serial.println("GREEN");
            break;
            case 0x02: LED_RGB.setPixelColor(0, LED_RGB.Color(0,0,255)); 
            Serial.println("BLUE");
            break;
            case 0x03: LED_RGB.setPixelColor(0, LED_RGB.Color(255,0,0)); 
            Serial.println("RED");
            break;
            default:   LED_RGB.setPixelColor(0, LED_RGB.Color(0,0,0)); 
            Serial.println("DEFAULT");
            break;
  }
  LED_RGB.show();
}

// ================= ARDUINO ENTRY =================
void setup() {
  Serial.begin(115200);
  delay(500);

  LED_RGB.begin();
  LED_RGB.show();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  


// 🔑 FORCE CHANNEL
esp_wifi_set_promiscuous(true);
esp_wifi_set_channel(0, WIFI_SECOND_CHAN_NONE);
esp_wifi_set_promiscuous(false);


  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(onDataReceived);
  Serial.println("ESP-NOW receive callback registered");


  Serial.println("ESP32 Receiver Ready");\
  
  Serial.print("Receiver WiFi STA MAC: ");
  Serial.println(WiFi.macAddress());

}

void loop() {
  // Event-driven
}
