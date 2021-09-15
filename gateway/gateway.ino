/* 
 * This sketch is to be uploaded to Gateway ESP.
 * 
 * Routes data packet from Gateway to RPi using serial port.
 * 
 * Gateway receives data from Relay3 and stores in an array.
 * Gateway prints data to serial port in the form of an array.
 * 
 * 
 * 
 */
#include <ESP8266WiFi.h>
#include <espnow.h>

const int NUM = 6;

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

// Create a data structure to receive data
// Matches the sender struct
typedef struct message {
  int id;
  float moisture;
} message;

// Create message that stores variables to be received
message sensorData[NUM];


// Callback gets executed when a message is received by Gateway 
// Prints each sensor's variables in array
void OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len) {
  // Copy received array into a new array called sensorData
  memcpy(&sensorData, incomingData, sizeof(sensorData));

  // Print array
  for(int i = 0; i < NUM; i++) {
    Serial.printf("ID %u: %f ", sensorData[i].id, sensorData[i].moisture);
    Serial.println();
  }
  
}


void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW role
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  
  // Register for Receive CallBack to get the contents of the received packet
  esp_now_register_recv_cb(OnDataRecv);

}

void loop() {

}
