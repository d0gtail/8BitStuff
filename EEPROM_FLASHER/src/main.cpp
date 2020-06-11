#include <Arduino.h>

//ATMEL AT28C256 DATA
#define EEPROM_SIZE 262144
#define EEPROM_MAX_ADDRESS (EEPROM_SIZE / 8) - 1


const uint8_t DATA_BUS[] = {5,6,7,8,9,10,11,12};

#define SHIFT_DATA 2
#define SHIFT_CLOCK 3
#define SHIFT_LATCH 4
#define DATAPINS (sizeof(DATA_BUS) / sizeof(DATA_BUS[0]))
#define OE 13
#define WE 14

uint8_t assembly[]{
  0xA9, 0xFF, // lda #$FF, prepare Value to set all pins on VIA Port A to output (0xFF to 0x6002)
  0x8D, 0x03, 0x60, // sta $6003 (all pins for Port A)

  0xA9, 0xFF, // lda #$FF, prepare Value to set all pins on VIA Port B to output (0xFF to 0x6002)
  0x8D, 0x02, 0x60, // sta $6002 (all pins for Port B)

  // Memory pos. 0x800A
  0xA9, 0x55, // lda #$55, 0x55 for output on VIA Port A
  0x8D, 0x01, 0x60, // sta $6001 (output VIA Port A)

  0xA9, 0xAA, // lda #$00, 0x00 for output on VIA Port A
  0x8D, 0x01, 0x60, // sta $6001 (output VIA Port A)

  0xA9, 0xAA, // lda #$AA, 0xAA for output on VIA Port B
  0x8D, 0x00, 0x60, // sta $6000 (output VIA Port B)

  0xA9, 0x55, // lda #$55, 0x55 for output on VIA Port B
  0x8D, 0x00, 0x60, // sta $6000 (output VIA Port B)

  0x4C, 0x0A, 0x80 // jmp $800A

};

uint8_t readByte(uint16_t address);
void writeByte(uint16_t address, uint8_t data);

void buildROM(){
  for(uint16_t pos; pos < EEPROM_MAX_ADDRESS; ++pos) {
    if(pos < sizeof(assembly)) {
      writeByte(pos, assembly[pos]);
    } else if(pos == 0x7FFC) {
      writeByte(pos, 0x00); // 0x00 Postreset jump address LSB
    } else if(pos == 0x7FFD) {
      writeByte(pos,0x80); // 0x80 Postreset jump address MSB
    } else {
      writeByte(pos, 0xEA); // fill the rest of the EEPROM with NOPs
    }
  }
}

void buildAssemblyOnly() {
  for(uint16_t pos; pos < sizeof(assembly); ++pos) {
    writeByte(pos, assembly[pos]);
    Serial.printf("Assembly size: %d, position %d\n", sizeof(assembly),pos);
  }
}
/*
 * Read some content of the EEPROM and print them to the serial monitor.
 */
void readSomeBytes() {
  for (int base = 0; base <= 255; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readByte(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}

void setAddress(uint16_t address){
  shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, (address >> 8));
  shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, address);

  // Trigger the latch to move the data from the shift register to the storage register
  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

uint8_t readByte(uint16_t address) {
  for(uint8_t d = 0; d < DATAPINS; ++d) {
    pinMode(DATA_BUS[d], INPUT);
  }
  uint8_t buf = 0x00;
  setAddress(address);
  digitalWrite(OE, LOW);
for(int d = (DATAPINS - 1); d >= 0; --d) { //TODO: clean DATAPINS up
      uint8_t bit = digitalRead(DATA_BUS[d]);
      buf = (buf << 1) + bit; //TODO: optimize
  }
  digitalWrite(OE, HIGH);
  return buf;
}

uint8_t pollByte() {
  for(uint8_t d = 0; d < DATAPINS; ++d) {
    pinMode(DATA_BUS[d], INPUT);
  }
  uint8_t buf = 0x00;
  digitalWrite(OE, LOW);
for(int d = (DATAPINS - 1); d >= 0; --d) { //TODO: clean DATAPINS up
      buf = (buf << 1) + digitalRead(DATA_BUS[d]);
  }
  digitalWrite(OE, HIGH);
  return buf;
}

void readAll() {
  for(uint16_t i = 0; i <= EEPROM_MAX_ADDRESS; ++i) {
    readByte(i);
  }
}

void writeByte(uint16_t address, uint8_t data) {
  uint8_t writeData = data;
  setAddress(address);
  digitalWrite(WE, LOW);
  for(uint8_t d = 0; d < DATAPINS; ++d) {
    pinMode(DATA_BUS[d], OUTPUT);
    digitalWrite(DATA_BUS[d], writeData & 1);
    writeData >>= 1;
  }
  
  //delayMicroseconds(1);
  digitalWrite(WE, HIGH); 
  digitalWrite(WE, LOW);
  digitalWrite(WE, HIGH);
  bool dataPolling = true;
  while(dataPolling) {
    if((pollByte() & 0x80) == (data & 0x80)) {
      dataPolling = false;
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLOCK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  pinMode(WE, OUTPUT);
  pinMode(OE, OUTPUT);
  for(uint8_t d = 0; d < DATAPINS; ++d) {
    pinMode(DATA_BUS[d], INPUT);
  }
  
  digitalWrite(SHIFT_DATA, LOW);
  digitalWrite(SHIFT_CLOCK, LOW);
  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(WE, HIGH);
  digitalWrite(OE, HIGH);
  Serial.begin(57600);
  /*
  * Do the actual work
  */
  setAddress(0x0000);
  //buildROM();
  buildAssemblyOnly();
  readSomeBytes();
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
}