// DA VERIFICARE FLEXITIMER
#include <Arduino.h>
#include <Swimmer.h>
// #include <MsTimer2.h>   // eventualmente usare flextimer2
//https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&cad=rja&uact=8&ved=2ahUKEwjJwbeFg5DdAhXF16QKHS1RDbgQFjABegQICBAB&url=https%3A%2F%2Fgithub.com%2FPaulStoffregen%2FMsTimer2&usg=AOvVaw2z2-fxhAnNavVBQEHeO2DH
#include <FlexiTimer2.h>
#include <SPI.h>
#define PRINTLED(R,G,B) SPI.transfer(R);SPI.transfer(G);SPI.transfer(B);

#define LENSTRIP 164 // lungheza della striscia
#define RESOLUTION 1.0/10000
#define MCD 910  //  910=0,1 Millisecondi di chiamata passo per passo 15 sec ogni 25m =164
#define DECIMAL_TIMER 1  //Ogni secondo autostep  3...
#define DELAY_SWIMMER_1 10
#define DELAY_SWIMMER_2 20

Swimmer s   (5, 0, 1, 255,  0,  0, 4, 2, 2, LENSTRIP, 0);
Swimmer s2   (3, 0, 1, 0,  255,  0, 4, 2, 2, LENSTRIP, DELAY_SWIMMER_1);
Swimmer s3   (3, 0, 1, 0,  0,  255, 4, 2, 2, LENSTRIP, DELAY_SWIMMER_2);
volatile unsigned long count_times = 0;
volatile bool to_print = false;

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
  delayMicroseconds(510);
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
