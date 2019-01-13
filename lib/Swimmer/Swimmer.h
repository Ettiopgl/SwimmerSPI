#ifndef SWIMMER_H
#define SWIMMER_H

#include <Arduino.h>

//Qua bisogna fare il commento della classe.
class Swimmer
{
private:
  //  static Adafruit_NeoPixel * strip;
  unsigned int nLed, pos, step, r, g, b, nVasche = 1, nRipetizioni = 0, nSerie = 0, totVasche, totRip, nSerieTotRag, totSecSerie, strip_length, delay_step, delay_repetition, delay_series;
  bool isRipRagg = false, isSerieRagg = false, isSerieTotRagg = false, firsTime = false, blink = true, goSwim = true;
  unsigned long previousMillis; // partenza rec ripetizioni;  // variabile recupero ripetizioni

  unsigned int time_call = 0; //variabile che mi tiene conto di quante volte è stata chiamata la funzione autostep.
public:
  unsigned long recRip, tstart;

  bool downStart = false; //parte dalla vasca di ritorno
  //  p_goSwim      parti swimmer!

  Swimmer();

  void init(unsigned int p_nLed, unsigned int p_pos, unsigned int p_step, unsigned int p_r, unsigned int p_g, unsigned int p_b, unsigned int totvasche, unsigned int totrip, unsigned int totSecSerie, unsigned int p_strip_length, unsigned int p_delay_step = 0, unsigned int p_delay_repetition = 0, unsigned int p_delay_series = 0);

  unsigned int getNled();

  unsigned int getLength();

  unsigned int getPos();

  void doStep();

  void undoStep();

  unsigned int getNvasche();

  bool isFinishRip();

  bool isFinishSerie();

  void resetRip();

  void resetSecSerie();

  bool isFinishSerieTot();

  void resetSecSerieTot();

  void autoStep();

  void isFirstTime();
};

#endif
