#ifndef SPISTRIP
#define SPISTRIP

#include <Arduino.h>
#define PRINTLED(R,G,B) SPI.transfer(R);SPI.transfer(G);SPI.transfer(B);

class SPIStrip{

  private:

    uint16_t nPixels;

public:

  SPIStrip(uint16_t);

  void clear();

  void begin();

  void setPixelColor(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, bool = true);

  uint16_t numPixels();

};

#endif
