/*
  TocaMelodia.h - Libreria para tocar melodias pre-establecidas.
  Guimi (http://guimi.net)
  Released into the public domain.

  Based on ToneMelody by Tom Igoe 
*/

#include <TocaMelodia.h>

TocaMelodia tocaMelodia(12);  // Inicializamos el altavoz, indicando en el pin

void setup() {
}

void loop() {
  tocaMelodia.melodia(MELODIA_FANFARRIA);
  delay(1000);


  delay(2000);
}
