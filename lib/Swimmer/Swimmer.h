#ifndef SWIMMER_H
#define SWIMMER_H

#include <Arduino.h>

typedef struct {
    byte r, g, b;
} SwimmerColor;

class Swimmer {
private:
    uint32_t nLed, pos = 0, step = 1, nVasche = 0, nRipetizioni = 0, nSerie = 0, totSerie, totVasche, totRip, strip_length;
    SwimmerColor colors;
    unsigned long delay_repetition, delay_series, delay_step, time_call = 0;
    bool isfirstTime = true, istotVascheOdd = false;
public:

    Swimmer();

    void init(uint32_t p_nLed, byte p_r, byte p_g,
              byte p_b, uint32_t p_totVasche, uint32_t p_totrip, uint32_t p_totSerie,
              uint32_t p_strip_length, uint32_t p_delay_step = 0, unsigned long p_delay_repetition = 0,
              unsigned long p_delay_series = 0);

    uint32_t getNled();

    uint32_t getPos();

    void doStep();

    void undoStep();

    void gestione_serie();

    void autoStep();

    SwimmerColor getColors();

};

#endif
