#include "Swimmer.h"
//Adafruit_NeoPixel *Swimmer::strip;

Swimmer::Swimmer(unsigned int p_nLed, unsigned int p_pos, unsigned int p_step, unsigned int p_r, unsigned int p_g, unsigned int p_b, unsigned int totvasche, unsigned int totrip, unsigned int totserie, unsigned int p_strip_length, unsigned int p_delay_step){
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
  strip_length = p_strip_length;
  delay_step = p_delay_step;
}

unsigned int Swimmer::getNled(){
  return nLed;
}

void Swimmer::doStep(){
  int start_neg = pos;
  if (start_neg < 0)
    start_neg = 0;
    pos += step;

  if (pos >= strip_length-1){ //Fine vasca
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

void Swimmer::undoStep(){
  int finish_neg = pos+nLed-1+step;
  if (finish_neg >= strip_length)
  finish_neg = strip_length-1;
  pos -= step;

  if (pos <= 0){ //Fine vasca
    nVasche++;
    time_call = 0;
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

unsigned int Swimmer::getNvasche(){
  return nVasche;
}

unsigned int Swimmer::getPos(){
  return pos;
}

unsigned int Swimmer::getLength(){
  return strip_length;
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

void Swimmer::autoStep(){
  if (time_call >= delay_step){
    if(nVasche %2 == 0){
      undoStep();
    }else {
      doStep();
    }
  }else{
    ++time_call;
  }
}
void Swimmer::resetRip(){ // reset della ripetizione 100m=4=nVasche
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
