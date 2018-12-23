// DA VERIFICARE FLEXITIMER
#include <Arduino.h>
#include <Swimmer.h>
// #include <MsTimer2.h>   // eventualmente usare flextimer2
//https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&cad=rja&uact=8&ved=2ahUKEwjJwbeFg5DdAhXF16QKHS1RDbgQFjABegQICBAB&url=https%3A%2F%2Fgithub.com%2FPaulStoffregen%2FMsTimer2&usg=AOvVaw2z2-fxhAnNavVBQEHeO2DH
#include <FlexiTimer2.h>
#include <SPI.h>
#define PRINTLED(R,G,B) SPI.transfer(R);SPI.transfer(G);SPI.transfer(B);

// lungheza della striscia
#define LENSTRIP 164
#define RESOLUTION 1.0/10000
//  910=0,1 Millisecondi di chiamata passo per passo 15 sec ogni 25m =164
#define MCD 18 // 18= (15sec/25m =passo al metro x5m attuali=3sec /164 led= 18ms= passo reale per 5m
#define DECIMAL_TIMER 10
#define dStartSwimmer 30 //Pausa iniziale Ritarto  3 sec o 5 sec rispetto al primo
#define DELAY_START_SWIMMER_2 60 //Pausa iniziale Ritarto  3 sec o 5 sec rispetto al primo
#define DELAY_FOREACH_REPETITION 824 // = 15sec/passo 15/0.018 =824
#define DELAY_FOREACH_SERIES 3296 //Pausa per ogni serie di vasche =60/0.018=3296

//p_nLed=led segmento,p_pos=posizione led,p_step=step in avanti 1 2....,p_r=red ,p_g=green,p_b= blue, totvasche= numero vasche,totrip=numero ripetizioni,totSecSerie=numero serie,p_strip_length=lung striscia,p_delay_step=ritardo partenza 3,5sec,p_delay_repetition =REcupero ripetizione ,p_delay_series = Recupero serie

Swimmer s   (5, 0, 1, 255,  0,  0, 2, 2, 2, LENSTRIP, 0,DELAY_FOREACH_REPETITION,DELAY_FOREACH_SERIES);
Swimmer s2   (5, 0, 1, 0,  255,  0, 2, 2, 2, LENSTRIP, dStartSwimmer, DELAY_FOREACH_REPETITION,DELAY_FOREACH_SERIES);
Swimmer s3   (5, 0, 1, 0,  0,  255, 2 , 2, 2, LENSTRIP, DELAY_START_SWIMMER_2, DELAY_FOREACH_REPETITION,DELAY_FOREACH_SERIES);
volatile unsigned long count_times = 0;
volatile bool to_print = false; //Variabile ridondante che indica se è possibile chiamare nel loop la funzione printstrip()

//Funzione che gestisce i colori da applicare a ciascun led della striscia.
void printStrip(){
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  for (unsigned int i=0; i<LENSTRIP; i++){
      if (i>=s.getPos() && i<s.getNled()+s.getPos()){
        PRINTLED(255, 0, 0);
      }else if (i>=s2.getPos() && i<s2.getNled()+s2.getPos()){
        PRINTLED(255, 255, 255);
      }else if(i>=s3.getPos() && i<s3.getNled()+s3.getPos()){
        PRINTLED(255, 0, 255);
      }
      else{
        PRINTLED(0, 0, 0);
      }
  }
  SPI.endTransaction();
  delayMicroseconds(511);
}

void base_dei_tempi(){
  count_times++;
  if (count_times >= DECIMAL_TIMER && !to_print){
    s.autoStep();
    s2.autoStep();
    s3.autoStep();
    count_times = 0;
    to_print = true;
  }
}

void setup(){
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  SPI.begin();
  FlexiTimer2::set(MCD, RESOLUTION, base_dei_tempi);
  FlexiTimer2::start();
}

void loop(){
  if (to_print){
    printStrip();
    to_print = false;
  }
}
