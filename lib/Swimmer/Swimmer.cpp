#include "Swimmer.h"


Swimmer::Swimmer() {
}

void Swimmer::init(unsigned int p_nLed, unsigned int p_pos, unsigned int p_step, unsigned int p_r, unsigned int p_g,
                   unsigned int p_b, unsigned int p_totVasche, unsigned int p_totrip, unsigned int p_totSerie,
                   unsigned int p_strip_length, unsigned int p_delay_step, unsigned long p_delay_repetition,
                   unsigned long p_delay_series) {
    totVasche = p_totVasche; // numero di vasche per ripetizione
    if (totVasche % 2 != 0)
        istotVascheOdd = true;

    totRip = p_totrip;           // totale ripetizioni

    totSerie = p_totSerie; // totale serie
    nLed = p_nLed;             // numero led segmento
    step = p_step;             // numero degli step  (salto tra un led e l'altro )
    r = p_r;
    g = p_g;
    b = p_b;
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
            Serial.print("fine ripetizione undostep");
            if (delay_repetition > 0) {
                time_call = 0;
                delay_step = delay_repetition;
                if (istotVascheOdd)
                    totVasche = totVasche - 1;
            }
            nVasche = 0;

            nRipetizioni++;
            if (nRipetizioni == totRip) {
                isRipRagg = true;
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

                    isSerieRagg = true;
                }
            }
        }
    }
}

unsigned int Swimmer::getNvasche() {
    return nVasche;
}

unsigned int Swimmer::getPos() {
    return pos;
}

unsigned int Swimmer::getLength() {
    return strip_length;
}

bool Swimmer::isFinishRip() {

    return isRipRagg;
}

bool Swimmer::isFinishSerie() {

    return isSerieRagg;
}

bool Swimmer::isFinishSerieTot() {
    return isSerieTotRagg;
}

void Swimmer::autoStep() {
    if (time_call >= delay_step) {
        if (isfirstTime ==
            true)  // comporta il cambio di nVasche ed inserire isfirstTime = false alla fine di doStep dopo nVasche++
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

void Swimmer::resetRip() { // reset della ripetizione 100m=4=nVasche
    nVasche = 0;

    isRipRagg = false;
}

void Swimmer::resetSecSerie() {
    nVasche = 0;

    nRipetizioni = 0;
    isSerieRagg = false;
}

void Swimmer::resetSecSerieTot() {
    resetSecSerie();
    nSerie = 0;
    isSerieTotRagg = false;
}
