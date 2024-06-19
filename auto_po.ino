#define PROVISION_DEVICE_NAME   "DEVICE_NAME_TO_CREATE"
#define THINGSBOARD_TELEMETRY_TOPIC "v1/devices/me/telemetry"

#define EEPROM_START_ADDRESS 1
#define CHAR_ARRAY_LENGTH 21

char* read_credentials(int, int);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  
  delay(10);
  accessTokenChar = read_credentials(EEPROM_START_ADDRESS, CHAR_ARRAY_LENGTH);

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection with the device in Thingsboard...");
    if (client.connect("ESP32Client2", accessTokenChar, NULL)) {
      Serial.println("connected to mqtt user device");
    } else {
      Serial.print("2 failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void initial_reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting initial MQTT connection for provisioning...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(provision_response_topic);
      
      DynamicJsonDocument provision_request(256);
      provision_request["deviceName"] = PROVISION_DEVICE_NAME;
      provision_request["provisionDeviceKey"] = provision_device_key;
      provision_request["provisionDeviceSecret"] = provision_device_secret;
      char buffer[256];
      serializeJson(provision_request, buffer);
      client.publish(provision_request_topic, buffer);
      Serial.println(buffer);
    } else {
      Serial.print(" 1 failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void device_provisioned(){
      client.disconnect();
      Serial.println("***********************Device provisioned successfully*************************");
      writeEEPROM(0,1);
      delay(10);
      devs=readEEPROM(0);
}


void copyConstCharToChar(const char* src, char* dest, int length) {
  // Ensure we don't overflow the destination array
  for (int i = 0; i < length - 1; ++i) {
    dest[i] = src[i];
    if (src[i] == '\0') break; // Stop if the end of the source string is reached
  }
  dest[length - 1] = '\0'; // Ensure the destination array is null-terminated
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  // Create a buffer to store the payload
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';
  
  // Print the received message
  Serial.println(message);

  DynamicJsonDocument doc(256);
  deserializeJson(doc, payload, length);

  const char* status = doc["status"];
  if (strcmp(status, "SUCCESS") == 0) {
  const char* credentials_value = doc["credentialsValue"];
  
  // Convert const char* to char*
  char dataToWriteChar[CHAR_ARRAY_LENGTH];
  copyConstCharToChar(credentials_value, dataToWriteChar, CHAR_ARRAY_LENGTH);

  writeCharArrayToEEPROM(EEPROM_START_ADDRESS, dataToWriteChar, CHAR_ARRAY_LENGTH);
  Serial.println("Data written to EEPROM.");

  Serial.println("Credentials saved successfully");
  device_provisioned();
  Serial.println("***********************Device provisioned successfully*************************");

  } else {
    const char* error_msg = doc["errorMsg"];
    Serial.printf("Provisioning failed with status: %s, error message: %s\n", status, error_msg);
  }
}


char* read_credentials(int _EEPROM_START_ADDRESS, int _CHAR_ARRAY_LENGTH) {
  char* Token = readCharArrayFromEEPROM(_EEPROM_START_ADDRESS, _CHAR_ARRAY_LENGTH);
  Serial.print("Data read from EEPROM: ");
  Serial.println(Token);
  return Token;
}


void initialize_device(){
  // eraseCharArrayToEEPROM(EEPROM_START_ADDRESS, CHAR_ARRAY_LENGTH);

  delay(10);
  devs=readEEPROM(0);
  Serial.print("Initial status of deivce: ");
  Serial.println(devs);

  if (!devs){
    initial_reconnect();
  }
  else {
    accessTokenChar = read_credentials(EEPROM_START_ADDRESS, CHAR_ARRAY_LENGTH);
    reconnect();
  }
  delay(100);
}
