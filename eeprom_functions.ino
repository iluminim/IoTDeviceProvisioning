  // Initialize EEPROM
void initEEPROM(const int EEPROM_SIZE) {
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("Failed to initialise EEPROM");
    return;
  }
  Serial.println("EEPROM initialized successfully");
}


void writeEEPROM(int address, bool myBoolean){
  EEPROM.writeBool(address, myBoolean);
  EEPROM.commit();
  Serial.println("Boolean value written to EEPROM");
}


bool readEEPROM(int address){
  bool myBoolean = EEPROM.readBool(address);
  Serial.print("Boolean value read from EEPROM: ");
  Serial.println(myBoolean ? "true" : "false");
  return myBoolean;
}


void writeCharArrayToEEPROM(int startAddress, char* data, int length) {
  for (int i = 0; i < length; ++i) {
    EEPROM.write(startAddress + i, data[i]);
  }
  EEPROM.commit(); // Make sure to commit changes to EEPROM
}


char* readCharArrayFromEEPROM(int startAddress, int length) {
  char* data = new char[length]; // Allocate memory for the char array
  for (int i = 0; i < length; ++i) {
    data[i] = EEPROM.read(startAddress + i);
  }
  data[length-1] = '\0'; // Null-terminate the char array
  return data; // Return the pointer to the char array
}