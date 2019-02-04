#include "utility.h"
#include <Arduino.h>
#include <Swimmer.h>
// #include <MsTimer2.h>   // eventualmente usare flextimer2
//https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&cad=rja&uact=8&ved=2ahUKEwjJwbeFg5DdAhXF16QKHS1RDbgQFjABegQICBAB&url=https%3A%2F%2Fgithub.com%2FPaulStoffregen%2FMsTimer2&usg=AOvVaw2z2-fxhAnNavVBQEHeO2DH
#include <FlexiTimer2.h>
#include <SPI.h>
#define PRINTLED(R, G, B) \
  SPI.transfer(R);        \
  SPI.transfer(G);        \
  SPI.transfer(B);

// lungheza della striscia
#define LENSTRIP 160  // 32 led per metro
#define RESOLUTION 1.0 / 10000 // 0,1 ms
//  910=0,1 Millisecondi di chiamata passo per passo 15 sec ogni 25m =164
//#define MCD 18 // 18= (15sec/25m =passo al metro x5m attuali=3sec /164 led= 18ms= passo reale per 5m
#define MCD 1 // PER RISOLUZIONE DI 0,1 ms
//#define DECIMAL_TIMER 10
#define DECIMAL_TIMER 180//  PASSO PER AVER VELOCITA DI 15SEC SU 25M con 800 non va in errore 
#define DELAY_START_SWIMMER 30        //Pausa iniziale Ritarto  3 sec o 5 sec rispetto al primo
#define DELAY_START_SWIMMER_2 60      //Pausa iniziale Ritarto  3 sec o 5 sec rispetto al primo
#define DELAY_START_SWIMMER_3 90      //Pausa iniziale Ritarto  3 sec o 5 sec rispetto al primo
//#define tSecRipetizioni 3824 // = 15sec/passo 15/0.018 =824
//#define tSecSerie 3296     //Pausa per ogni serie di vasche =60/0.018=3296

// Input da Tastiera e Bluetooth \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
  // #include "utility.h"

/////traslatore di livello
// LV = +3.3v
// HV = +5v
// xxO STA PER 5V
// xxI STA PER 3.3V
// TXI = TX BlueTooth
// RXI = RX BlueTooth
// TXO = RX Arduino
// RXO = TX Arduino
// FINE TRASLATORE DI livello

#define Reset_AVR()      \
  wdt_enable(WDTO_30MS); \
  while (1)              \
  {                      \
  }
#define N_COMMANDS 11

bool is_inputTastiera = false;

#include <SoftwareSerial.h>
// #define BT_TX_PIN 14  // 11  x arduino 1 // Per Arduino 1
//#define BT_RX_PIN 15  // 10 per Arduino 1// Per Arduino 1

// Gestione BlueTooth
//SoftwareSerial bt(BT_RX_PIN, BT_TX_PIN); // Per Arduino 1
String s_ricevuta = "";
bool fine_ricezione_stringa = false;
String prima_parte;
int seconda_parte;
byte k = 0; // conto quanti ";" arrivano  x deteminare la fine immissione dati
// fine gestione bluetooth//

// input tastiera
#include <Keypad.h>
int countReturn;
const byte ROWS = 4; //4 righe
const byte COLS = 4; //4 colonne
char keys[ROWS][COLS] =
    {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}};
byte colPins[COLS] = {37, 36, 35, 34}; // Pin di arduino connessi ai pin 5,6,7,8 delle righe del keypad
byte rowPins[ROWS] = {41, 40, 39, 38}; // Pin di arduino connessi ai pin 5,6,7,8 delle righe del keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
int Val_Numerico = 0; //variabile di costruzione valore unmerico da associare alla variabile totRipetizioni.....Sec_Recuper
/*   KeyPad=         1 -2 -3 -4 -5 -6 -7 -8
  arduino_1_   PIN=  9 -8 -7 -6 -5 -4 -3 -2
  arduino Mega PIN= 22-23-24-25-26-27-28-29
  */
// fine gestione tastiera
// End Input da Tastiera e Bluetooth \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//Swimmers...........
volatile unsigned long count_times = 0;
volatile bool to_print = false; //Variabile ridondante che indica se è possibile chiamare nel loop la funzione printstrip()
bool goSwimmer1 = false, goSwimmer2 = false, goSwimmer3 = false, goSwimmer4 = false;
uint8_t tPartenza = 0, totVasche = 0, dStartSwimmer = 3;
uint8_t nSwimmer, totRipetizioni, totSerie, mAndatura, sAndatura, mRecupero, sRecupero, mSerie, sSerie;
int Distanza, lVasca, tSecRipetizioni = 0, tMinRecupero = 0, tSecSerie = 0, tSecAndatura = 0;
// variabili nascoste su utility
// swimmer from 94 to 146 main.cpp
//p_nLed=led segmento,p_pos=posizione led,p_step=step in avanti 1 2....,p_r=red ,p_g=green,p_b= blue, p_totVasche= numero vasche,tSecSerie=numero serie,p_strip_length=lung striscia,p_delay_step=ritardo partenza 3,5sec,p_delay_repetition =REcupero ripetizione ,p_delay_series = Recupero serie
Swimmer s1; //(5, 0, 1, 255,   0,    0,totVasche, 2, 2, LENSTRIP, 0,tSecRipetizioni,tSecSerie);
Swimmer s2; //(5, 0, 1,   0, 255,    0,totVasche, 2, 2, LENSTRIP, DELAY_START_SWIMMER, tSecRipetizioni,tSecSerie);
Swimmer s3; //(5, 0, 1,   0,   0,  255,totVasche, 2, 2, LENSTRIP, DELAY_START_SWIMMER_2, tSecRipetizioni,tSecSerie);
Swimmer s4; //(5, 0, 1, 255,   0,  255,totVasche, 2, 2, LENSTRIP, DELAY_START_SWIMMER_3, tSecRipetizioni,tSecSerie);

void resetParams()
{ // reset parametri di input from 168 to184 main.cpp
  nSwimmer = 1;
  dStartSwimmer = 0;
  totRipetizioni = 0;
  totSerie = 0;
  Distanza = 0;
  mAndatura = 0;
  sAndatura = 0;
  tSecAndatura = 0;
  mRecupero = 0;
  sRecupero = 0;
  tSecRipetizioni = 0;
  tMinRecupero = 0;
  mSerie = 0;
  sSerie = 0;
  tSecSerie = 0;
} // End Reset resetParams

//Funzione che gestisce i colori da applicare a ciascun led della striscia.  line 118
void printStrip()
{
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  for (unsigned int i = 0; i < LENSTRIP; i++)
  {
    if (goSwimmer1 && i >= s1.getPos() && i < s1.getNled() + s1.getPos())
    {
      PRINTLED(255, 0, 0);
    }
    else if (goSwimmer2 && i >= s2.getPos() && i < s2.getNled() + s2.getPos())
    {
      PRINTLED(0, 255, 0);
    }
    else if (goSwimmer3 && i >= s3.getPos() && i < s3.getNled() + s3.getPos())
    {
      PRINTLED(0, 0, 255);
    }
    else if (goSwimmer4 && i >= s4.getPos() && i < s4.getNled() + s4.getPos())
    {
      PRINTLED(255, 0, 255);
    }
    else
    {
      PRINTLED(0, 0, 0);
    }
  }
  SPI.endTransaction();
  delayMicroseconds(511);
}

void base_dei_tempi()
{
  count_times++;
  if (count_times >= DECIMAL_TIMER && !to_print)
  {
    s1.autoStep();
    s2.autoStep();
    s3.autoStep();
    s4.autoStep();
    count_times = 0;
    to_print = true;
  }
} //End Base dei tempi

void summaryInput()
{ //riepilogo input dati da bt e tastiera
  lcd.clear();
  Lcd_Length_String("Riepilogo", 0, 0, false, 0);
  delay(1500);
  Lcd_Length_String("Swimmer", 0, 0, true, nSwimmer);

  Lcd_Length_String("Delay to start", 0, 1, true, dStartSwimmer);
  Lcd_Length_String("Ripetitions", 0, 2, true, totRipetizioni);
  Lcd_Length_String("Series", 0, 3, true, totSerie);
  delay(1500);
  lcd.clear();
  Lcd_Length_String("Distance", 0, 0, true, Distanza);
  Lcd_Length_String("Cruise in min.", 0, 1, true, mAndatura);
  Lcd_Length_String("Cruise in sec.", 0, 2, true, sAndatura);
  delay(1500);
  lcd.clear();
  Lcd_Length_String("Ricovery min.", 0, 0, true, mRecupero);
  Lcd_Length_String("Ric. seconds", 0, 1, true, sRecupero);
  Lcd_Length_String("Ric. Series min", 0, 2, true, mSerie);
  Lcd_Length_String("Ric. Series sec", 0, 3, true, sSerie);

  delay(1000);
  lcd.clear();
  Lcd_Length_String("Press * to Go!", 0, 3, true, 0);

} // end riepilogo dati

void calculation()
{
  totVasche = Distanza / 25;

  tSecAndatura = (mAndatura * 60);
  tSecAndatura = tSecAndatura + (sAndatura);
  tSecRipetizioni = (mRecupero * 60);
  tSecRipetizioni = tSecRipetizioni + (sRecupero);
  tSecRipetizioni = tSecRipetizioni * 18 *3;
  tSecSerie = (mSerie * 60);
  tSecSerie = (tSecSerie + sSerie);
  tSecSerie = tSecSerie *18*3;
} // end calculation

void setup()
{

  Serial.begin(9600);
  pinMode(52, OUTPUT); // SCK Arduino mega
  //pinMode(13,OUTPUT); // SCK arduino 1

  // void setup da Input Tastiera & BlueTooth ////////////////////////////////////////

  //bt.begin(38400); // Per Arduino 1
  Serial3.begin(38400); // Velocità di default del modulo HC-05
  lcd.init();           // initialize the lcd
  inputMessage();
  countReturn = 0;

  //  else{ // del BlueTooth

  while (!is_inputTastiera)
  {
    char key = keypad.getKey();
    if (key != NO_KEY) //diverso da
    {
      if (key == 'C')
      { //Reset parametri
        resetParams();
        countReturn = 0;
        Val_Numerico = 0;
      }
      else if (key == '#') // Reset arduino
      {
        Reset_AVR();
      }
      else if (key == 'A') // input da tastiera
      {
        lcd.clear();
        Lcd_Length_String("Swimmer", 0, 0, false, nSwimmer);
        Lcd_Length_String("Delay to start", 0, 1, false, dStartSwimmer);
        Lcd_Length_String("Ripetitions", 0, 2, false, totRipetizioni);
        Lcd_Length_String("Series", 0, 3, false, totSerie);
      }

      else if (key == 'B') //input da Bluetooth
      {
        lcd.clear();
        Lcd_Length_String("  Bluetooth True", 1, 1, false, 0);
        Serial.println("aspetto " + String(N_COMMANDS) + " comandi.");
        String ricevute[N_COMMANDS];
        for (int i = 0; i < N_COMMANDS; i++)
        {
          ricevute[i] = waitStringFromBt();
          Serial.println("Mancano : " + String(N_COMMANDS - i - 1) + " comandi");
        }
        Serial.println("fine 10 comandi e stampo a video prima e seconda parte di tutti.");
        for (int i = 0; i < 11; i++)
        {
          String s = ricevute[i];
          String prima_parte = getMeString(s);
          int seconda_parte = getMeValue(s);
          Serial.println(prima_parte);
          Serial.println(seconda_parte);
          if (prima_parte == "nSwimmer")
            nSwimmer = seconda_parte;
          else if (prima_parte == "dStartSwimmer")
            dStartSwimmer = seconda_parte;
          else if (prima_parte == "totRipetizioni")
            totRipetizioni = seconda_parte;
          else if (prima_parte == "totSerie")
            totSerie = seconda_parte;
          else if (prima_parte == "Distanza")
            Distanza = seconda_parte;
          else if (prima_parte == "mAndatura")
            mAndatura = seconda_parte;
          else if (prima_parte == "sAndatura")
            sAndatura = seconda_parte;
          else if (prima_parte == "mRecupero")
            mRecupero = seconda_parte;
          else if (prima_parte == "sRecupero")
            sRecupero = seconda_parte;
          else if (prima_parte == "mSerie")
            mSerie = seconda_parte;
          else if (prima_parte == "sSerie")
            sSerie = seconda_parte;
        }               // end input BlueTooth
        summaryInput(); //riepilogo input dati
        calculation();
        Serial.print("totVasche=" + totVasche);
        Serial.println(totVasche);
        Serial.print("crociera in sec");
        Serial.println(tSecAndatura);
        Serial.print("Recupero in sec");
        Serial.println(tSecRipetizioni);
        Serial.print("Recuperoserie in sec");
        Serial.println(tSecSerie);
        Serial.println("fine stampa_da bt");
        is_inputTastiera = true; // disabilita tastiera
        //Serial.println ("Bluetooth attivo");

        ////  funzione Run
        bool is_pressed = false;
        while (!is_pressed)
        { // finchè è falso
          Serial.print("loop to Run" + String(is_pressed));
          String rec = waitStringFromBt();
          Serial.print(rec);
          String comando = getMeString(rec);
          is_pressed = comando.equalsIgnoreCase("run");
        }
      }
      else if (key == '*') // ho premuto premuto * Return
      {
        countReturn++; // conta quante volte ho premuto *
        switch (countReturn)
        {
        case 1: // Num Swimmer
          nSwimmer = Val_Numerico;
          if ((nSwimmer < 10) && (nSwimmer >= 1)) // Limite delle ripetizioni minore di 10 and maggiore di 1
          {
            Lcd_Length_String("Swimmer", 0, 0, true, nSwimmer);              //1
            Lcd_Length_String("Delay to start", 0, 1, false, dStartSwimmer); //2
            Lcd_Length_String("Ripetitions", 0, 2, false, totRipetizioni);     //3
            Lcd_Length_String("Series", 0, 3, false, totSerie);                //4
            Val_Numerico = 0;                                                // a zero per preparare il nuovo valore
          }
          else
          {
            Lcd_Length_String("Out of range", 0, 0, false, nSwimmer);
            delay(2000);
            Lcd_Length_String("Range >1 o <10", 0, 0, false, nSwimmer);
            delay(2000);
            Lcd_Length_String("Swimmer", 0, 0, false, nSwimmer);
            Val_Numerico = 0;
            countReturn--;
          }
          break;
        case 2: // delay partenza swimmwer
          dStartSwimmer = Val_Numerico;
          if ((dStartSwimmer < 10) && (dStartSwimmer >= 0))
          {
            Lcd_Length_String("Swimmer", 0, 0, true, nSwimmer);             //1
            Lcd_Length_String("Delay to start", 0, 1, true, dStartSwimmer); //2
            Lcd_Length_String("Series", 0, 3, false, totSerie);               //4
            Lcd_Length_String("Ripetitions", 0, 2, false, totRipetizioni);    //3
            Val_Numerico = 0;                                               // a zero per preparare il nuovo valore
          }
          else
          {
            Lcd_Length_String("Out of range", 0, 1, false, dStartSwimmer);
            delay(2000);
            Lcd_Length_String("Range <10", 0, 1, false, dStartSwimmer);
            delay(2000);
            Lcd_Length_String("Delay to start", 0, 1, false, dStartSwimmer);
            Val_Numerico = 0;
            countReturn--;
          }
          break;
        case 3: // num di ripetizioni
          totRipetizioni = Val_Numerico;
          if ((totRipetizioni < 50) && (totRipetizioni >= 0)) // Limite delle ripetizioni minore di 50 and maggiore di 1
          {
            Lcd_Length_String("Swimmer", 0, 0, true, nSwimmer);
            Lcd_Length_String("Delay to start", 0, 1, true, dStartSwimmer);
            Lcd_Length_String("Ripetitions", 0, 2, true, totRipetizioni);
            Lcd_Length_String("Series", 0, 3, false, totSerie);
            Val_Numerico = 0; // a zero per preparare il nuovo valore
          }
          else
          {
            Lcd_Length_String("Out of Range", 0, 2, false, totRipetizioni);
            delay(2000);
            Lcd_Length_String("Range <1 <50", 0, 2, false, totRipetizioni);
            delay(2000);
            Lcd_Length_String("Ripetitions", 0, 2, false, totRipetizioni);
            Val_Numerico = 0;
            countReturn--;
          }
          break;
        case 4: //num di totSerie

          totSerie = Val_Numerico;
          // if ( (totSerie<10) && (totSerie>=1)  && (countReturn==2))  // Limite delle ripetizioni minore di 10 and maggiore di 1
          if ((totSerie < 10) && (totSerie >= 1)) // Limite delle ripetizioni minore di 10 and maggiore di 1
          {
            Lcd_Length_String("Swimmer", 0, 0, true, nSwimmer);
            Lcd_Length_String("Delay to start", 0, 1, true, dStartSwimmer);
            Lcd_Length_String("Ripetitions", 0, 2, true, totRipetizioni);
            Lcd_Length_String("Series", 0, 3, true, totSerie);
            Val_Numerico = 0; // azzero per preparare il nuovo valore
            delay(1000);
            lcd.clear();
            Lcd_Length_String("Distance", 0, 0, false, Distanza);
            Lcd_Length_String("Cruise in min.", 0, 1, false, mAndatura);
            Lcd_Length_String("Cruise in sec.", 0, 2, false, sAndatura);
            Lcd_Length_String("Ricovery min.", 0, 3, false, mRecupero);
          }
          else
          {
            Lcd_Length_String("Out of Range", 0, 3, false, totSerie);
            delay(2000);
            Lcd_Length_String("Range <1 <10", 0, 3, false, totSerie);
            delay(2000);
            Lcd_Length_String("Series", 0, 3, false, totSerie);
            Val_Numerico = 0;
            countReturn--;
          }
          break;
        case 5: // Distanza
          Distanza = Val_Numerico;
          if ((Distanza < 2000) && (Distanza >= 25)) // Limite della Distanza minore di 10 and maggiore di 1
          {
            Lcd_Length_String("Distance", 0, 0, true, Distanza);
            Lcd_Length_String("Cruise in min.", 0, 1, false, mAndatura);
            Lcd_Length_String("Cruise in sec.", 0, 2, false, sAndatura);
            Lcd_Length_String("Ricovery min.", 0, 3, false, mRecupero);
            // totVasche=Distanza/25;
            Val_Numerico = 0; // azzero per preparare il nuovo valore
          }
          else
          {
            Lcd_Length_String("Out of Range", 0, 0, false, Distanza);
            delay(2000);
            Lcd_Length_String("Range >25 <2000", 0, 0, false, Distanza);
            delay(2000);
            Lcd_Length_String("Multiple of 25", 0, 0, false, Distanza);
            delay(2000);
            Lcd_Length_String("Distance", 0, 0, false, Distanza);
            Val_Numerico = 0;
            countReturn--;
            Distanza = 0;
          }
          break;
        case 6: //input Minuti crociera
          mAndatura = Val_Numerico;
          if ((mAndatura < 35)) // Limite dei minuti di Crociera
          {
            Lcd_Length_String("Cruise in sec.", 0, 2, false, sAndatura);
            Lcd_Length_String("Ricovery min.", 0, 3, false, mRecupero);
            Lcd_Length_String("Distance", 0, 0, true, Distanza);
            Lcd_Length_String("Cruise in min.", 0, 1, true, mAndatura);
            Val_Numerico = 0; // azzero per preparare il nuovo valore
            // tSecAndatura=(mAndatura*60);
          }
          else
          {
            Lcd_Length_String("Out of Range", 0, 1, false, mAndatura);
            delay(2000);
            Lcd_Length_String("Range >35 min", 0, 1, false, mAndatura);
            delay(2000);
            Lcd_Length_String("Cruise in min.", 0, 1, false, mAndatura);
            Val_Numerico = 0;
            countReturn--;
            mAndatura = 0;
          }
          break;
        case 7: //input Secondi crociera
          sAndatura = Val_Numerico;
          if ((sAndatura < 59)) // Limite dei minuti di Crociera
          {
            Lcd_Length_String("Ricovery min.", 0, 3, false, mRecupero);
            Lcd_Length_String("Series", 0, 0, true, Distanza);
            Lcd_Length_String("Cruise in min.", 0, 1, true, mAndatura);
            Lcd_Length_String("Cruise in sec.", 0, 2, true, sAndatura);
            Val_Numerico = 0; // azzero per preparare il nuovo valore
            // tSecAndatura=tSecAndatura+(sAndatura);
          }
          else
          {
            Lcd_Length_String("Out of Range", 0, 2, false, sAndatura);
            delay(2000);
            Lcd_Length_String("Range <59 sec", 0, 2, false, sAndatura);
            delay(2000);
            Lcd_Length_String("Cruise in sec.", 0, 2, false, sAndatura);
            Val_Numerico = 0;
            countReturn--;
            sAndatura = 0;
          }
          break;
        case 8: //input Minuti Recupero
          mRecupero = Val_Numerico;
          if ((mRecupero < 10)) // Limite dei minuti di Recupero
          {
            Lcd_Length_String("Series", 0, 0, true, Distanza);
            Lcd_Length_String("Cruise in min.", 0, 1, true, mAndatura);
            Lcd_Length_String("Cruise in sec.", 0, 2, true, sAndatura);
            Lcd_Length_String("Ricovery min.", 0, 3, true, mRecupero);
            Val_Numerico = 0; // azzero per preparare il nuovo valore
            // tSecRipetizioni=(mRecupero*60);
            delay(2000);
            lcd.clear();
            Lcd_Length_String("Ric. seconds", 0, 0, false, sRecupero);
            Lcd_Length_String("Ric. Series min", 0, 1, false, sSerie);
            Lcd_Length_String("Ric. Series sec", 0, 2, false, mSerie);
          }
          else
          {
            Lcd_Length_String("Out of Range", 0, 3, false, mRecupero);
            delay(2000);
            Lcd_Length_String("Range <10 sec", 0, 3, false, mRecupero);
            delay(2000);
            Lcd_Length_String("Ricovery min.", 0, 3, false, mRecupero);
            Val_Numerico = 0;
            countReturn--;
            mRecupero = 0;
          }
          break;
        case 9: //input Secondi Recupero
          sRecupero = Val_Numerico;
          if ((sRecupero < 59)) // Limite dei minuti di Recupero
          {
            Lcd_Length_String("Ric. seconds", 0, 0, true, sRecupero);
            Lcd_Length_String("Ric.Series min.", 0, 1, false, mSerie);
            Lcd_Length_String("Ric.Series sec.", 0, 2, false, sSerie);
            Val_Numerico = 0; // azzero per preparare il nuovo valore
            // tSecRipetizioni=tSecRipetizioni+(sRecupero);
          }
          else
          {
            Lcd_Length_String("Out of Range", 0, 0, false, sRecupero);
            delay(2000);
            Lcd_Length_String("Range <59 sec", 0, 0, false, sRecupero);
            delay(2000);
            Lcd_Length_String("Ricovery sec.", 0, 0, false, sRecupero);
            Val_Numerico = 0;
            countReturn--;
            sRecupero = 0;
          }
          break;
        case 10: //input minuti Recupero totSerie
          mSerie = Val_Numerico;
          if ((mSerie < 10)) // Limite dei minuti di Recupero
          {
            Lcd_Length_String("Ric. seconds", 0, 0, true, sRecupero);
            Lcd_Length_String("Ric. Series min", 0, 1, true, mSerie);
            Lcd_Length_String("Ric. Series sec", 0, 2, false, sSerie);
            Val_Numerico = 0; // azzero per preparare il nuovo valore
            // tSecSerie=(mSerie*60);
          }
          else
          {
            Lcd_Length_String("Out of range", 0, 1, false, mSerie);
            delay(2000);
            Lcd_Length_String("Range <10 min.", 0, 1, false, mSerie);
            delay(2000);
            Lcd_Length_String("Ricovery min.", 0, 1, false, mSerie);
            Val_Numerico = 0;
            countReturn--;
            mSerie = 0;
          }
          break;
        case 11: // recupero serie in sec
          sSerie = Val_Numerico;
          if ((sSerie < 60)) // Limite dei minuti di Recupero
          {
            Lcd_Length_String("Ric. seconds", 0, 0, true, sRecupero);
            Lcd_Length_String("Ric. Series min", 0, 1, true, mSerie);
            Lcd_Length_String("Ric. Series sec", 0, 2, true, sSerie);
            Lcd_Length_String("Press * to go on", 0, 3, false, "on");
            delay(1000);
            Val_Numerico = 0; // azzero per preparare il nuovo valore
            //tSecSerie=(tSecSerie + sSerie);
          }
          else
          {
            Lcd_Length_String("Out of Range", 0, 2, false, sSerie);
            delay(2000);
            Lcd_Length_String("Range <59 sec.", 0, 2, false, sSerie);
            delay(2000);
            Lcd_Length_String("Ricovery sec.", 0, 2, false, sSerie);
            Val_Numerico = 0;
            countReturn--;
            sSerie = 0;
          }
          break;
        case 12:
          summaryInput(); //riepilogo input dati
          calculation();  // calcola totVasche tSecAndatura,tSecRipetizioni,tSecSerie
          Serial.print("totVasche=" + totVasche);
          Serial.println(totVasche);
          Serial.print("crociera in sec");
          Serial.println(tSecAndatura);
          Serial.print("Recupero in sec");
          Serial.println(tSecRipetizioni);
          Serial.print("Recuperoserie in sec");
          Serial.println(tSecSerie);
          Serial.println("fine stampa_da kpad");
        default:
          break;
        }
      }
      else
      {
        Val_Numerico = Val_Numerico * 10 + (key - '0');
      }
    } // fine tastiera
    if (countReturn == 13)
    {
      is_inputTastiera = true;
      countReturn = 0;
    }
  } //while Gestione Tastiera

  // End void setup da input Tastiera & BlueTooth////////////////////////////////////

  //End

  SPI.begin();
  // flextimer2:: set....
  FlexiTimer2::set(MCD, RESOLUTION, base_dei_tempi);
  FlexiTimer2::start();

  for (uint8_t i = 1; i <= nSwimmer; i++)
  { // gestione nSwimmer
    if (i == 1)
    {
      goSwimmer1 = true;
    }
    else if (i == 2)
    {
      goSwimmer2 = true;
    }
    else if (i == 3)
    {
      goSwimmer3 = true;
    }
    else if (i == 4)
    {
      goSwimmer4 = true;
    }
  } //End nSwimmer attivi
  Serial.print("totVasche=" + totVasche);
  Serial.println(totVasche);
  Serial.print("crociera in sec");
  Serial.println(tSecAndatura);
  Serial.print("Recupero in sec");
  Serial.println(tSecRipetizioni);
  Serial.print("Recuperoserie in sec");
  Serial.println(tSecSerie);
  //Qui inserisci al posto dei numeri le variabili con i valori.
  s1.init(5, 0, 1, 255, 0, 0, totVasche, totRipetizioni, totSerie, LENSTRIP, 0, tSecRipetizioni, tSecSerie);
  s2.init(5, 0, 1, 0, 255, 0, totVasche, totRipetizioni, totSerie, LENSTRIP, DELAY_START_SWIMMER, tSecRipetizioni, tSecSerie);
  s3.init(5, 0, 1, 0, 0, 255, totVasche, totRipetizioni, totSerie, LENSTRIP, DELAY_START_SWIMMER_2, tSecRipetizioni, tSecSerie);
  s4.init(5, 0, 1, 255, 0, 255, totVasche, totRipetizioni, totSerie, LENSTRIP, DELAY_START_SWIMMER_3, tSecRipetizioni, tSecSerie);
}

void loop()
{
  if (to_print)
  {
    printStrip();
    to_print = false;
  }
}
