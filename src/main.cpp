#include "utility.h"
#include <Arduino.h>
#include <Swimmer.h>
#include <Keypad.h>
#include <SoftwareSerial.h>

#include <FlexiTimer2.h>
#include <SPI.h>

#define PRINTLED(R, G, B) \
  SPI.transfer(R);        \
  SPI.transfer(G);        \
  SPI.transfer(B);

#define LENSTRIP 800  // 32 led per metro
#define RESOLUTION 1.0 / 10000 // 0,1 ms


#define MCD 1 // PER RISOLUZIONE DI 0,1 ms


#define DELAY_START_SWIMMER 80       // 160 Pausa iniziale Ritarto  3 sec o 5 sec rispetto al primo
#define DELAY_START_SWIMMER_2 160    // 320 Pausa iniziale Ritarto  3 sec o 5 sec rispetto al primo
#define DELAY_START_SWIMMER_3  240    //Pausa iniziale Ritarto  3 sec o 5 sec rispetto al primo


#define Reset_AVR()      \
  wdt_enable(WDTO_30MS); \
  while (1)              \
  {                      \
  }
#define N_COMMANDS 11

bool is_inputTastiera = false;

uint32_t universal_decimal_timer = 0;


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



volatile unsigned long count_times = 0;
volatile bool to_print = false; //Variabile ridondante che indica se è possibile chiamare nel loop la funzione printstrip()
bool goSwimmer1 = false, goSwimmer2 = false, goSwimmer3 = false, goSwimmer4 = false;
int dStartSwimmer = 3;
int totVasche = 0;

int nSwimmer, totRipetizioni, totSerie;
int sSerie;
int mSerie;
int mRecupero;
int sRecupero;
int sAndatura;
int mAndatura;
int Distanza;
int tSecAndatura = 0;
int tSecSerie = 0;
int tSecRipetizioni = 0;


Swimmer s1; //(5, 0, 1, 255,   0,    0,totVasche, 2, 2, LENSTRIP, 0,tSecRipetizioni,tSecSerie);
Swimmer s2; //(5, 0, 1,   0, 255,    0,totVasche, 2, 2, LENSTRIP, DELAY_START_SWIMMER, tSecRipetizioni,tSecSerie);
Swimmer s3; //(5, 0, 1,   0,   0,  255,totVasche, 2, 2, LENSTRIP, DELAY_START_SWIMMER_2, tSecRipetizioni,tSecSerie);
Swimmer s4; //(5, 0, 1, 255,   0,  255,totVasche, 2, 2, LENSTRIP, DELAY_START_SWIMMER_3, tSecRipetizioni,tSecSerie);

void resetParams() { // reset parametri di input from 168 to184 main.cpp
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
    mSerie = 0;
    sSerie = 0;
    tSecSerie = 0;
} // End Reset resetParams

void printStrip() {
    SPIClass::beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    for (unsigned int i = 0; i < LENSTRIP; i++) {
        if (goSwimmer1 && i >= s1.getPos() && i < s1.getNled() + s1.getPos()) {
            PRINTLED(255, 0, 0);
        } else if (goSwimmer2 && i >= s2.getPos() && i < s2.getNled() + s2.getPos()) {
            PRINTLED(0, 255, 0);
        } else if (goSwimmer3 && i >= s3.getPos() && i < s3.getNled() + s3.getPos()) {
            PRINTLED(0, 0, 255);
        } else if (goSwimmer4 && i >= s4.getPos() && i < s4.getNled() + s4.getPos()) {
            PRINTLED(255, 0, 255);
        } else {
            PRINTLED(0, 0, 0);
        }
    }
    SPIClass::endTransaction();
    delayMicroseconds(505);
}

void base_dei_tempi() {
    count_times++;
    if (count_times >= universal_decimal_timer && !to_print) {
        s1.autoStep();
        s2.autoStep();
        s3.autoStep();
        s4.autoStep();
        count_times = 0;
        to_print = true;
    }
} //End Base dei tempi

void summaryInput() { //riepilogo input dati da bt e tastiera
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

void calculation() {
    totVasche = Distanza / 25;
    tSecAndatura = ((mAndatura * 60) + (sAndatura)) * 10000;// espresso in 0.1ms

    Serial.print("mRecupero = " + String(mRecupero));
    Serial.print("sRecupero = " + String(sRecupero));
    tSecRipetizioni = ((mRecupero * 60) + (sRecupero)) * 10000; // espresso in 0.1ms
    Serial.print("tSecRipetizioni = " + String(tSecRipetizioni));

    universal_decimal_timer = static_cast<uint32_t>(tSecAndatura / (totVasche * LENSTRIP));

    tSecRipetizioni = static_cast<int>(tSecRipetizioni / universal_decimal_timer);

    tSecSerie = (mSerie * 60);
    tSecSerie = (tSecSerie + sSerie) * 10000; // espresso in 0.1ms


    tSecSerie = static_cast<int>(tSecSerie / universal_decimal_timer);

} // end calculation

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
void setup() {

    Serial.begin(9600);
    pinMode(52, OUTPUT); // SCK Arduino mega



    Serial3.begin(38400); // Velocità di default del modulo HC-05
    lcd.init();           // initialize the lcd
    inputMessage();
    countReturn = 0;


    while (!is_inputTastiera) {
        char key = keypad.getKey();
        if (key != NO_KEY) //diverso da
        {
            if (key == 'C') { //Reset parametri
                resetParams();
                countReturn = 0;
                Val_Numerico = 0;
            } else if (key == '#') // Reset arduino
            {
                Reset_AVR();
            } else if (key == 'A') // input da tastiera
            {
                lcd.clear();
                Lcd_Length_String("Swimmer", 0, 0, false, nSwimmer);
                Lcd_Length_String("Delay to start", 0, 1, false, dStartSwimmer);
                Lcd_Length_String("Ripetitions", 0, 2, false, totRipetizioni);
                Lcd_Length_String("Series", 0, 3, false, totSerie);
            } else if (key == 'B') //input da Bluetooth
            {
                lcd.clear();
                Lcd_Length_String("  Bluetooth True", 1, 1, false, 0);
                Serial.println("aspetto " + String(N_COMMANDS) + " comandi.");
                String ricevute[N_COMMANDS];
                for (int i = 0; i < N_COMMANDS; i++) {
                    ricevute[i] = waitStringFromBt();
                    Serial.println("Mancano : " + String(N_COMMANDS - i - 1) + " comandi");
                }
                Serial.println("fine 10 comandi e stampo a video prima e seconda parte di tutti.");
                for (const auto &s : ricevute) {
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
                Serial.print("totVasche=" + String(totVasche));
                Serial.println(totVasche);
                Serial.print("crociera in sec");
                Serial.println(tSecAndatura);
                Serial.print("Recupero in sec");
                Serial.println(String(tSecRipetizioni));
                Serial.print("Recuperoserie in sec");
                Serial.println(String(tSecSerie));
                Serial.println("fine stampa_da bt");
                is_inputTastiera = true; // disabilita tastiera


                bool is_pressed = false;
                while (!is_pressed) { // finchè è falso
                    Serial.print("loop to Run" + String(is_pressed));
                    String rec = waitStringFromBt();
                    Serial.print(rec);
                    String comando = getMeString(rec);
                    is_pressed = comando.equalsIgnoreCase("run");
                }
            } else if (key == '*') // ho premuto premuto * Return
            {
                countReturn++; // conta quante volte ho premuto *
                switch (countReturn) {
                    case 1: // Num Swimmer
                        nSwimmer = Val_Numerico;
                        if ((nSwimmer < 10) &&
                            (nSwimmer >= 1)) // Limite delle ripetizioni minore di 10 and maggiore di 1
                        {
                            Lcd_Length_String("Swimmer", 0, 0, true, nSwimmer);              //1
                            Lcd_Length_String("Delay to start", 0, 1, false, dStartSwimmer); //2
                            Lcd_Length_String("Ripetitions", 0, 2, false, totRipetizioni);     //3
                            Lcd_Length_String("Series", 0, 3, false, totSerie);                //4
                            Val_Numerico = 0;                                                // a zero per preparare il nuovo valore
                        } else {
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
                        if ((dStartSwimmer < 10) && (dStartSwimmer >= 0)) {
                            Lcd_Length_String("Swimmer", 0, 0, true, nSwimmer);             //1
                            Lcd_Length_String("Delay to start", 0, 1, true, dStartSwimmer); //2
                            Lcd_Length_String("Series", 0, 3, false, totSerie);               //4
                            Lcd_Length_String("Ripetitions", 0, 2, false, totRipetizioni);    //3
                            Val_Numerico = 0;                                               // a zero per preparare il nuovo valore
                        } else {
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
                        if ((totRipetizioni < 50) &&
                            (totRipetizioni >= 0)) // Limite delle ripetizioni minore di 50 and maggiore di 1
                        {
                            Lcd_Length_String("Swimmer", 0, 0, true, nSwimmer);
                            Lcd_Length_String("Delay to start", 0, 1, true, dStartSwimmer);
                            Lcd_Length_String("Ripetitions", 0, 2, true, totRipetizioni);
                            Lcd_Length_String("Series", 0, 3, false, totSerie);
                            Val_Numerico = 0; // a zero per preparare il nuovo valore
                        } else {
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

                        if ((totSerie < 10) &&
                            (totSerie >= 1)) // Limite delle ripetizioni minore di 10 and maggiore di 1
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
                        } else {
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
                        if ((Distanza < 2000) &&
                            (Distanza >= 25)) // Limite della Distanza minore di 10 and maggiore di 1
                        {
                            Lcd_Length_String("Distance", 0, 0, true, Distanza);
                            Lcd_Length_String("Cruise in min.", 0, 1, false, mAndatura);
                            Lcd_Length_String("Cruise in sec.", 0, 2, false, sAndatura);
                            Lcd_Length_String("Ricovery min.", 0, 3, false, mRecupero);

                            Val_Numerico = 0; // azzero per preparare il nuovo valore
                        } else {
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

                        } else {
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

                        } else {
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

                            delay(2000);
                            lcd.clear();
                            Lcd_Length_String("Ric. seconds", 0, 0, false, sRecupero);
                            Lcd_Length_String("Ric. Series min", 0, 1, false, sSerie);
                            Lcd_Length_String("Ric. Series sec", 0, 2, false, mSerie);
                        } else {
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

                        } else {
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

                        } else {
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

                        } else {
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
                        Serial.print("totVasche=" + String(totVasche));
                        Serial.println(totVasche);
                        Serial.print("crociera in sec");
                        Serial.println(tSecAndatura);
                        Serial.print("Recupero in sec");
                        Serial.println(String(tSecRipetizioni));
                        Serial.print("Recuperoserie in sec");
                        Serial.println(String(tSecSerie));
                        Serial.println("fine stampa_da kpad");
                    default:
                        break;
                }
            } else {
                Val_Numerico = Val_Numerico * 10 + (key - '0');
            }
        } // fine tastiera
        if (countReturn == 13) {
            is_inputTastiera = true;
            countReturn = 0;
        }
    } //while Gestione Tastiera



    SPIClass::begin();

    FlexiTimer2::set(MCD, RESOLUTION, base_dei_tempi);
    FlexiTimer2::start();

    for (uint8_t i = 1; i <= nSwimmer; i++) { // gestione nSwimmer
        if (i == 1) {
            goSwimmer1 = true;
        } else if (i == 2) {
            goSwimmer2 = true;
        } else if (i == 3) {
            goSwimmer3 = true;
        } else if (i == 4) {
            goSwimmer4 = true;
        }
    } //End nSwimmer attivi
    Serial.print("totVasche=" + String(totVasche));
    Serial.println(totVasche);
    Serial.print("crociera in sec");
    Serial.println(tSecAndatura);
    Serial.print("Recupero in sec");
    Serial.println(String(tSecRipetizioni));
    Serial.print("Recuperoserie in sec");
    Serial.println(String(tSecSerie));

    s1.init(5, 0, 1, 255, 0, 0, static_cast<unsigned int>(totVasche), static_cast<unsigned int>(totRipetizioni),
            static_cast<unsigned int>(totSerie), LENSTRIP, 0, static_cast<unsigned long>(tSecRipetizioni),
            static_cast<unsigned long>(tSecSerie));
    s2.init(5, 0, 1, 0, 255, 0, static_cast<unsigned int>(totVasche), static_cast<unsigned int>(totRipetizioni),
            static_cast<unsigned int>(totSerie), LENSTRIP, DELAY_START_SWIMMER,
            static_cast<unsigned long>(tSecRipetizioni),
            static_cast<unsigned long>(tSecSerie));
    s3.init(5, 0, 1, 0, 0, 255, static_cast<unsigned int>(totVasche), static_cast<unsigned int>(totRipetizioni),
            static_cast<unsigned int>(totSerie), LENSTRIP, DELAY_START_SWIMMER_2,
            static_cast<unsigned long>(tSecRipetizioni),
            static_cast<unsigned long>(tSecSerie));
    s4.init(5, 0, 1, 255, 0, 255, static_cast<unsigned int>(totVasche), static_cast<unsigned int>(totRipetizioni),
            static_cast<unsigned int>(totSerie), LENSTRIP, DELAY_START_SWIMMER_3,
            static_cast<unsigned long>(tSecRipetizioni),
            static_cast<unsigned long>(tSecSerie));
}
#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
void loop() {
    if (to_print) {
        printStrip();
        to_print = false;
    }
}
#pragma clang diagnostic pop
