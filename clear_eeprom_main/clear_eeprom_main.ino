#include <EEPROM.h>

const int EEPROM_SIZE = 1024; // EEPROM size in bytes

void initEEPROM(const int EEPROM_SIZE) {
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("Failed to initialise EEPROM");
    return;
  }
  Serial.println("EEPROM initialized successfully");
}

void eraseCharArrayToEEPROM(int startAddress, int length) {
  for (int i = 0; i < length; ++i) {
    EEPROM.write(startAddress + i, 0);
  }
  EEPROM.commit(); // Make sure to commit changes to EEPROM
}

void setup() {
  Serial.begin(115200);
  initEEPROM(EEPROM_SIZE);
  // uncomment if you add the firmware for the first time for the device. 
  eraseCharArrayToEEPROM(0, 22);
  Serial.println("Erased memory");
}

void loop(){
  Serial.println("Memory cleared. Done");
  delay(10000);
}