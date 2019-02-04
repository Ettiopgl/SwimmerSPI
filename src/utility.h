#include <Arduino.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
// uint8_t totRipetizioni, totSerie, tSecSerie, mAndatura, sAndatura, mRecupero, sRecupero, mSerie, sSerie;
 //int Distanza, lVasca,tSecRipetizioni = 0, tMinRecupero = 0,tSecSerie = 0, tSecAndatura = 0;
// uint8_t      nSwimmer, tPartenza = 0,totVasche, dStartSwimmer=3;


String getMeString(String s){
  String first_part = "";
  unsigned int i = 0;
  while(s[i] != '=' && i < s.length())
  first_part += s[i++];
  return first_part;
}//getMeString

int getMeValue(String s){
  String second_part = "";
  unsigned int i = 0;
  while(s[i] != '=' && i < s.length()){i++;}
  i++;
  while(s[i] != ';' && i < s.length())
  second_part += s[i++];
  return second_part.toInt();
}//getMeValue

String waitStringFromBt(){
  bool is_finish = false;
  String request = "";
  while(!is_finish){
    if (Serial3.available()){
      request += (char)Serial3.read();
    }
    if (request.length() > 0 && (request[request.length()-1] == ';' ||  request[request.length()-1] == '\n')){
      is_finish = true;
    }
  }
  return request;
}


#define LENGHDISPLAY 16  // LUNGHEZZA DISPLAY MENO 4
void Lcd_Length_String(String Row1,byte xPos,byte yPos,bool cursorOn,long result)
{

  byte nSpace;            //  numero spazi
  String backSpace =" ";// stringa con uno spazio
  String sRow1 = Row1;
  String Equal = "=";
  nSpace=sRow1.length(); // lunghezza della  stringa
  nSpace= (LENGHDISPLAY-1)- nSpace; // per tenere conto del = alla fine
  for(int i=0;i<nSpace;i++){
    sRow1.concat(backSpace);
    Row1=Row1;
  }
  sRow1.concat(Equal); // concatena = a fine stringa
  lcd.setCursor(xPos,yPos);
  lcd.print("                    "); // pulisce la riga
  lcd.setCursor(xPos,yPos);
  lcd.print(sRow1);
  //lcd.print("  ");
  //lcd.blink();
  if (cursorOn){   // se si scrivi il valore
    lcd.setCursor((sRow1.length()),yPos);
    //lcd.cursor_on(); //si posiziona alla fine
    //lcd.print("=" );
    lcd.print(result);
    //lcd.scrollDisplayLeft(); // scrolla l'intero DISPLAY
  }
}

//////fine  gestione lcd

// Messaggio di avvio
void inputMessage(){
  // Print a inputMessage to the LCD. from 154 to 188 line main.cpp
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello Swimmers");
  lcd.setCursor(5,1);
  lcd.print("Power by");
  lcd.setCursor(6,2);
  lcd.print("Alessio");
  lcd.setCursor(2,3);
  lcd.print("Andrea & Michele");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("     Input dati     ");
  delay(2000);
  lcd.setCursor(0,0);
  lcd.print("Cercare il canale ");
  lcd.setCursor(0,1);
  lcd.print("Swimmer per input");
  lcd.setCursor(0,2);
  lcd.print("dati da Bluetooth");
  lcd.setCursor(0,3);
  lcd.print("poi premere B");

  delay(10000);

  lcd.clear();
  lcd.print("Funtions");             // Stampo a video il AVE ...
  lcd.setCursor(0, 1);          // 1° colonna - 2° riga
  lcd.print("C-#-* for Clear-Reset-Return");            // e prosegue sulla stessa riga
  lcd.setCursor(0, 2);          // 1° colonna - 3° riga
  lcd.print("B =Go to Bluetooth"); // e prosegue sulla stessa riga
  lcd.setCursor(0, 3);          // 1° colonna - 3° riga
  lcd.print("A = Go to keypad");      // e prosegue sulla stessa riga
  delay(2000);
}




//void summaryInput(){} //riepilogo input dati da bt  from    to    e da keypad from    to
