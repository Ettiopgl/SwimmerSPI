1 adesso funziona con tastiera e Bluetooth selezionando il numero di nSwimmer manualmente modificando la variabile
nSwimmer
2 start anche da BlueTooth con tasto verde
3 Lcd_Length_String funzione nata per gestire la posizione  x sul display lcd
4 create getMeString e getMeValue e waitStringFromBt da Andrea per la gestione input bt


Prova gestione goSwimmer e nSwimmer : dichiarati nel main in 91 e 93


Variabili main.cpp
totSerie numero di Serie
totRipetizioni numero di ripetizioni
LENSTRIP lunghezza della striscia
RESOLUTION 1.0 / 10000 risoluzione 0,1ms su FlexiTimer2
MCD 1 risoluzione 0,1ms su FlexiTimer2
DECIMAL_TIMER 180   = 180 decimi dimillisecondi da passare come variabile come basedei tempi accensione nLed
DELAY_START_SWIMMER 30 Ritardo di partenza tra uno Swimmer e l'altro
N_COMMANDS 11 numero variabili che si aspetta il BlueTooth
is_inputTastiera = tastiera attiva
count_times
to_print = Variabile ridondante che indica se è possibile chiamare nel loop la funzione printstrip()
goSwimmer1,2,3... = indica Swimmer attivi
tPartenza NON ADOPERATO
totVasche = numero di vasche
dStartSwimmer ritardo partenza nuotatori  ( da inserire adesso è fisso)
nSwimmer numero di swimmer
 mAndatura minuti int
 sAndatura secondi andatura
 mRecupero minuti di recupero
 sRecupero secondi  di recupero
 mSerie minuti di recupero
 sSerie secondi di REcupero
 Distanza metri della ripetizione
lVasca lunghezza della vasca int
tSecRipetizioni totale secondi ripetizione 
tMinRecupero totale minuti di Recupero
tSecSerie totale secondi di Recupero Series
tSecAndatura totale secondi andatura
