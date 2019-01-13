#include "Swimmer.h"

//Adafruit_NeoPixel *Swimmer::strip;
//                    p_nLed=led segmento,p_pos=posizione led,p_step=step in avanti 1 2....,p_r=red ,p_g=green,p_b= blue, totvasche= numero vasche,totrip=numero ripetizioni,totSecSerie=numero serie,p_strip_length=lung striscia,p_delay_step=ritardo partenza 3,5sec,p_delay_repetition =REcupero ripetizione ,p_delay_series = Recupero serie
Swimmer::Swimmer()
{
}

void Swimmer::init(unsigned int p_nLed, unsigned int p_pos, unsigned int p_step, unsigned int p_r, unsigned int p_g, unsigned int p_b, unsigned int totvasche, unsigned int totrip, unsigned int totSecSerie, unsigned int p_strip_length, unsigned int p_delay_step = 0, unsigned int p_delay_repetition = 0, unsigned int p_delay_series = 0)
{
  totVasche = totvasche + 1; // numero di vasche per ripetizione
  totRip = totrip;           // totale ripetizioni
  totSecSerie = totSecSerie; // totale serie
  nLed = p_nLed;             // numero led segmento
  step = p_step;             // numero degli step  (salto tra un led e l'altro )
  r = p_r;
  g = p_g;
  b = p_b;
  strip_length = p_strip_length; //linghezza striscia
  delay_step = p_delay_step;
  delay_repetition = p_delay_repetition;
  delay_series = p_delay_series;
}

unsigned int Swimmer::getNled()
{
  return nLed;
}

void Swimmer::doStep()
{
  int start_neg = pos;
  if (start_neg < 0)
    start_neg = 0;
  pos += step;

  if (pos >= strip_length - 1)
  { //Fine vasca
    nVasche++;
    if (nVasche == totVasche)
    { //Fine Ripetizione 50..100 così via
      if (delay_repetition > 0)
      { //Attendo delay_repetition
        time_call = 0;
        delay_step = delay_repetition;
      }
      isRipRagg = true;
      nVasche = 1;
      nRipetizioni++;
      if (nRipetizioni == totRip)
      {

        if (delay_series > 0)
        { //Attendo delay_series
          time_call = 0;
          delay_step = delay_series;
        }

        isSerieRagg = true;
        nSerie++;
        if (nSerie == totSecSerie)
        { // QUI DEVE FERMARSI DEFINITIVAMENTE
          if (delay_series > 0)
          { //Attendo delay_series
            time_call = 0;
            delay_step = delay_series * 10; // QUI DEVE FERMARSI DEFINITIVAMENTE
          }
          isSerieTotRagg = true;
        }
      }
    }
  }
}

void Swimmer::undoStep()
{
  int finish_neg = pos + nLed - 1 + step;
  if (finish_neg >= strip_length)
    finish_neg = strip_length - 1;
  pos -= step;

  if (pos <= 0)
  { //Fine vasca
    nVasche++;
    time_call = 0;
    if (nVasche == totVasche)
    {
      if (delay_repetition > 0)
      {
        delay_step = delay_repetition;
      }
      isRipRagg = true;
      nVasche = 1;
      nRipetizioni++;

      if (nRipetizioni == totRip)
      {

        if (delay_series > 0)
        { //Attendo delay_series
          time_call = 0;
          delay_step = delay_series;
        }

        nSerie++;
        isSerieRagg = true;
        if (nSerie == totSecSerie)
        { // QUI DEVE FERMARSI DEFINITIVAMENTE
          if (delay_series > 0)
          { //Attendo delay_series
            time_call = 0;
            delay_step = delay_series; // QUI DEVE FERMARSI DEFINITIVAMENTE
          }

          isSerieRagg = true;
        }
      }
    }
  }
}

unsigned int Swimmer::getNvasche()
{
  return nVasche;
}

unsigned int Swimmer::getPos()
{
  return pos;
}

unsigned int Swimmer::getLength()
{
  return strip_length;
}

bool Swimmer::isFinishRip()
{

  return isRipRagg;
}

bool Swimmer::isFinishSerie()
{

  return isSerieRagg;
}

bool Swimmer::isFinishSerieTot()
{
  return isSerieTotRagg;
}

void Swimmer::autoStep()
{
  if (time_call >= delay_step)
  {
    if (nVasche % 2 == 0)
    {
      undoStep();
    }
    else
    {
      doStep();
    }
  }
  else
  {
    ++time_call;
  }
}
void Swimmer::resetRip()
{ // reset della ripetizione 100m=4=nVasche
  nVasche = 1;
  isRipRagg = false;
}

void Swimmer::resetSecSerie()
{
  nVasche = 1;
  nRipetizioni = 0;
  isSerieRagg = false;
}

void Swimmer::resetSecSerieTot()
{
  resetSecSerie();
  nSerie = 0;
  isSerieTotRagg = false;
}
