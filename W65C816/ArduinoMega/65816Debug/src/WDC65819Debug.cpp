#include <Arduino.h>

const uint8_t ADDR[] = {53,51,49,47,45,43,41,39,37,35,33,31,29,27,25,23};
const uint8_t DATA[] = {52,50,48,46,44,42,40,38};
#define CLOCK 2
#define RWB 3
#define E 4
#define ADDRPINS (sizeof(ADDR)/sizeof(ADDR[0]))
#define DATAPINS (sizeof(DATA)/sizeof(DATA[0]))

void onClock();

void setup() {
  pinMode(CLOCK, INPUT);
  pinMode(RWB, INPUT);
  for(uint8_t a = 0; a < ADDRPINS; ++a) {
    pinMode(ADDR[a], INPUT);
  }
  for(uint8_t d = 0; d < DATAPINS; ++d) {
    pinMode(DATA[d], INPUT);
  }

  attachInterrupt(digitalPinToInterrupt(CLOCK), onClock, RISING);
  Serial.begin(57600); 
}

uint16_t readAddress() {
  uint16_t address = 0;
  for(uint8_t a = 0; a < ADDRPINS; ++a) {
      uint8_t bit = digitalRead(ADDR[a]) ? 1 : 0;
      Serial.print(bit);
      address = (address << 1) + bit;
  }
  Serial.print(" ");
  return address;
}
uint8_t readData() {
  uint8_t data = 0;
  for(uint8_t d = 0; d < DATAPINS; ++d) {
      uint8_t bit = digitalRead(DATA[d]) ? 1 : 0;
      Serial.print(bit);
      data = (data << 1) + bit;
  }
  return data;
  
}

void onClock() {
  char output[32];
  uint16_t addr = readAddress();
  uint8_t data = readData();
  sprintf(output, " Addr: 0x%04X Data: 0x%02X R/W: %c EMU: %c", addr, data, digitalRead(RWB) ? 'R' : 'W', digitalRead(E) ? 'E' : 'N');
  Serial.println(output);
}


void loop() {
  // put your main code here, to run repeatedly:

}
