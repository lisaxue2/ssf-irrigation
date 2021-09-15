/* 
 * This sketch is to be uploaded to Relay2 ESP.
 * 
 * Routes data packet from Relay2 to Relay3 using one-to-one delivery scheme.
 * 
 * Relay2 receives data from Relay1 and stores in an array.
 * Relay2 sends data to a Relay3 in the form of an array.
 * 
 * 
 * Must get: MAC address of Relay3
 * 
 */
#include <ESP8266WiFi.h>
#include <espnow.h>

// No. of sensors
const int NUM = 6;

// MAC Address of Relay3
uint8_t relay3Address[] = {0xC4, 0x5B, 0xBE, 0x70, 0x68, 0x4B};

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

// Create a data structure to receive data & send data
// Matches the sender struct & receiver struct
typedef struct message {
  int id;
  float moisture;
} message;

// Create message that stores variables to be received
message sensorData[NUM];

// Callback gets executed when a message is received by Relay2 
// Prints MAC address of sender, message size, and each sensor's variables in array
void OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len) {
  
  // Get MAC address of the sender
  char relay1Address[18];
  Serial.print("Packet received from: ");
  snprintf(relay1Address, sizeof(relay1Address), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(relay1Address);
  
  // Copy received array into a new array called sensorData
  memcpy(&sensorData, incomingData, sizeof(sensorData));
  Serial.printf("Relay1: %u bytes sent\n", len);

  // Print array
  for(int i = 0; i < NUM; i++) {
    Serial.printf("Sensor ID %u: %f V\n", sensorData[i].id, sensorData[i].moisture);
  }
  
}

// Callback gets executed when data is sent by Relay2
// Prints whether message array has been successfully sent to Relay3
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
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
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  
  // Register for Receive CallBack to get the contents of the received packet
  esp_now_register_recv_cb(OnDataRecv);

  // Register for Send CallBack to get the status of the transmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer as Relay3 ESP
  esp_now_add_peer(relay3Address, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

}

void loop() {
  // Relay2 sends data every 10 seconds
  if ((millis() - lastTime) > timerDelay) {
    // Send the message via ESP-NOW
    esp_now_send(relay3Address, (uint8_t *) &sensorData, sizeof(sensorData));

    lastTime = millis();
  }
}
