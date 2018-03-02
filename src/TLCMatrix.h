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
  static void pwmCycle();
  static void setPixeldata(byte (*pixeldata)[8]);
private:
  TLCMatrix() {}
  static char spi_transfer(volatile char data);
  static void pulseGSCLK();
  static byte setBits(int start, int num);
  static int _MAP[64];
};

#endif
