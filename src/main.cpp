
#include <Arduino.h>
#include <Swimmer.h>
#include <MsTimer2.h>   // eventualmente usare flextimer2
//https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&cad=rja&uact=8&ved=2ahUKEwjJwbeFg5DdAhXF16QKHS1RDbgQFjABegQICBAB&url=https%3A%2F%2Fgithub.com%2FPaulStoffregen%2FMsTimer2&usg=AOvVaw2z2-fxhAnNavVBQEHeO2DH
#define LENSTRIP 164
#define MCD 1//Millisecondi di chiamata
#define TIMER1 10  //Ogni secondo autostep  3...

//Adafruit_NeoPixel * strip = new Adafruit_NeoPixel(299, 30, NEO_GRB + NEO_KHZ800);
// cambiando da 5 a 10 il timer1 non cambia per un giro completo ci mette 4"40 circa

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row ,Datapin,clockpin
SPIStrip *strip = new SPIStrip(LENSTRIP);

//  inizializza la classe Adafruit_NeoPixel passandole il numero di led, il pin a cui è connesso
// connessa la striscia di neopixel ed un parametro che indica alla libreria come comunicare con
// la striscia led;
// Adafruit_NeoPixel * strip = new Adafruit_NeoPixel(299,30,NEO_GRB + NEO_KHZ800);
// Adafruit_NeoPixel * strip = new Adafruit_NeoPixel(299, 2, NEO_GRB + NEO_KHZ800);
//Swimmer s (segmento,posizione,step,r,g,b,totvasche,totrip,tot serie)
Swimmer s  (5, 0, 1, 255, 0, 0, 4, 2, 2);
//volatile serve al compilatore per indicare che quella variabile viene gestita da una funzione "non standard".
//variabile che dice se sono passati effettivamente 10ms
volatile bool was_passed = false;

//variabile che conta i decimi di millesimo trascorsi.
volatile int cron = 0;

//Funzione che viene chiamata ogni MCD millisecondi.
void base_dei_tempi(){
  cron++;
  if (cron >= TIMER1)
    was_passed = true;
}

void setup(){
  Serial.begin(115200);
  Swimmer::setStrip(strip);
  //Imposto i millesimi di secondo per
   //chiamare base_dei_tempi.
  MsTimer2::set(MCD, base_dei_tempi);
  //Avvio la base dei tempi.
  MsTimer2::start();
}

void loop(){
  if (was_passed){
    s.autoStep(true);
    was_passed = false;
    cron = 0;
  }
  if(s.isFinishRip()){
    s.resetRip();
  }
}
