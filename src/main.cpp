#define LAYERS_BIT1 4
#define LAYERS_BIT2 5
#define LAYERS_BIT3 6

#include "Arduino.h"
#include "TLCMatrix.h"

static byte LAYERS[] = {
  B00000000, //Layer 1
  B00010000, //Layer 2
  B00100000, //Layer 3
  B00110000, //Layer 4
  B01000000, //Layer 5
  B01010000, //Layer 6
  B01100000, //Layer 7
  B01110000 //Layer 8
};

byte voxeldata[8][8];

unsigned int loopcounter = 0;
unsigned int layer = 0;

void set_voxeldata(int x, int y, int z, bool bit) {
  if (bit) {
    voxeldata[y][x] |= 1 << z; // Z ist ein Byte, Position z des Bits wird gesetzt
  } else {
    voxeldata[y][x] &= ~(1 << z);
  }
}

void setup() {
  pinMode(LAYERS_BIT1, OUTPUT);
  pinMode(LAYERS_BIT2, OUTPUT);
  pinMode(LAYERS_BIT3, OUTPUT);

  digitalWrite(LAYERS_BIT1, LOW);
  digitalWrite(LAYERS_BIT2, LOW);
  digitalWrite(LAYERS_BIT3, LOW);

  TLCMatrix::init();

  loopcounter = 0;
  layer = 0;

  for(int x = 0; x < 8; x++) {
    for(int y = 0; y < 8; y++) {
      voxeldata[y][x] = B00000000;
    }
  }
}

void loop() {
  PORTD = (PORTD & B10001111) | LAYERS[layer];

  if (layer < 7) {
    layer++;
  } else {
    layer = 0;
  }

  TLCMatrix::pwmCycle();
  //TLCMatrix::setPixeldata(&voxeldata[layer]);

  if (loopcounter == 65535) loopcounter = 0;
  loopcounter++;
}
