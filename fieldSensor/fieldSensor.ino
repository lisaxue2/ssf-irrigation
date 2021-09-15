/* 
 * This sketch is to be uploaded to Field ESPs interfacing with capacitative soil moisture sensors.
 *  
 * Routes data packets from field sensors to the Terminal ESP using many-to-one delivery scheme.
 * 
 * Each Field ESP sends data to the Terminal via ESP-NOW.
 * Terminal receives data and stores in an array.
 * 
 * Forms a star network.
 * 
 * Must get: MAC address of Terminal 
 */
#include <ESP8266WiFi.h>
#include <espnow.h>

// Create variable to be measured by sensor
float analogMoisture;
float voltage = (analogMoisture * 3.08) / 1024;

// MAC address of Terminal
uint8_t terminalAddress[] = {0x8C, 0xAA, 0xB5, 0x7C, 0x50, 0x9C};

// Set ESP Board ID
#define BOARD_ID 0

// Create a data structure to send data
// Matches the receiver struct
typedef struct message {
  int id;
  float moisture;
} message;

// Create message that stores variables to be sent
message soilData;

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

// CallBack gets executed when a message is sent by Field ESP
// Prints whether message has been successfully sent to Terminal
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
  
  // Set device as a WiFi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW role
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  
  // Register for Send CallBack to get the status of the transmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer as Terminal ESP
  esp_now_add_peer(terminalAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

}

void loop() {
  // Field ESP sends message every 10 seconds
  if ((millis() - lastTime) > timerDelay) {
    // Take sensor reading
    analogMoisture = analogRead(A0);
    // Set values to send
    soilData.id = BOARD_ID;
    soilData.moisture = voltage;
      
    // Send the message via ESP-NOW
    esp_now_send(terminalAddress, (uint8_t *) &soilData, sizeof(soilData));

    lastTime = millis();
}
