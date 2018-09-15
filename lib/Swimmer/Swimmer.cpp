#include "Swimmer.h"

SPIStrip *Swimmer::strip;
//Adafruit_NeoPixel *Swimmer::strip;

Swimmer::Swimmer(unsigned int p_nLed, unsigned int p_pos, unsigned int p_step, unsigned int p_r, unsigned int p_g, unsigned int p_b, unsigned int totvasche, unsigned int totrip, unsigned int totserie){
  /*firsTime = firstTime;
  */
  totVasche = totvasche +1;
  totRip = totrip;
  totSerie = totserie;
  nLed = p_nLed;
  step = p_step;
  r = p_r;
  g = p_g;
  b = p_b;
}

unsigned int Swimmer::getNled(){
  return nLed;
}
void Swimmer::setStrip(SPIStrip * strip_new){
// void Swimmer::setStrip(Adafruit_NeoPixel * strip_new){
  strip = strip_new;
  strip->begin();
  strip->clear();
}

void Swimmer::doStep(){  // incrementa in avanti
  int start_neg = pos;
  if (start_neg < 0)
    start_neg = 0;
  strip->setPixelColor(pos, r, g, b, nLed);
  pos += step; //incrementa pos del valore di step

  if (pos >= strip->numPixels()-1){
    //if (pos == strip->numPixels()-1){ // non va
    nVasche++;
    if (nVasche == totVasche){
      isRipRagg = true;
      nVasche = 1;
      nRipetizioni++;
      if (nRipetizioni == totRip){
        isSerieRagg = true;
        nSerie++;
        if (nSerie == totSerie){
          isSerieTotRagg = true;
        }
      }
    }
  }
}

void Swimmer::undoStep(){  // decrementa


  int finish_neg = pos+nLed-1+step;
  if (finish_neg >= strip->numPixels())
  finish_neg = strip->numPixels()-1;
  strip->setPixelColor(pos, r, g, b, nLed, false);
  pos -= step;

  if (pos <= 0){
    nVasche++;
    if (nVasche == totVasche){
      isRipRagg = true;
      nVasche = 1;
      nRipetizioni++;

      if (nRipetizioni == totRip){

        nSerie++;
        isSerieRagg = true;
        if (nSerie == totSerie){
          isSerieRagg = true;
        }

      }
    }
  }

}

//void Swimmer::lightup(){   // originale

//  if (pos+nLed <= strip->numPixels()-1){
//    for (int i=pos; i<pos+nLed; i++){
//      strip->setPixelColor(i, r, g, b);
//    }
//  }
// }

unsigned int Swimmer::getNvasche(){
  return nVasche;
}

unsigned int Swimmer::getPos(){
  return pos;
}

bool Swimmer::isFinishRip(){

  return isRipRagg;
}

bool Swimmer::isFinishSerie(){

  return isSerieRagg;
}

bool Swimmer::isFinishSerieTot(){
  return isSerieTotRagg;
}

void Swimmer::autoStep(bool autoLightUp = false){ // contollo andata e ritorno

  /*isFirstTime();
  */
  if (nVasche == 0)
  doStep();

  if(nVasche %2 == 0){    //controlla se pari
    undoStep();
  }else {
    doStep();
  }
}

void Swimmer::resetRip(){
  nVasche = 1;
  isRipRagg = false;

}

void Swimmer::resetSerie(){

  nVasche = 1;
  nRipetizioni = 0;
  isSerieRagg = false;
}

void Swimmer::resetSerieTot(){
  resetSerie();
  nSerie = 0;
  isSerieTotRagg = false;
}

/*  void Swimmer::isFirstTime(){
if (firsTime == true && downStart == false ){
nVasche = 0;
doStep();
}
else{
//else(firsTime && downStart == true){
nVasche = 0;
undoStep();
}
}
*/
