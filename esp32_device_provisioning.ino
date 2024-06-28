#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "FS.h"
#include "SPIFFS.h"
#include <EEPROM.h>

// WiFi configurations
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

//MQTT broker configurations
const char* mqtt_server = "mqtt.thingsboard.cloud";
const int mqtt_port = 1883;

//MQTT configurations
const char* mqtt_user="provision";
const char* mqtt_password ="";
const char* provision_device_key = "PROVISION_DEVICE_KEY";
const char* provision_device_secret = "PROVISION_DEVICE_SECRET";
const char* provision_request_topic = "/provision/request";
const char* provision_response_topic = "/provision/response";

char* accessTokenChar = new char[20];

bool devs;
const int EEPROM_SIZE = 1024; // EEPROM size in bytes

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  initEEPROM(EEPROM_SIZE);
  check_provisioned_status();
}


void loop() {

  if (!client.connected()) {
    Serial.println("Client not connected");
    if (devs) {
    device_mqtt_reconnect();
    Serial.println("Client connection completed");
    }
    else {
    Serial.println("Device not provisioned to connect");
    provision_mqtt_reconnect();
    }
  }

  if (devs) {
    
    Serial.println("loop started client already connected publish loop");
    publishmsg();
    Serial.println("publish loop completed ");

  }

  client.loop();   

  delay(10000); // Publish message every 10 seconds
}