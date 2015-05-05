


// Demo_Landscape
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of the loadBitmap()-function.
//
// This program requires UTFT_tinyFAT, UTFT v2.41 or higher,
// as well as tinyFAT v3.0 or higher.
//
// The image files must be present in the root folder
// of a FAT16 formatted SDcard in the module cardslot.
//
// Please note that this demo only supports the following
// display sizes:
//      220x176
//      320x240
//      400x240
//      480x272
//      800x480


#include <UTFT.h>
#include <UTouch.h>
#include <UTFT_Buttons.h>
#include <tinyFAT.h>
#include <UTFT.h>
#include <UTFT_tinyFAT.h>
// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t Dingbats1_XL[];

int cont = 0;
int sensorPin = A0;    // select the input pin for the analog input
int sensorValue = 0;  // variable to store the value coming from the sensor
int ledPin = 11;

int pwm1_out = 9;    // LED connected to digital pin 9
int pwm2_out = 10;    // LED connected to digital pin 9

// Declare which fonts we will be using
extern uint8_t SmallFont[];

UTFT myGLCD(TFT01_70, 38, 39, 40, 41); // Remember to change the model parameter to suit your display module!
UTFT_tinyFAT myFiles(&myGLCD);

// List of filenames for pictures to display.


int picsize_x, picsize_y;
boolean display_rendertime = false; // Set this to true if you want the rendertime to be displayed after a picture is loaded
boolean display_filename = true; // Set this to false to disable showing of filename

word res;
long sm, em;
UTouch        myTouch(6, 5, 4, 3, 2);

// Finally we set up UTFT_Buttons :)
UTFT_Buttons  myButtons(&myGLCD, &myTouch);
void setup()
{
  pinMode(ledPin, OUTPUT);
  delay(100);
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);

  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  myButtons.setTextFont(SmallFont);
  myButtons.setSymbolFont(Dingbats1_XL);
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  file.initFAT();
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(SmallFont);
  picsize_x = myGLCD.getDisplayXSize();
  picsize_y = myGLCD.getDisplayYSize();


}

void loop()
{
  int cont = 0;
  int cont2 = 0;

//  foto_presentacion();

  menu_inicial(cont, cont2);

}

//FUNCIONES*******************************************************


//Foto presentacion*****************************************************************************************************************

void foto_presentacion() {



}

//Menu inicial********************************************************************************************************************************

void	menu_inicial(int cont, int cont2) {

#if defined(__AVR__)
#define imagedatatype  unsigned int
#elif defined(__PIC32MX__)
#define imagedatatype  unsigned short
#elif defined(__arm__)
#define imagedatatype  unsigned short
#endif
  extern imagedatatype vl3[];
  extern imagedatatype aw4[];
  extern imagedatatype ajustes[];

  int AJUSTES, L3_button, AW4_button, pressed_button;
  boolean default_colors = true;
  
  myGLCD.clrScr();
   myGLCD.drawPixel(0,0); 
  //  myFiles.loadBitmap(0, 0, picsize_x, picsize_y, "PIC301.RAW");
  myButtons.setTextFont(BigFont);
  AJUSTES = myButtons.addButton( 15,  235, 100,  100, ajustes);  //X, Y , LARGO, ANCHO
  L3_button = myButtons.addButton( 395,  235, 100,  53, vl3);
  AW4_button = myButtons.addButton(215 , 235, 110,  83, aw4);
  myButtons.drawButtons();


  while (1)
  {

    if (cont == 255) {
      cont = 0;
      cont2++;
      if (cont2 == 80) {
        lectura_analogica();
        cont2 = 0;
      }
    }
    if (myTouch.dataAvailable() == true)
    {
      pressed_button = myButtons.checkButtons();



      if (pressed_button == AJUSTES) {
        myButtons.deleteButton(AJUSTES);
        myButtons.deleteButton(L3_button);
        myButtons.deleteButton(AW4_button);

        myButtons.disableButton(AJUSTES);
        myButtons.disableButton(L3_button);
        myButtons.disableButton(AW4_button);

        menu_ajustes(cont, cont2);



      }
      if (pressed_button == L3_button) {
        menu_inicial(cont, cont2);
        menu_l3(cont, cont2);
      }


      if (pressed_button == AW4_button) {

        menu_aw4(cont, cont2);
      }


    }
  }
}

void	menu_ajustes(int cont, int cont2) {



#if defined(__AVR__)
#define imagedatatype  unsigned int
#elif defined(__PIC32MX__)
#define imagedatatype  unsigned short
#elif defined(__arm__)
#define imagedatatype  unsigned short
#endif
 extern imagedatatype ajustes[];




   myGLCD.drawPixel(0,0); 
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  OCR2A = 180;
  OCR2B = 50;
  int pwm1_value = 0 ;
  int pwm2_value = 0 ;
  int BACK, PW1_UP, PW1_DOWN, PW1_MAS, PW1_MENOS, PW2_MAS, PW2_MENOS, PW2_UP, PW2_DOWN, PW1_0, PW1_380, PW1_700, PW1_1A, PW2_0, PW2_380, PW2_700, PW2_1A, but4, butX, butY, pressed_button;
  boolean default_colors = true;
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.setFont(BigFont);
  myGLCD.print("LLENADO", 40, 20);
  myGLCD.print("VACIADO", 400, 20);

  //****************botones llenado********************************
  PW1_UP = myButtons.addButton( 15,  50, 120,  30, "PWM1 UP");  //X, Y , LARGO, ANCHO
  PW1_MAS = myButtons.addButton( 165,  50, 60,  30, "+10");
  PW1_0 = myButtons.addButton( 15,  85, 90,  30, "0 mA");
  PW1_380 = myButtons.addButton( 165,  85, 120,  30, "380 mA");
  PW1_700 = myButtons.addButton( 15,  120, 120,  30, "700 mA");
  PW1_1A = myButtons.addButton( 165,  120, 90,  30, "1A");
  PW1_DOWN = myButtons.addButton( 15,  155, 130,  30, "PW1 DOWN");
  PW1_MENOS = myButtons.addButton( 165,  155, 60,  30, "-10");

  //*****************Botones vaciado*************************************
  PW2_UP = myButtons.addButton( 350,  50, 120,  30, "PWM2 UP");  //X, Y , LARGO, ANCHO
  PW2_MAS = myButtons.addButton( 500,  50, 60,  30, "+10");
  PW2_0 = myButtons.addButton( 350,  85, 90,  30, "0 mA");
  PW2_380 = myButtons.addButton( 500,  85, 120,  30, "380 mA");
  PW2_700 = myButtons.addButton( 350,  120, 120,  30, "700 mA");
  PW2_1A = myButtons.addButton( 500,  120, 90,  30, "1A");
  PW2_DOWN = myButtons.addButton(350,  155, 130,  30, "PW2 DOWN");
  PW2_MENOS = myButtons.addButton(500,  155, 60,  30, "-10");

  BACK = myButtons.addButton( 650,  235, 100,  100, ajustes);  //X, Y , LARGO, ANCHO

  myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_BLUE, VGA_RED);
  myGLCD.clrScr();
  myButtons.drawButtons();

  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.setFont(BigFont);


  myGLCD.print("PWM1:", 40, 180);
  myGLCD.print("PWM2:", 180, 180);
  myGLCD.printNumI(pwm1_value, 40, 195);
  myGLCD.printNumI(pwm2_value, 180, 195);
  myGLCD.print("READ:", 40, 205);
  myGLCD.printNumI(sensorValue, 40, 240);

  ////MIRAR BOTONES****************************************************************
  while (1)
  {

    if (cont == 255) {
      cont = 0;
      cont2++;
      if (cont2 == 80) {
        lectura_analogica();
        cont2 = 0;
      }
    }
    if (myTouch.dataAvailable() == true)
    {
      pressed_button = myButtons.checkButtons();

      //****bOTONES PWM1

      if (pressed_button == PW1_UP) {

        pwm1_value = pwm1_value + 1;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 195);

        myGLCD.printNumI(pwm1_value, 40, 195);
      }
      if (pressed_button == PW1_DOWN) {

        pwm1_value = pwm1_value - 1;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 195);
        myGLCD.printNumI(pwm1_value, 40, 195);
      }

      //VALORES DIRECTOS  PWM1
      if (pressed_button == PW1_0) {

        pwm1_value = 0;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 195);
        myGLCD.printNumI(pwm1_value, 40, 195);
      }

      if (pressed_button == PW1_380) {

        pwm1_value = 97;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 195);
        myGLCD.printNumI(pwm1_value, 40, 195);
      }
      if (pressed_button == PW1_700) {

        pwm1_value = 178;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 195);
        myGLCD.printNumI(pwm1_value, 40, 195);
      }

      if (pressed_button == PW1_1A) {

        pwm1_value = 255;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 195);
        myGLCD.printNumI(pwm1_value, 40, 195);
      }

      //****bOTONES PWM1+-10
      if (pressed_button == PW1_MAS) {

        pwm1_value = pwm1_value + 10;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 195);

        myGLCD.printNumI(pwm1_value, 40, 1975);
      }
      if (pressed_button == PW1_MENOS) {

        pwm1_value = pwm1_value - 10;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 195);
        myGLCD.printNumI(pwm1_value, 40, 195);
      }

      //****bOTONES PWM2
      if (pressed_button == PW2_UP) {

        pwm2_value = pwm2_value + 1;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 180, 195);

        myGLCD.printNumI(pwm2_value, 180, 195);
      }
      if (pressed_button == PW2_DOWN) {

        pwm2_value = pwm2_value - 1;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 180, 195);
        myGLCD.printNumI(pwm2_value, 180, 195);
      }

      //VALORES DIRECTOS  PWM2
      if (pressed_button == PW2_0) {

        pwm2_value = 0;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 180, 195);
        myGLCD.printNumI(pwm2_value, 180, 195);
      }

      if (pressed_button == PW2_380) {

        pwm2_value = 97;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 180, 195);
        myGLCD.printNumI(pwm2_value, 180, 195);
      }
      if (pressed_button == PW2_700) {

        pwm2_value = 178;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 180, 195);
        myGLCD.printNumI(pwm2_value, 180, 195);
      }

      if (pressed_button == PW2_1A) {

        pwm2_value = 255;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 180, 195);
        myGLCD.printNumI(pwm2_value, 180, 195);
      }


      //****bOTONES PWM2+-10

      if (pressed_button == PW2_MAS) {

        pwm2_value = pwm2_value + 10;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 180, 195);

        myGLCD.printNumI(pwm2_value, 180, 195);
      }
      if (pressed_button == PW2_MENOS) {

        pwm2_value = pwm2_value - 10;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 180, 195);
        myGLCD.printNumI(pwm2_value, 180, 195);
      }
      if (pressed_button == BACK) {

        menu_inicial(cont, cont2);
      }

    }
    cont++;
  }

}

void	lectura_analogica() {
  sensorValue = analogRead(sensorPin);


  myGLCD.print("     ", 40, 220);
  myGLCD.printNumI(sensorValue, 40, 220);

}

void	menu_aw4(int cont, int cont2) {

}

void	menu_l3(int cont, int cont2) {

}
