#include "Arduino.h"
#include "TLCMatrix.h"

static int _MAP[64];

char TLCMatrix::spi_transfer(volatile char data) {
  SPDR = data;
  while (!(SPSR & (1<<SPIF))) {};
  return SPDR; //SPSR wird gecleared, wenn SPIF=1 und SPDR gelesen wird
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

  //SPE=1: SPI aktivierem
  //MSTR=1: Master Mode
  //SPCR = (1<<SPE)|(1<<MSTR);//|B00000011;
  //SPSR |= (1<<SPI2X);

  delay(10);

  // for (int i = 0; i < 96; i++) {
  //   spi_transfer(0);
  // }

  latch();

  PORTB |= B00100000;
  PORTB &= B11011111;

  digitalWrite(BLANK, LOW);
}

void TLCMatrix::startGrayscaleCycle() {
  digitalWrite(BLANK, LOW);

  PORTD |= B00001000;
  PORTD &= B11110111;
}

void TLCMatrix::endGrayscaleCycle() {
  digitalWrite(BLANK, HIGH);
}

byte TLCMatrix::setBits(int start, int num, bool data) {
  // byte output = B00000000;
  //
  // if (!data) return output;
  //
  // for (int i = 0; i < num; i++) {
  //   output |= (1 << i);
  // }
  //
  // return output << start;
  if (!data) return 0;
  if (start == 0 && num == 8) {
    return B11111111;
  } else if (start == 0 && num == 4) {
    return B00001111;
  } else if (start == 4) {
    return B11110000;
  } else {
    return B00000000;
  }
}

void TLCMatrix::setPixeldata(byte (*pixeldata)[8], int led) {
  digitalWrite(SIN, HIGH);
PORTB |= B00100000;
PORTB &= B11011111;
digitalWrite(SIN, LOW);
  // for(int i = 0; i < 12 * 16 * 4; i++) {
  //   PORTB |= B00100000;
  //   delay(1);
  //   PORTB &= B11011111;
  // }

  //  byte nextByte = B00000000;
  // int usedNextBits = 0;
  //
  // for (int i = 63; i >= 0; i--) {
  //   //bool data = *pixeldata[_MAP[i]/8] & (1 << _MAP[i]%8); //Position im Array: x, Position im Byte: z
  //   bool data = (i==led) ? true:false;
  //   if (usedNextBits == 8) {
  //     spi_transfer(nextByte);
  //     nextByte = B00000000;
  //     usedNextBits = 0;
  //   }
  //
  //   spi_transfer(nextByte | setBits(0, 8-usedNextBits, data));
  //
  //   nextByte = setBits(4-usedNextBits, 4+usedNextBits, data);
  //
  //   usedNextBits = 4+usedNextBits;
  // }

  //spi_transfer(nextByte);
}

void TLCMatrix::latch() {
  //digitalWrite(XLAT, HIGH);
  //digitalWrite(XLAT, LOW);

  PORTB |= B00000010;
  PORTB &= B11111101;
}
