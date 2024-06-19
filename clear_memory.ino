void eraseCharArrayToEEPROM(int startAddress, int length) {
  for (int i = 0; i < length; ++i) {
    EEPROM.write(startAddress + i, 0);
  }
  EEPROM.commit(); // Make sure to commit changes to EEPROM
}


void eraseBooleanInEEPROM(int address) {
  EEPROM.write(address, 0); // Writing 0 to the specific address
  EEPROM.commit();
}