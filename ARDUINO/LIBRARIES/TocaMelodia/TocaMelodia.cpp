/*
  TocaMelodia.cpp - Libreria para tocar melodias pre-establecidas.
  Guimi (http://guimi.net)
  Released into the public domain.

  Based on work by Brett Hagman (pitches.h), Tom Igoe (ToneMelody), Erik Kringen and Dave Tucker
*/

#include "Arduino.h"
#include "TocaMelodia.h"

// Constructor
TocaMelodia::TocaMelodia(int pin) {
  // Definimos el pin como de salida
  pinMode(pin, OUTPUT);
  // Guardamos el pin indicado en la variable de la clase
  _pin = pin;
}

void TocaMelodia::melodia(int melodia) {
  switch(melodia){
    case 0:
      melodia0();
      break;
    case 1:
      melodia1();
      break;
    case 2:
      melodia2();
      break;
    case 3:
      melodia3();
      break;
    case 4:
      melodia4();
      break;
    case 5:
      melodia5();
      break;
  } 
}

void TocaMelodia::melodia0() { // MELODIA_FANFARRIA 0
  tone(_pin, NOTE_C4, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_G3, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_G3, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_A3, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_G3, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, 0, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_B3, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_C4, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  delay(10);
}

void TocaMelodia::melodia1() { // MELODIA_PODER_PERRUNO
  tone(_pin, NOTE_B4, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_B4, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_B4, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_GS5, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_B4, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_GS5, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  delay(10);
}

void TocaMelodia::melodia2() { // MELODIA_ENCUENTROS_3A_FASE
  // Based on work by Erik Kringen
  //http://www.mycontraption.com/sound-effects-with-and-arduino/
  tone(_pin, NOTE_AS5, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_C6, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_GS4, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_GS3, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_DS5, NOTA_BLANCA); delay (NOTA_BLANCA_PAUSA); noTone(_pin);
  delay (500);

  tone(_pin, NOTE_AS4, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_C5, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_GS3, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_GS2, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_DS4, NOTA_BLANCA); delay (NOTA_BLANCA_PAUSA); noTone(_pin);
  delay (500);

  tone(_pin, NOTE_AS3, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_C4, NOTA_NEGRA); delay (NOTA_NEGRA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_GS2, NOTA_BLANCA); delay (NOTA_BLANCA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_GS1, NOTA_BLANCA); delay (NOTA_BLANCA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_DS3, NOTA_REDONDA); delay (NOTA_REDONDA_PAUSA); noTone(_pin);
  delay(10);
}

void TocaMelodia::melodia3() { // MELODIA_R2D2
  // Based on work by Erik Kringen and Dave Tucker
  //http://www.mycontraption.com/sound-effects-with-and-arduino/
  //http://dtucker.co.uk/make/arduino-using-my-melodyutils-library-for-r2-d2-style-chirps.html
  tone(_pin, NOTE_A7, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_G7, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_E7, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_C7, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_D7, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_B7, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_F7, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_C8, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_A7, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_G7, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_E7, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_C7, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_D7, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_B7, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_F7, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  tone(_pin, NOTE_C8, NOTA_CORCHEA); delay (NOTA_CORCHEA_PAUSA); noTone(_pin);
  delay(10);
}


void TocaMelodia::melodia4() { // MELODIA_OHHH
  Glis(NOTE_C6, NOTE_C7, 6);
  Glis(NOTE_C7, NOTE_C6, 5);
  //for (int i=1000; i<2000; i=i*1.02) { tone(_pin,i,10); };
  //for (int i=2000; i>1000; i=i*.98) { tone(_pin,i,10); delay(10);};
}


void TocaMelodia::melodia5() { // MELODIA_UHOH
  Glis(NOTE_C6, NOTE_DS6, 6);
  delay(200);
  Glis(NOTE_DS6, NOTE_CS6, 5);
  //for (int i=1000; i<1244; i=i*1.01) { tone(_pin,i,30); };
  //delay(200);
  //for (int i=1244; i>1108; i=i*.99) { tone(_pin,i,30);  delay(30);};

}


void TocaMelodia::Glis(int nota1, int nota2, int tasa) {
  // By Dave Tucker
  //http://dtucker.co.uk/make/arduino-using-my-melodyutils-library-for-r2-d2-style-chirps.html
  // Glissando = Slide
  // Slides up or down from note1 to note2
  // rate = 0 is fast and can be increased to slow the effect down

  if (nota1 < nota2) { //Slide up
    for (int nota = nota1; nota < nota2; nota++) {
      tone(_pin, nota, tasa); delay (tasa); noTone(_pin);
    }
  } else { //Slide down
    for (int nota = nota1; nota > nota2; nota--) {
      tone(_pin, nota, tasa); delay (tasa); noTone(_pin);
    }
  }
  noTone(_pin);
}

void TocaMelodia::Trem(int note, int length, int rate) {
  // By Dave Tucker
  //http://dtucker.co.uk/make/arduino-using-my-melodyutils-library-for-r2-d2-style-chirps.html
  // Tremolo = Fast repetition of a note
  // note = the note (from pitches.h)
  // length = duration of the effect in msec
  // rate = number of repetitions

  int note_duration = length/rate;
  int pauseBetweenNotes = rate * (1 + rate/length);

  for (int i = 0; i <= rate; i++) {
    tone(_pin, note, note_duration);
    delay(pauseBetweenNotes);
    noTone(_pin);
  }
}

