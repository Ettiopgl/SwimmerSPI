// DA VERIFICARE FLEXITIMER
#include <Arduino.h>
#include <Swimmer.h>
// #include <MsTimer2.h>   // eventualmente usare flextimer2
//https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&cad=rja&uact=8&ved=2ahUKEwjJwbeFg5DdAhXF16QKHS1RDbgQFjABegQICBAB&url=https%3A%2F%2Fgithub.com%2FPaulStoffregen%2FMsTimer2&usg=AOvVaw2z2-fxhAnNavVBQEHeO2DH

#include <FlexiTimer2.h>


#define LENSTRIP 164 // lungheza della striscia
#define MCD 91//  910=0,1 Millisecondi di chiamata passo per passo 15 sec ogni 25m =164
#define TIMER1 1  //Ogni secondo autostep  3...



SPIStrip *strip = new SPIStrip(LENSTRIP);


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
    Swimmer::setStrip(strip);
  //Imposto i millesimi di secondo per
   //chiamare base_dei_tempi.
//  MsTimer2::set(MCD, base_dei_tempi);
  FlexiTimer2::set(MCD, 1.0/10000, base_dei_tempi);

  //Avvio la base dei tempi.
  //MsTimer2::start();
  FlexiTimer2::start();
}

void loop(){
  if (was_passed){
    s.autoStep(true);
    was_passed = false;
    cron = 0;
  }
  if(s.isFinishRip()){

    s.timeRecRipetizioni(10000);
    s.resetRip();
  }
}
