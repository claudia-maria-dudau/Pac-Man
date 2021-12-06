#include <EEPROM.h>

void writeIntIntoEEPROM(int address, int number)
{ 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}

void setup() {
  Serial.begin(9600);
  writeIntIntoEEPROM(45, 0);
  int number = readIntFromEEPROM(45);
  Serial.print("Number: ");
  Serial.println(number);
}

void loop() {}
