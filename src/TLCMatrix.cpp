#include "Arduino.h"
#include "TLCMatrix.h"

static int _MAP[64];

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

  for (int i = 0; i < 64; i++) {
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
  digitalWrite(BLANK, LOW);

  for (int i = 0; i < 4096; i++) {
    pulseGSCLK();
  }

  digitalWrite(BLANK, HIGH); // Diese Zeilen evtl. ans Ende
}

byte TLCMatrix::setBits(int start, int num, bool data) {
  byte output = B00000000;

  if (!data) return output;

  for (int i = 0; i < num; i++) {
    output |= (1 << i);
  }

  return output << start;
}

void TLCMatrix::setPixeldata(byte (*pixeldata)[8], int led) {
  byte nextByte = B00000000;
  int usedNextBits = 0;

  for (int i = 63; i >= 0; i--) {
    //bool data = *pixeldata[_MAP[i]/8] & (1 << _MAP[i]%8); //Position im Array: x, Position im Byte: z
    bool data = (i == led) ? true : false;
    if (usedNextBits == 8) {
      spi_transfer(nextByte);
      nextByte = B00000000;
      usedNextBits = 0;
    }

    spi_transfer(nextByte | setBits(0, 8-usedNextBits, data));

    nextByte = setBits(4-usedNextBits, 4+usedNextBits, data);

    usedNextBits = 4+usedNextBits;
  }
}

void TLCMatrix::latch() {
  digitalWrite(XLAT, HIGH);
  digitalWrite(XLAT, LOW);
}
