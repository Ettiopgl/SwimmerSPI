#ifndef SWIMMER_H
#define SWIMMER_H

#include <Arduino.h>

typedef struct {
    byte r, g, b;
} SwimmerColor;

class Swimmer {
private:
    uint32_t nLed, pos, step, nVasche = 0, nRipetizioni = 0, nSerie = 0, totSerie, totVasche, totRip, strip_length;
    SwimmerColor colors;
    unsigned long delay_repetition, delay_series, delay_step, time_call = 0;
    bool isfirstTime = true, istotVascheOdd = false;
public:

    Swimmer();

    void init(unsigned int p_nLed, unsigned int p_step, byte p_r, byte p_g,
              byte p_b, unsigned int p_totVasche, unsigned int p_totrip, unsigned int p_totSerie,
              unsigned int p_strip_length, unsigned int p_delay_step = 0, unsigned long p_delay_repetition = 0,
              unsigned long p_delay_series = 0);

    unsigned int getNled();

    uint32_t getPos();

    void doStep();

    void undoStep();

    void gestione_serie();

    void autoStep();

    SwimmerColor getColors();

};

#endif
