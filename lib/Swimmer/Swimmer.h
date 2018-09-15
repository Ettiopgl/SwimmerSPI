#ifndef SWIMMER_H
#define SWIMMER_H

#include <Arduino.h>
//#include <Adafruit_NeoPixel.h>

#include <SPIStrip.h>

//Qua bisogna fare il commento della classe.
class Swimmer{
  private:


  static SPIStrip * strip;
  //  static Adafruit_NeoPixel * strip;
    unsigned int nLed, pos, step, r, g, b, nVasche = 1, nRipetizioni = 0, nSerie = 0, totVasche , totRip, nSerieTotRag, totSerie;
    bool isRipRagg = false, isSerieRagg = false, isSerieTotRagg = false, firsTime = false;

  public:


    bool downStart = false; //parte dalla vasca di ritorno

    Swimmer(unsigned int p_nLed, unsigned int p_pos, unsigned int p_step, unsigned int p_r, unsigned int p_g, unsigned int p_b, unsigned int totvasche, unsigned int totrip, unsigned int totserie);

    unsigned int getNled();

    unsigned int getLength();

    unsigned int getPos();

static void setStrip(SPIStrip * strip_new);

    //static void setStrip(Adafruit_NeoPixel * strip_new);

    void doStep();

    void undoStep();

    void start();

    unsigned int getNvasche();

    bool isFinishRip();

    bool isFinishSerie();

    void resetRip();

    void resetSerie();

    bool isFinishSerieTot();

    void resetSerieTot();

    void autoStep(bool autoLightUp);

    void isFirstTime();

};

#endif
