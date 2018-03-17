#ifndef TLCMatrix_h
#define TLCMatrix_h

#define SIN 11
#define SCLK 13
#define XLAT 9
#define GSCLK 3
#define BLANK 10

#include "Arduino.h"

class TLCMatrix {
public:
  static void init();
  static void startGrayscaleCycle();
  static void endGrayscaleCycle();
  static void setPixeldata(byte (*pixeldata)[8], int led);
  static void latch();
private:
  TLCMatrix() {}
  static char spi_transfer(volatile char data);
  static byte setBits(int start, int num, bool data);
};

#endif
