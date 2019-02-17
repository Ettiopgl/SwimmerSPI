#include "Swimmer.h"


Swimmer::Swimmer() {
}

void Swimmer::init(unsigned int p_nLed, unsigned int p_step, byte p_r, byte p_g,
                   byte p_b, unsigned int p_totVasche, unsigned int p_totrip, unsigned int p_totSerie,
                   unsigned int p_strip_length, unsigned int p_delay_step, unsigned long p_delay_repetition,
                   unsigned long p_delay_series) {
    totVasche = p_totVasche; // numero di vasche per ripetizione
    istotVascheOdd = totVasche % 2 != 0; //se dispari...
    totRip = p_totrip;           // totale ripetizioni
    totSerie = p_totSerie; // totale serie
    nLed = p_nLed;             // numero led segmento
    step = p_step;             // numero degli step  (salto tra un led e l'altro )
    colors.r = p_r;
    colors.g = p_g;
    colors.b = p_b;
    strip_length = p_strip_length; //linghezza striscia
    delay_step = p_delay_step;  //ritardo  di partenza
    delay_repetition = p_delay_repetition;  // recupero ripetizioni
    delay_series = p_delay_series;
}

unsigned int Swimmer::getNled() {
    return nLed;
}

void Swimmer::doStep() {
    pos += step; //incrementa di uno step

    if (pos >= strip_length)  // se siamo a fine vasca  -1
    {
        gestione_serie();
    }
    isfirstTime = false; // prima vasca
}

void Swimmer::undoStep() {
    pos -= step;


    if (pos <= 0) {
        gestione_serie();
    }
}

void Swimmer::gestione_serie() {
    {
        nVasche++;
        if (nVasche >= totVasche) {
            if (delay_repetition > 0) {
                time_call = 0;
                delay_step = delay_repetition;
                if (istotVascheOdd)
                    totVasche = totVasche - 1;
            }
            nVasche = 0;

            nRipetizioni++;
            if (nRipetizioni == totRip) {
                if (delay_series > 0) { //Attendo delay_series
                    time_call = 0;
                    delay_step = delay_series;
                }
                nSerie++;
                nRipetizioni = 0;
                if (nSerie == totSerie) {
                    if (delay_series > 0) {
                        time_call = 0;
                        delay_step = delay_series * 10;
                        nSerie = 0;
                    }

                }
            }
        }
    }
}

uint32_t Swimmer::getPos() {
    return pos;
}

void Swimmer::autoStep() {
    if (time_call >= delay_step) {
        if (isfirstTime)  // comporta il cambio di nVasche ed inserire isfirstTime = false alla fine di doStep dopo nVasche++
            doStep();


        else if (nVasche % 2 != 0)   //1  /// or  nVasche == 0 and nRipetizioni ==1
        {
            undoStep();
        } else {
            doStep();
        }
    } else {
        ++time_call;
    }
}

SwimmerColor Swimmer::getColors(){
    return colors;
}

