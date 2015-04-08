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



// Declare which fonts we will be using
extern uint8_t SmallFont[];

UTFT         myGLCD(SSD1963_800, 38, 39, 40, 41);   // Remember to change the model parameter to suit your display module!
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
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);

  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  
  myButtons.setTextFont(BigFont);
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
       foto_presentacion();
      menu_inicial();
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
        delay(3000);
      }
        delay(3000);
 }
 
  //Menu inicial********************************************************************************************************************************
 void	menu_inicial(){
         int SET_PW1, SET_PW2, READ, but4, butX, butY, pressed_button;
  boolean default_colors = true;
  
  SET_PW1 = myButtons.addButton( 40,  30, 100,  60, "PWM1");
  SET_PW2 = myButtons.addButton( 180,  30, 100,  60, "PWM2");
      myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
    myGLCD.setFont(BigFont);
  myGLCD.print("     ", 40, 110);
    myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
    myGLCD.setFont(BigFont);
  myGLCD.print("READ:", 40, 125);
      myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
    myGLCD.setFont(BigFont);
  myGLCD.print("     ", 40, 140);
 
  
  myButtons.drawButtons();

  myGLCD.print("You pressed:", 110, 205);
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setBackColor(VGA_WHITE);
  myGLCD.print("None    ", 110, 220);

  while(1) 
  {
    if (myTouch.dataAvailable() == true)
    {
      pressed_button = myButtons.checkButtons();

      if (pressed_button==SET_PW1)
        myGLCD.print("Button 1", 110, 220);
      if (pressed_button==SET_PW2)
        myGLCD.print("Button 2", 110, 220);
      if (pressed_button==READ)
       if (pressed_button==-1)
        myGLCD.print("None    ", 110, 220);
    }
 } 
  
}

