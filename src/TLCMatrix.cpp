#include "Arduino.h"
#include "TLCMatrix.h"

char TLCMatrix::spi_transfer(volatile char data) {
  SPDR = data;
  while (!(SPSR & (1<<SPIF))) {};
  return SPDR;
}

void TLCMatrix::init() {
  pinMode(SIN, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(XLAT, OUTPUT);
  pinMode(GSCLK, OUTPUT);
  pinMode(BLANK, OUTPUT);

  digitalWrite(SIN, LOW);
  digitalWrite(SCLK, LOW);
  digitalWrite(XLAT, LOW);
  digitalWrite(GSCLK, LOW);
  digitalWrite(BLANK, HIGH);

  for(int i = 0; i < 64; i++) {
    _MAP[i] = i;
  }

  SPCR = (1<<SPE)|(1<<MSTR);

  delay(10);

  for (int i = 0; i < 96; i++) { //12 Bit PWM * 16 Outputs * 4 TLCs
    spi_transfer(B11111111);
  }

  digitalWrite(XLAT, HIGH);
  digitalWrite(XLAT, LOW);
  PORTB |= B00100000;
  PORTB &= B11011111;
  digitalWrite(BLANK, LOW);
}

void TLCMatrix::pulseGSCLK() {
  PORTD |= B00001000;
  PORTD &= B11110111;
}

void TLCMatrix::pwmCycle() {
  digitalWrite(BLANK, HIGH); // Diese Zeilen evtl. ans Ende
  digitalWrite(BLANK, LOW);

  for (int i = 0; i < 4096; i++) {
    pulseGSCLK();
  }
}

byte TLCMatrix::setBits(int start, int num) { //Start: Linksseitig
  // byte output = B00000000;
  //
  // for (int i = 0; i < num; i++) {
  //   output |= 1 << i;
  // }
  //
  // return output << start;

  return (2^num-1) << start; //schneller?
}

void TLCMatrix::setPixeldata(byte (*pixeldata)[8]) {
  byte nextByte = B00000000;
  int usedBits = 0;

  for (int i = 0; i < 64; i++) {
    bool data = *pixeldata[i/8] & (1 << i%8); //Position im Array: x, Position im Byte: z


  }
}
