#include "SPIStrip.h"
#include <SPI.h>

SPIStrip::SPIStrip(uint16_t n){
  nPixels = n;
}

void SPIStrip::begin(){
  SPI.begin();
}

void SPIStrip::clear(){ // spegne tutti i led
  setPixelColor(nPixels, 0, 0, 0, 0, true);
}

void SPIStrip::setPixelColor(uint16_t nStartLed, uint8_t r, uint8_t g, uint8_t b, uint8_t len_segment, bool is_forward){
//nStartLed= led da accendere, r,g,b,len_segment=lungezza segmento,is_forward accende in avanti
  if(nStartLed > 0)  {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    for(uint16_t i = 0; i<nStartLed; i++){
      PRINTLED(0,0,0); // printa i led "spenti"
    }
    for (uint8_t i=0; i<len_segment; i++){
      PRINTLED(r,g,b); // printa il segmneto
    }
    if (!is_forward){
      PRINTLED(0,0,0); // se torna indietro "cancella l'ultimo led"
    }
    SPI.endTransaction();
  }

  delayMicroseconds(510); // ritardo hardware di fine trasmissione min=500uS


}

uint16_t SPIStrip::numPixels(){
  return nPixels; // lunghezza della striscia
}
