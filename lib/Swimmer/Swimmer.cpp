#include "Swimmer.h"


<<<<<<< HEAD
Swimmer::Swimmer() = default;
=======
void Swimmer::init(unsigned int p_nLed, unsigned int p_pos, unsigned int p_step, unsigned int p_r, unsigned int p_g, unsigned int p_b, unsigned int p_totVasche, unsigned int p_totrip, unsigned int p_totSerie, unsigned int p_strip_length, unsigned int p_delay_step = 0, unsigned int p_delay_repetition = 0, unsigned int p_delay_series = 0)
{
  totVasche = p_totVasche ; // numero di vasche per ripetizione
  if(totVasche % 2 == 1)
  istotVascheOdd = true;
// dddd
  totRip = p_totrip;           // totale ripetizioni
  // tSecSerie = tSecSerie; // totale serie
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
>>>>>>> parent of 873365d... giusti commit

void Swimmer::init(uint32_t p_nLed, byte p_r, byte p_g,
                   byte p_b, uint32_t p_totVasche, uint32_t p_totrip, uint32_t p_totSerie,
                   uint32_t p_strip_length, uint32_t p_delay_step, unsigned long p_delay_repetition,
                   unsigned long p_delay_series) {
    totVasche = p_totVasche; // numero di vasche per ripetizione
    istotVascheOdd = totVasche % 2 != 0; //se dispari...
    totRip = p_totrip;           // totale ripetizioni
    totSerie = p_totSerie; // totale serie
    nLed = p_nLed;             // numero led segmento
    colors.r = p_r;
    colors.g = p_g;
    colors.b = p_b;
    strip_length = p_strip_length; //linghezza striscia
    delay_step = p_delay_step;  //ritardo  di partenza
    delay_repetition = p_delay_repetition;  // recupero ripetizioni
    delay_series = p_delay_series;
}

<<<<<<< HEAD
uint32_t Swimmer::getNled() {
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
=======
void Swimmer::doStep()
{
  int start_neg = pos;
  if (start_neg < 0)
    start_neg = 0;
  pos += step; //incrementa di uno step
  Serial.print("pos/doStep=");
  Serial.println(pos);
  Serial.print("nRipetizioni=");
 Serial.println(nRipetizioni);
  if (pos >=strip_length )  // se siamo a fine vasca  -1
  {
    nVasche++;
    isfirstTime = false ; // prima vasca
    Serial.print("nVasche/totVasche =");
    Serial.print(nVasche);
    Serial.print("/");
    Serial.println(totVasche);
    if (nVasche >= totVasche)
    { //Fine Ripetizione 50..100 così via
      if (delay_repetition > 0)
      { //Attendo delay_repetition
          time_call = 0;
        delay_step = delay_repetition;  //////////////////////////////////??????????????????????????
if(istotVascheOdd)
        totVasche =totVasche +1;
      }
      Serial.print("delay_repetition to dostep=");
      Serial.println(delay_repetition);
      //isRipRagg = true; // ---->62
       nVasche = 0;


      nRipetizioni++;
      Serial.print("nRipetizioni/totRip in doStep");
      Serial.print(nRipetizioni);
      Serial.print("/");
      Serial.println(totRip);
      if (nRipetizioni == totRip)
      {
isRipRagg = true;
        if (delay_series > 0)
        { //Attendo delay_series
          time_call = 0;
          delay_step = delay_series;
        }
        Serial.print("delay_series to dostep series");
        Serial.println(delay_series);
        isSerieRagg = true;
        nSerie++;
        Serial.print("nSerie doStep/totSerie =");
        Serial.print(nSerie);
        Serial.print("/");
        Serial.println(totSerie);
        nRipetizioni = 0;
        // if (nSerie == tSecSerie) // variabile sbagliata tSecSerie
      if (nSerie == totSerie)
        { // QUI DEVE FERMARSI DEFINITIVAMENTE
          if (delay_series > 0)
          { //Attendo delay_series
            time_call = 0;
            Serial.print("Fine SERIE");
            Serial.println(nSerie);
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
  if (finish_neg >  strip_length)
    finish_neg = strip_length - 1;
  pos -= step;
  Serial.print("pos/undoStep=");
  Serial.println(pos);
  if (pos <= 0)
  { //Fine vasca
    nVasche++;
  //  time_call = 0;  // ----->106
    Serial.print("nVasche/totVasche ritorno=");
    Serial.print(nVasche);
    Serial.print("/");
    Serial.println(totVasche);
    if (nVasche >= totVasche)
    {
      if (delay_repetition > 0)
      {
        time_call =0;
        delay_step = delay_repetition;
        if(istotVascheOdd)
                totVasche =totVasche -1;
      }
      //isRipRagg = true; // --->>104
      Serial.print("delay_repetition to undostep");
      Serial.println(delay_repetition);
    nVasche = 0;



      nRipetizioni++;
      Serial.print("nRipetizioni/totRip in undoStep");
      Serial.print(nRipetizioni);
      Serial.print("/");
      Serial.println(totRip);
      if (nRipetizioni == totRip)
      {
isRipRagg = true;
        if (delay_series > 0)
        { //Attendo delay_series
          time_call = 0;
          delay_step = delay_series;
        }
        nSerie++;
        Serial.print("delay_series to undostep");
        Serial.println(delay_series);
        Serial.print("nSerie undoStep/totSerie =");
        Serial.print(nSerie);
        Serial.print("/");
        Serial.println(totSerie);
        nRipetizioni = 0;
        //isSerieRagg = true;
      //  if (nSerie == tSecSerie) anche qui variabile sbagliata tSecSerie == tempo tot in sec serie
        if (nSerie == totSerie)
        { // QUI DEVE FERMARSI DEFINITIVAMENTE
          if (delay_series > 0)
          { //Attendo delay_series
            time_call = 0;
            Serial.print("Fine SERIE");
            Serial.println(nSerie);
            delay_step = delay_series * 10; // QUI DEVE FERMARSI DEFINITIVAMENTE

          }

          isSerieRagg = true;
>>>>>>> parent of 873365d... giusti commit
        }
      }
    }
<<<<<<< HEAD
=======
  }
}

unsigned int Swimmer::getNvasche()
{
  return nVasche;
>>>>>>> parent of 873365d... giusti commit
}

uint32_t Swimmer::getPos() {
    return pos;
}

void Swimmer::autoStep() {
    if (time_call >= delay_step) {
        if (isfirstTime)  // comporta il cambio di nVasche ed inserire isfirstTime = false alla fine di doStep dopo nVasche++
            doStep();


<<<<<<< HEAD
        else if (nVasche % 2 != 0)   //1  /// or  nVasche == 0 and nRipetizioni ==1
        {
            undoStep();
        } else {
            doStep();
        }
    } else {
        ++time_call;
    }
=======
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
    if(isfirstTime == true)  // comporta il cambio di nVasche ed inserire isfirstTime = false alla fine di doStep dopo nVasche++
    doStep();

    //if((nVasche != 0 && nVasche % 2 == 1 )|| (nVasche != 0 && nRipetizioni % 2 == 1)){
    //Serial.println("undoStep");
    //undoStep();
  //}
    // else
     //(nVasche != 0 && nVasche % 2 == 0);
     //{
      // Serial.println("doStep");
    //doStep();
  //}


                                // nVasche=1;
                                //   undoStep();
                               // }
                               // else{
                               //nVasche=1;
                               // doStep();
                               // }

  else  if (nVasche % 2 == 1)   //1  /// or  nVasche == 0 and nRipetizioni ==1
    {
    undoStep();
  }
  else
  {
    doStep();
  }
}
else{
++time_call;
}
}
void Swimmer::resetRip()
{ // reset della ripetizione 100m=4=nVasche
  nVasche = 0;

  isRipRagg = false;
>>>>>>> parent of 873365d... giusti commit
}

SwimmerColor Swimmer::getColors() {
    return colors;
}

