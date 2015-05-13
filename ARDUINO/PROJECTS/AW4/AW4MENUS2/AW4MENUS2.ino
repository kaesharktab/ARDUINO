


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

#include <TocaMelodia.h>
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

int EV1 = A6;
int EV2 = A7;
int EV3 = A8;
int EV4 = A9;
int EV5 = A10;
int EV6 = A11;

int sensorTD2 = A0;            // Entrada analogica A0 arduino mega pin0  sensor  TD2
int sensorTD2AW4Value = 0;    // variable to store the value coming from the sensor
int sensorTD1 = A1;          // Entrada analogica A1 arduino mega pin1  sensor  TD1
int sensorTD1Value = 0;      // variable to store the value coming from the sensor
TocaMelodia tocaMelodia(12);  // Inicializamos el altavoz, indicando en el pin12
int speakerPin = 12;        // Inicializamos el altavoz, indicando en el pin12
int pwm1_out = 9;    // Salida pwm1 para valvula de llenado aw4 EV7 arduino mega pin9
int pwm2_out = 10;    // Salida pwm2 para valvula de vaciado aw4 EV8 arduino mega pin10


// Declare which fonts we will be using
extern uint8_t SmallFont[];

UTFT myGLCD(TFT01_70, 38, 39, 40, 41); // Remember to change the model parameter to suit your display module!
UTFT_tinyFAT myFiles(&myGLCD);

// List of filenames for pictures to display.


int picsize_x, picsize_y;
boolean display_rendertime = false; // Set this to true if you want the rendertime to be displayed after a picture is loaded
boolean display_filename = false; // Set this to false to disable showing of filename

word res;
long sm, em;
UTouch        myTouch(6, 5, 4, 3, 2);

// Finally we set up UTFT_Buttons :)
UTFT_Buttons  myButtons(&myGLCD, &myTouch);
void setup()
{
  pinMode(speakerPin, OUTPUT);
  pinMode(EV1, OUTPUT);
  pinMode(EV2, OUTPUT);
  pinMode(EV3, OUTPUT);
  pinMode(EV4, OUTPUT);
  pinMode(EV5, OUTPUT);
  pinMode(EV6, OUTPUT);
  delay(100);
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);

  myTouch.InitTouch();
  myTouch.setPrecision(PREC_HI);

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

  foto_presentacion();

  menu_inicial(cont, cont2);

}

//FUNCIONES*******************************************************


//Foto presentacion*****************************************************************************************************************

void foto_presentacion() {

  //myFiles.loadBitmap(0, 0, picsize_x, picsize_y, "PIC301.RAW");
  //  tocaMelodia.melodia(MELODIA_FANFARRIA);
}

//Menu inicial********************************************************************************************************************************

void	menu_inicial(int cont, int cont2) {
  digitalWrite(A2, HIGH);
#if defined(__AVR__)
#define imagedatatype  unsigned int
#elif defined(__PIC32MX__)
#define imagedatatype  unsigned short
#elif defined(__arm__)
#define imagedatatype  unsigned short
#endif
  extern imagedatatype l32[];
  extern imagedatatype aw42[];
  extern imagedatatype ajustes[];


  int AJUSTES, L3_button, AW4_button, pressed_button;
  boolean default_colors = true;


  myButtons.deleteAllButtons();
  myButtons.setTextFont(BigFont);
  AJUSTES = myButtons.addButton( 15,  235, 60,  60, ajustes);  //X, Y , LARGO, ANCHO
  L3_button = myButtons.addButton( 395,  235, 60,  60, l32);
  AW4_button = myButtons.addButton(215 , 235, 60,  60, aw42);
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
        touch_click();
        myButtons.deleteAllButtons();
        menu_ajustes(cont, cont2);



      }
      if (pressed_button == L3_button) {
        touch_click();
        myButtons.deleteAllButtons();
        menu_l3(cont, cont2);
      }


      if (pressed_button == AW4_button) {
        touch_click();
        myButtons.deleteAllButtons();
        menu_estanqueidad(cont, cont2);
      }


    }
  }
}


//***************************MENU AJUSTES******************************************************************

void	menu_ajustes(int cont, int cont2) {
  myButtons.deleteAllButtons();
  myGLCD.clrScr();
  digitalWrite(A2, LOW);
#if defined(__AVR__)
#define imagedatatype  unsigned int
#elif defined(__PIC32MX__)
#define imagedatatype  unsigned short
#elif defined(__arm__)
#define imagedatatype  unsigned short
#endif


  extern imagedatatype casa[];


  myGLCD.drawPixel(0, 0);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  OCR2A = 180;
  OCR2B = 50;
  int pwm1_value = 0 ;
  int pwm2_value = 0 ;
  int EV1_ON, EV2_ON, EV3_ON, EV4_ON, EV5_ON, EV6_ON, EV1_OFF, EV2_OFF, EV3_OFF, EV4_OFF, EV5_OFF, EV6_OFF;
  int  HOME, PW1_UP, PW1_DOWN, PW1_MAS, PW1_MENOS, PW2_MAS, PW2_MENOS, PW2_UP, PW2_DOWN;
  int PW1_0, PW1_380, PW1_700, PW1_1A, PW2_0, PW2_380, PW2_700, PW2_1A, but4, butX, butY, pressed_button;
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

  HOME = myButtons.addButton(  738, 0, 60,  60, casa);

  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.setFont(BigFont);


  myGLCD.print("PWM1 EV7:", 40, 190);
  myGLCD.print("PWM2 EV8:", 350, 190);
  myGLCD.printNumI(pwm1_value, 40, 210);
  myGLCD.printNumI(pwm2_value, 350, 210);
  myGLCD.print("TD2:", 40, 230);
  myGLCD.printNumI(sensorTD2AW4Value, 40, 250);

  myGLCD.print("TD1:", 180, 230);
  myGLCD.printNumI(sensorTD2AW4Value, 180, 250);

  myGLCD.print("EV1:", 61, 290);
  EV1_ON = myButtons.addButton( 38, 316, 50,  30, "ON");//X, Y , LARGO, ANCHO
  EV1_OFF = myButtons.addButton( 98,  316, 50,  30, "OFF");


  myGLCD.print("EV2:", 183, 290);
  EV2_ON = myButtons.addButton( 160,  316, 50,  30, "ON");
  EV2_OFF = myButtons.addButton( 220,  316, 50,  30, "OFF");


  myGLCD.print("EV3:", 305, 290);
  EV3_ON = myButtons.addButton( 282,  316, 50,  30, "ON");
  EV3_OFF = myButtons.addButton( 342,  316, 50,  30, "OFF");


  myGLCD.print("EV4:", 427, 290);
  EV4_ON = myButtons.addButton( 404,  316, 50,  30, "ON");
  EV4_OFF = myButtons.addButton( 464,  316, 50,  30, "OFF");


  myGLCD.print("EV5:", 549, 290);
  EV5_ON = myButtons.addButton(526,  316, 50,  30, "ON");
  EV5_OFF = myButtons.addButton(586,  316, 50,  30, "OFF");


  myGLCD.print("EV6:", 671, 290);
  EV6_ON = myButtons.addButton(648,  316, 50,  30, "ON");
  EV6_OFF = myButtons.addButton(708,  316, 50,  30, "OFF");


  myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_BLUE, VGA_RED);
  myButtons.drawButtons();

  myGLCD.setColor(VGA_LIME);
  myGLCD.fillCircle(94, 365, 15);//X, Y , RADIO
  myGLCD.fillCircle(215, 365, 15);
  myGLCD.fillCircle(337, 365, 15);
  myGLCD.fillCircle(459, 365, 15);
  myGLCD.fillCircle(581, 365, 15);
  myGLCD.fillCircle(703, 365, 15);



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
        touch_click();
        pwm1_value = pwm1_value + 1;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 210);

        myGLCD.printNumI(pwm1_value, 40, 210);
      }
      if (pressed_button == PW1_DOWN) {
        touch_click();
        pwm1_value = pwm1_value - 1;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 210);
        myGLCD.printNumI(pwm1_value, 40, 210);
      }

      //VALORES DIRECTOS  PWM1
      if (pressed_button == PW1_0) {
        touch_click();
        pwm1_value = 0;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 210);
        myGLCD.printNumI(pwm1_value, 40, 210);
      }

      if (pressed_button == PW1_380) {
        touch_click();
        pwm1_value = 97;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 210);
        myGLCD.printNumI(pwm1_value, 40, 210);
      }
      if (pressed_button == PW1_700) {
        touch_click();
        pwm1_value = 178;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 210);
        myGLCD.printNumI(pwm1_value, 40, 210);
      }

      if (pressed_button == PW1_1A) {
        touch_click();
        pwm1_value = 255;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 210);
        myGLCD.printNumI(pwm1_value, 40, 210);
      }

      //****bOTONES PWM1+-10
      if (pressed_button == PW1_MAS) {
        touch_click();
        pwm1_value = pwm1_value + 10;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 210);

        myGLCD.printNumI(pwm1_value, 40, 210);
      }
      if (pressed_button == PW1_MENOS) {
        touch_click();
        pwm1_value = pwm1_value - 10;
        analogWrite(pwm1_out, pwm1_value);
        myGLCD.print("     ", 40, 210);
        myGLCD.printNumI(pwm1_value, 40, 210);
      }

      //****bOTONES PWM2*******************************

      if (pressed_button == PW2_UP) {
        touch_click();
        pwm2_value = pwm2_value + 1;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 350, 210);

        myGLCD.printNumI(pwm2_value, 350, 210);
      }
      if (pressed_button == PW2_DOWN) {
        touch_click();
        pwm2_value = pwm2_value - 1;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 350, 210);
        myGLCD.printNumI(pwm2_value, 350, 210);
      }

      //VALORES DIRECTOS  PWM2
      if (pressed_button == PW2_0) {
        touch_click();
        pwm2_value = 0;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 350, 210);
        myGLCD.printNumI(pwm2_value, 350, 210);
      }

      if (pressed_button == PW2_380) {
        touch_click();
        pwm2_value = 97;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 350, 210);
        myGLCD.printNumI(pwm2_value, 350, 210);
      }
      if (pressed_button == PW2_700) {
        touch_click();
        pwm2_value = 178;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 350, 210);
        myGLCD.printNumI(pwm2_value, 350, 210);
      }

      if (pressed_button == PW2_1A) {
        touch_click();
        pwm2_value = 255;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 350, 210);
        myGLCD.printNumI(pwm2_value, 350, 210);
      }


      //****bOTONES PWM2+-10

      if (pressed_button == PW2_MAS) {
        touch_click();
        pwm2_value = pwm2_value + 10;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 350, 210);

        myGLCD.printNumI(pwm2_value, 350, 210);
      }
      if (pressed_button == PW2_MENOS) {
        touch_click();
        pwm2_value = pwm2_value - 10;
        analogWrite(pwm2_out, pwm2_value);
        myGLCD.print("     ", 350, 210);
        myGLCD.printNumI(pwm2_value, 350, 210);
      }
      if (pressed_button == HOME) {
        touch_click();
        menu_inicial(cont, cont2);
      }
      
      ///********************BOTONES ELECTROVALVULAS EV1 EV2 EV3 EV4 EV5 EV6******************************************
            if (pressed_button == EV1_ON) {
        touch_click();
         digitalWrite(EV1, HIGH);
          myGLCD.setColor(VGA_LIME);
          myGLCD.fillCircle(94, 365, 15);//X, Y , RADIO

      }
      if (pressed_button == EV1_OFF) {
        touch_click();
         digitalWrite(EV1, LOW);
          myGLCD.setColor(VGA_RED);
          myGLCD.fillCircle(94, 365, 15);//X, Y , RADIO

    }
      
      

    }
    cont++;
  }

}



//**********************MENU AW4***************************************************

void	menu_estanqueidad(int cont, int cont2) {

  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  OCR2A = 180;
  OCR2B = 50;
  int pwm1_value = 0 ;
  int pwm2_value = 0 ;
  int LLENADO_MARCHA, LLENADO_PARO, VACIADO_MARCHA, VACIADO_PARO, FORWARD, BACK, HOME, pressed_button;
  boolean default_colors = true;

#if defined(__AVR__)
#define imagedatatype  unsigned int
#elif defined(__PIC32MX__)
#define imagedatatype  unsigned short
#elif defined(__arm__)
#define imagedatatype  unsigned short
#endif
  extern imagedatatype atras[];
  extern imagedatatype adelante[];
  extern imagedatatype casa[];
  myButtons.deleteAllButtons();
  myGLCD.clrScr();

  myGLCD.setColor(VGA_PURPLE);
  myGLCD.setFont(BigFont);
  myGLCD.fillRect(220, 0, 579, 26);
  myGLCD.setBackColor(VGA_PURPLE);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("PRUEBA DE ESTANQUEIDAD", CENTER, 5);

  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.setFont(BigFont);
  myGLCD.print("PRESION ENT TD1", 1, 90);
  myGLCD.print("VALOR", 1, 110);
  myGLCD.print("BAR", 85, 110);
  myGLCD.setColor(VGA_LIME);
  myGLCD.fillCircle(160, 125, 15);//X, Y , RADIO

  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.print("I.TRABAJO:700mA", CENTER, 50);

  myGLCD.print("I.REAL:", 680, 90);
  myGLCD.print("VALOR", 660, 110);
  myGLCD.print("mA", 745, 110);
  myGLCD.setColor(VGA_LIME);
  myGLCD.fillCircle(630, 125, 15);

  myGLCD.setFont(BigFont);
  myGLCD.fillRect(20, 250, 250, 300);
  myGLCD.setBackColor(VGA_LIME);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("LLENADO", 70, 265);

  myGLCD.setColor(VGA_RED);
  myGLCD.setFont(BigFont);
  myGLCD.fillRect(550, 250, 780, 300);
  myGLCD.setBackColor(VGA_RED);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("VACIADO", 600, 265);

  LLENADO_MARCHA = myButtons.addButton( 20,  400, 100,  30, "MARCHA");  //X, Y , LARGO, ANCHO
  myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_BLUE, VGA_LIME);
  myButtons.drawButton(LLENADO_MARCHA);
  LLENADO_PARO = myButtons.addButton( 170,  400, 80,  30, "PARO");
  myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_BLUE, VGA_RED);
  myButtons.drawButton(LLENADO_PARO);
  VACIADO_MARCHA = myButtons.addButton( 550,  400, 100,  30, "MARCHA");
  myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_BLUE, VGA_LIME);
  myButtons.drawButton(VACIADO_MARCHA);
  VACIADO_PARO = myButtons.addButton( 700,  400, 80,  30, "PARO");
  myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_BLUE, VGA_RED);
  myButtons.drawButton(VACIADO_PARO);

  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.drawRect(330, 150, 430, 430);
  myGLCD.print("10BAR", 435, 150);
  myGLCD.print("5BAR", 435, 280);
  myGLCD.print("0BAR", 435, 414);

  HOME = myButtons.addButton( 676, 0, 60,  60, casa);
  myButtons.drawButton(HOME);
  FORWARD = myButtons.addButton( 738, 0, 60,  60, adelante);
  myButtons.drawButton(FORWARD);

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



      if (pressed_button == LLENADO_MARCHA) {
        touch_click();
        myGLCD.clrScr();
        myGLCD.print("marcha llenado", 40, 210);


      }

      if (pressed_button == LLENADO_PARO) {
        touch_click();
        myGLCD.clrScr();
        myGLCD.print("marcha PARO", 40, 210);


      }
      if (pressed_button == VACIADO_MARCHA) {
        touch_click();
        myGLCD.clrScr();
        myGLCD.print("VACIADO llenado", 40, 210);


      }
      if (pressed_button == VACIADO_PARO) {
        touch_click();
        myGLCD.clrScr();
        myGLCD.print("VACIADO PARO", 40, 210);

      }

      if (pressed_button == HOME) {
        touch_click();
        myGLCD.clrScr();
        menu_inicial(cont, cont2);

      }

      if (pressed_button == FORWARD) {
        touch_click();
        myButtons.deleteAllButtons();
        myGLCD.clrScr();
        menu_regulacionAW4(cont, cont2);

      }
    }
  }
}

///*************REGULACION AW4 ************************************************************

void	menu_regulacionAW4(int cont, int cont2) {

  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  OCR2A = 180;
  OCR2B = 50;
  int pwm1_value = 0 ;
  int pwm2_value = 0 ;
  int LLENADO_MARCHA, LLENADO_PARO, VACIADO_MARCHA, VACIADO_PARO, FORWARD, BACK, HOME, pressed_button;
  boolean default_colors = true;

#if defined(__AVR__)
#define imagedatatype  unsigned int
#elif defined(__PIC32MX__)
#define imagedatatype  unsigned short
#elif defined(__arm__)
#define imagedatatype  unsigned short
#endif
  extern imagedatatype atras[];
  extern imagedatatype casa[];
  myButtons.deleteAllButtons();
  myGLCD.clrScr();

  myGLCD.setColor(VGA_PURPLE);
  myGLCD.setFont(BigFont);
  myGLCD.fillRect(220, 0, 579, 26);
  myGLCD.setBackColor(VGA_PURPLE);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("REGULACION AW4", CENTER, 5);

  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.setFont(BigFont);
  myGLCD.print("PRESION ENT TD1", 1, 90);
  myGLCD.print("VALOR", 1, 110);
  myGLCD.print("BAR", 85, 110);
  myGLCD.setColor(VGA_LIME);
  myGLCD.fillCircle(160, 125, 15);//X, Y , RADIO

  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.print("I.TRABAJO:700mA", CENTER, 50);

  myGLCD.print("I.REAL:", 680, 90);
  myGLCD.print("VALOR", 660, 110);
  myGLCD.print("mA", 745, 110);
  myGLCD.setColor(VGA_LIME);
  myGLCD.fillCircle(630, 125, 15);

  myGLCD.setFont(BigFont);
  myGLCD.fillRect(20, 250, 250, 300);
  myGLCD.setBackColor(VGA_LIME);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("LLENADO", 70, 265);

  myGLCD.setColor(VGA_RED);
  myGLCD.setFont(BigFont);
  myGLCD.fillRect(550, 250, 780, 300);
  myGLCD.setBackColor(VGA_RED);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("VACIADO", 600, 265);

  LLENADO_MARCHA = myButtons.addButton( 20,  400, 100,  30, "MARCHA");  //X, Y , LARGO, ANCHO
  myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_BLUE, VGA_LIME);
  myButtons.drawButton(LLENADO_MARCHA);
  LLENADO_PARO = myButtons.addButton( 170,  400, 80,  30, "PARO");
  myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_BLUE, VGA_RED);
  myButtons.drawButton(LLENADO_PARO);
  VACIADO_MARCHA = myButtons.addButton( 550,  400, 100,  30, "MARCHA");
  myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_BLUE, VGA_LIME);
  myButtons.drawButton(VACIADO_MARCHA);
  VACIADO_PARO = myButtons.addButton( 700,  400, 80,  30, "PARO");
  myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_BLUE, VGA_RED);
  myButtons.drawButton(VACIADO_PARO);

  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.drawRect(330, 150, 430, 430);
  myGLCD.print("10BAR", 435, 150);
  myGLCD.print("5BAR", 435, 280);
  myGLCD.print("0BAR", 435, 414);

  HOME = myButtons.addButton( 676, 0, 60,  60, casa);
  myButtons.drawButton(HOME);
  BACK = myButtons.addButton( 738, 0, 60,  60, atras);
  myButtons.drawButton(BACK);

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



      if (pressed_button == LLENADO_MARCHA) {
        touch_click();
        myGLCD.clrScr();
        myGLCD.print("marcha llenado", 40, 210);


      }

      if (pressed_button == LLENADO_PARO) {
        touch_click();
        myGLCD.clrScr();
        myGLCD.print("marcha PARO", 40, 210);


      }
      if (pressed_button == VACIADO_MARCHA) {
        touch_click();
        myGLCD.clrScr();
        myGLCD.print("VACIADO llenado", 40, 210);


      }
      if (pressed_button == VACIADO_PARO) {
        touch_click();
        myGLCD.clrScr();
        myGLCD.print("VACIADO PARO", 40, 210);

      }

      if (pressed_button == HOME) {
        touch_click();
        myGLCD.clrScr();
        menu_inicial(cont, cont2);

      }

      if (pressed_button == BACK) {
        touch_click();
        myButtons.deleteAllButtons();
        myGLCD.clrScr();
        menu_estanqueidad(cont, cont2);

      }
    }
  }
}

void	menu_l3(int cont, int cont2) {

}

void	lectura_analogica() {
  myGLCD.setColor(VGA_WHITE);
  sensorTD2AW4Value = analogRead(sensorTD2);
  myGLCD.print("     ", 40, 250);
  myGLCD.printNumI(sensorTD2AW4Value, 40, 250);

  sensorTD1Value = analogRead(sensorTD1);
  myGLCD.print("     ", 180, 250);
  myGLCD.printNumI(sensorTD1Value, 180, 250);

}

void	touch_click() {
  digitalWrite(speakerPin, HIGH);
  delay(50);
  digitalWrite(speakerPin, LOW);
}
