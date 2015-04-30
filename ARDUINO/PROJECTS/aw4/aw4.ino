

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

int cont =0;
int sensorPin = A0;    // select the input pin for the analog input
int sensorValue = 0;  // variable to store the value coming from the sensor
int ledPin = 11;

int pwm1_out = 9;    // LED connected to digital pin 9
 int pwm2_out = 10;    // LED connected to digital pin 9

// Declare which fonts we will be using
extern uint8_t SmallFont[];

UTFT myGLCD(TFT01_50,38,39,40,41);  // Remember to change the model parameter to suit your display module!
UTFT_tinyFAT myFiles(&myGLCD);

// List of filenames for pictures to display. 
char* files320[]={"PIC301.RAW"}; // 320x240
char* files400[]={"PIC401.RAW", "PIC402.RAW", "PIC403.RAW", "PIC404.RAW", "PIC405.RAW", "PIC406.RAW", "PIC407.RAW", "PIC408.RAW", "PIC409.RAW", "PIC410.RAW"}; // 400x240
char* files220[]={"PIC601.RAW", "PIC602.RAW", "PIC603.RAW", "PIC604.RAW", "PIC605.RAW", "PIC606.RAW", "PIC607.RAW", "PIC608.RAW", "PIC609.RAW", "PIC610.RAW"}; // 220x176
char* files480[]={"PIC701.RAW", "PIC702.RAW", "PIC703.RAW", "PIC704.RAW", "PIC705.RAW", "", "", "", "", ""}; // 480x272
char* files800[]={"PIC801.RAW", "PIC802.RAW", "PIC803.RAW", "PIC804.RAW", "PIC805.RAW", "", "", "", "", ""}; // 800x480
char* files[10];

int picsize_x, picsize_y;
boolean display_rendertime=false;  // Set this to true if you want the rendertime to be displayed after a picture is loaded
boolean display_filename=true;  // Set this to false to disable showing of filename

word res;
long sm, em;
UTouch        myTouch(6,5,4,3,2);

// Finally we set up UTFT_Buttons :)
UTFT_Buttons  myButtons(&myGLCD, &myTouch);
void setup()
{
    pinMode(ledPin, OUTPUT);
  myGLCD.InitLCD(PORTRAIT);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);

  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  
  myButtons.setTextFont(SmallFont);
  myButtons.setSymbolFont(Dingbats1_XL);
  myGLCD.InitLCD();
  myGLCD.clrScr();
  file.initFAT();
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(SmallFont);
  picsize_x=myGLCD.getDisplayXSize();
  picsize_y=myGLCD.getDisplayYSize();
  switch (picsize_x)
  {
    case 220:
      for (int z=0; z<sizeof(files220)/sizeof(*files220);z++)
        files[z] = files220[z];
      break;
    case 320:
      for (int z=0; z<sizeof(files320)/sizeof(*files320);z++)
        files[z] = files320[z];
      break;
    case 400:
      for (int z=0; z<sizeof(files400)/sizeof(*files400);z++)
        files[z] = files400[z];
      break;
    case 480:
      for (int z=0; z<sizeof(files480)/sizeof(*files480);z++)
        files[z] = files480[z];
      break;
    case 800:
      for (int z=0; z<sizeof(files800)/sizeof(*files800);z++)
        files[z] = files800[z];
      break;
  }


}

void loop()
{
  int init =0;
  
        foto_presentacion();
              menu_inicial();

                lectura_analogica(cont);


  }
 
 //FUNCIONES*******************************************************
 
 
 //Foto presentacion*****************************************************************************************************************
 
 void foto_presentacion(){
      sm=millis();
      res=myFiles.loadBitmap(0, 0, picsize_x, picsize_y, "PIC301.RAW");
      em=millis();
      if (res!=0)
      {
   
        if (display_rendertime==true)
        {
          myGLCD.print("Rendertime (secs):", 0, 0);
          myGLCD.printNumF(float((em-sm)/1000.0), 2, 160,0);
        }
        if (display_filename==true)
        {
          myGLCD.print("PIC301.RAW", CENTER, myGLCD.getDisplayYSize()-12);
        }
       // delay(3000);
      }
     //   delay(3000);
 }
 
  //Menu inicial********************************************************************************************************************************
  
 void	menu_inicial(){
    TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  OCR2A = 180;
  OCR2B = 50;
   int pwm1_value = 0 ;
    int pwm2_value = 0 ;
         int PW1_UP, PW1_DOWN,PW1_MAS,PW1_MENOS,PW2_MAS,PW2_MENOS,PW2_UP, PW2_DOWN, PW1_0,PW1_380,PW1_700,PW1_1A, PW2_0,PW2_380,PW2_700,PW2_1A,but4, butX, butY, pressed_button;
  boolean default_colors = true;
  
  PW1_UP = myButtons.addButton( 15,  20, 70,  30, "PWM1 UP");  //X, Y , LARGO, ANCHO
  PW1_0 = myButtons.addButton( 15,  55, 55,  30, "0 mA"); 
   PW1_380 = myButtons.addButton( 75,  55, 55,  30, "380 mA"); 
     PW1_700 = myButtons.addButton( 15,  90, 55,  30, "700 mA");
       PW1_1A = myButtons.addButton( 75,  90, 55,  30, "1A");
  PW1_DOWN = myButtons.addButton( 15,  125, 90,  30, "PW1 DOWN");
   PW1_MAS = myButtons.addButton( 110,  20, 40,  30, "+10");
   PW1_MENOS = myButtons.addButton( 110,  125, 40,  30, "-10");
   myButtons.drawButtons();

  PW2_UP = myButtons.addButton( 170,  20, 70,  30, "PWM2 UP");
    PW2_0 = myButtons.addButton( 170,  55, 55,  30, "0 mA"); 
   PW2_380 = myButtons.addButton( 230,  55, 55,  30, "380 mA"); 
     PW2_700 = myButtons.addButton(170,  90, 55,  30, "700 mA");
       PW2_1A = myButtons.addButton( 230,  90, 55,  30, "1A");
  PW2_DOWN = myButtons.addButton( 170,  125, 90,  30, "PW2 DOWN");
    PW2_MAS = myButtons.addButton( 265,  20, 40,  30, "+10");
   PW2_MENOS = myButtons.addButton( 265,  125, 40,  30, "-10");
   
    myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_BLUE, VGA_RED);

    myButtons.drawButtons();
    
      myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
    myGLCD.setFont(BigFont);
  

  myGLCD.print("PWM1:", 40, 160);
  myGLCD.print("PWM2:", 180, 160);
  myGLCD.printNumI(pwm1_value, 40, 175);
  myGLCD.printNumI(pwm2_value, 180, 175);
  myGLCD.print("READ:", 40, 205);
  myGLCD.printNumI(sensorValue, 40, 220);
  
 ////MIRAR BOTONES**************************************************************** 
 while(1) 
 {

    if (myTouch.dataAvailable() == true)
    {
      pressed_button = myButtons.checkButtons();
      
 //****bOTONES PWM1

      if (pressed_button==PW1_UP){
  
        pwm1_value=pwm1_value+1;
         analogWrite(pwm1_out, pwm1_value); 
         myGLCD.print("     ", 40, 175);        

      myGLCD.printNumI(pwm1_value, 40, 175);
    }
      if (pressed_button==PW1_DOWN){
  
        pwm1_value=pwm1_value-1;
         analogWrite(pwm1_out, pwm1_value);  
                myGLCD.print("     ", 40, 175);  
             myGLCD.printNumI(pwm1_value, 40, 175);        
    }
 
 //VALORES DIRECTOS  PWM1
         if (pressed_button==PW1_0){
  
        pwm1_value=0;
         analogWrite(pwm1_out, pwm1_value);  
                myGLCD.print("     ", 40, 175);  
             myGLCD.printNumI(pwm1_value, 40, 175);        
    }
  
        if (pressed_button==PW1_380){
  
        pwm1_value=97;
         analogWrite(pwm1_out, pwm1_value);  
                myGLCD.print("     ", 40, 175);  
             myGLCD.printNumI(pwm1_value, 40, 175);        
    }
        if (pressed_button==PW1_700){
  
        pwm1_value=178;
         analogWrite(pwm1_out, pwm1_value);  
                myGLCD.print("     ", 40, 175);  
             myGLCD.printNumI(pwm1_value, 40, 175);        
    }
    
       if (pressed_button==PW1_1A){
  
        pwm1_value=255;
         analogWrite(pwm1_out, pwm1_value);  
                myGLCD.print("     ", 40, 175);  
             myGLCD.printNumI(pwm1_value, 40, 175);        
    }
    
  //****bOTONES PWM1+-10
       if (pressed_button==PW1_MAS){
  
        pwm1_value=pwm1_value+10;
         analogWrite(pwm1_out, pwm1_value); 
         myGLCD.print("     ", 40, 175);        

      myGLCD.printNumI(pwm1_value, 40, 175);
    }
      if (pressed_button==PW1_MENOS){
  
        pwm1_value=pwm1_value-10;
         analogWrite(pwm1_out, pwm1_value);  
                myGLCD.print("     ", 40, 175);  
             myGLCD.printNumI(pwm1_value, 40, 175);        
    }
    
 //****bOTONES PWM2
         if (pressed_button==PW2_UP){
  
        pwm2_value=pwm2_value+1;
         analogWrite(pwm2_out, pwm2_value); 
         myGLCD.print("     ", 180, 175);        

      myGLCD.printNumI(pwm2_value, 180, 175);
    }
      if (pressed_button==PW2_DOWN){
  
        pwm2_value=pwm2_value-1;
         analogWrite(pwm2_out, pwm2_value);  
                myGLCD.print("     ", 180, 175);  
             myGLCD.printNumI(pwm2_value, 180, 175);        
    }
    
 //VALORES DIRECTOS  PWM2
         if (pressed_button==PW2_0){
  
        pwm2_value=0;
         analogWrite(pwm2_out, pwm2_value);  
               myGLCD.print("     ", 180, 175);  
             myGLCD.printNumI(pwm2_value, 180, 175);       
    }
  
        if (pressed_button==PW2_380){
  
        pwm2_value=97;
         analogWrite(pwm2_out, pwm2_value);  
               myGLCD.print("     ", 180, 175);  
             myGLCD.printNumI(pwm2_value, 180, 175);      
    }
        if (pressed_button==PW2_700){
  
        pwm2_value=178;
         analogWrite(pwm2_out, pwm2_value);  
                  myGLCD.print("     ", 180, 175);  
             myGLCD.printNumI(pwm2_value, 180, 175);    
    }
    
       if (pressed_button==PW2_1A){
  
        pwm2_value=255;
         analogWrite(pwm2_out, pwm2_value);  
                myGLCD.print("     ", 180, 175);  
             myGLCD.printNumI(pwm2_value, 180, 175);   
    }

    
 //****bOTONES PWM2+-10
 
         if (pressed_button==PW2_MAS){
  
        pwm2_value=pwm2_value+10;
         analogWrite(pwm2_out, pwm2_value); 
         myGLCD.print("     ", 180, 175);        

      myGLCD.printNumI(pwm2_value, 180, 175);
    }
      if (pressed_button==PW2_MENOS){
  
        pwm2_value=pwm2_value-10;
         analogWrite(pwm2_out, pwm2_value);  
                myGLCD.print("     ", 180, 175);  
             myGLCD.printNumI(pwm2_value, 180, 175);        
    }
    }
 }
 
}

 void	lectura_analogica(int cont){
     sensorValue = analogRead(sensorPin);
     int cont_=cont;
           
               myGLCD.print("    ", 48, 220);  
               myGLCD.printNumI(sensorValue, 40, 220);
               cont=0;
 
   }
