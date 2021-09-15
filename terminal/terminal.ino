/* 
 * This sketch is to be uploaded to the Terminal ESP.
 * 
 * Routes data packet from Terminal to Relay1 using one-to-one delivery scheme.
 * 
 * Terminal receives data from each Field ESP and stores in an array.
 * Terminal sends data to Relay1 in the form of an array.
 * 
 * 
 * Must get: MAC address of Relay1
 * 
 */
#include <ESP8266WiFi.h>
#include <espnow.h>

// Set no. of sensors
const int NUM = 6;

// MAC Address of Relay1
uint8_t relay1Address[] = {0xC4, 0x5B, 0xBE, 0x71, 0x2B, 0x7C};

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

// Create a data structure to receive data & send data
// Matches the sender struct & receiver struct
typedef struct message {
  int id;
  float moisture;
} message;

// Create message that stores variables to be received
message soilData;

// Hold the readings from each sensor
message sensor1;
message sensor2;
message sensor3;
message sensor4;
message sensor5;
message sensor6;

// Use array to hold all the messages
message fieldSensors[NUM] = { sensor1, sensor2, sensor3, sensor4, sensor5, sensor6 };

// Callback gets executed when a message is received by Terminal
// Prints MAC address of sender, message size, and sensor variables
void OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len) {
  
  // Get MAC address of the sender
  char fieldAddress[18];
  Serial.print("Packet received from: ");
  snprintf(fieldAddress, sizeof(fieldAddress), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(fieldAddress);
  
  // Copy received message into a new message called soilData
  memcpy(&soilData, incomingData, sizeof(soilData));
  
  
  // Assign soilData values to the corresponding sensor in the array
  fieldSensors[soilData.id - 1].id = soilData.id;
  fieldSensors[soilData.id - 1].moisture = soilData.moisture;
  Serial.printf("Sensor ID %u: %u bytes sent\n", fieldSensors[soilData.id-1].id, len);
  Serial.printf("Moisture value: %f V\n", fieldSensors[soilData.id-1].moisture);
  Serial.println();
  
}

// Callback gets executed when data is sent by Terminal
// Prints whether array has been successfully sent to Relay1
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
    Serial.println("");
  }
  else{
    Serial.println("Delivery fail");
    Serial.println("");

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
  
  // Register peer as Relay1 ESP
  esp_now_add_peer(relay1Address, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  

}

void loop() {  
  // Terminal sends data every 10 seconds
  if ((millis() - lastTime) > timerDelay) {
    // Send the message via ESP-NOW
    esp_now_send(relay1Address, (uint8_t *) &fieldSensors, sizeof(fieldSensors));

    lastTime = millis();
  }
}
