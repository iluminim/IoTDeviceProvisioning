int durationMs = 263520000; //183 days = 6 months

// Function to get the last 6 characters from a char array
void getLastSixChars(char* originalArray, char* lastSixChars) {
  // Determine the length of the original array
  int length = strlen(originalArray);

  // Check if the length is at least 6
  if (length >= 6) {
    // Copy the last 6 characters from the original array
    strncpy(lastSixChars, originalArray + length - 6, 6);

    // Add null terminator to the end of the new array
    lastSixChars[6] = '\0';
  } else {
    // If the original array is shorter than 6 characters, copy the whole array
    strncpy(lastSixChars, originalArray, length);
    lastSixChars[length] = '\0';
  }
}

void sendClaimRequest() {

  char secretKey[7]; // Extra space for null terminator
  getLastSixChars(PROVISION_DEVICE_NAME, secretKey);

  Serial.println("**** Device claiming started ******");
  Serial.println("Original Array:");
  Serial.println(PROVISION_DEVICE_NAME);
  Serial.println("Last 6 Characters:");
  Serial.println(secretKey);

  StaticJsonDocument<200> doc;
  doc["secretKey"] = secretKey;
  doc["durationMs"] = durationMs;

  char buffer[256];
  serializeJson(doc, buffer);
  
  Serial.print("Device name: ");
  Serial.println(PROVISION_DEVICE_NAME);
  Serial.print("Claim duration in milliseconds (6 months): ");
  Serial.println(durationMs);
  Serial.print("Claim request: "); 
  Serial.println(buffer);
  client.publish("v1/devices/me/claim", buffer);
  Serial.println("Claiming request was sent, now you should use claiming device widget to finish the claiming process.");
  Serial.println("**** Device claiming request sent ******");
}