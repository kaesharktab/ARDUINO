#include <UTFT.h>
#include <UTouch.h>


#include <UTFT_Buttons.h>
#include <tinyFAT.h>

#include <UTFT_tinyFAT.h>

  #if defined(__AVR__)
#define imagedatatype  unsigned int
#elif defined(__PIC32MX__)
#define imagedatatype  unsigned short
#elif defined(__arm__)
#define imagedatatype  unsigned short
#endif
 extern imagedatatype ajustes[];
  extern imagedatatype vl3[];
  
// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t SevenSeg_XXXL_Num[];
extern unsigned int v[0x400];            //Image loaded as a sourche file (later to move to SD)

int x, y,g,menu,pressed_button;

UTFT myGLCD(TFT01_70, 38, 39, 40, 41);       //Screen initialization
UTouch        myTouch(6, 5, 4, 3, 2);        //Touch initialization

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);                   // Initialize the display
  myGLCD.clrScr();                             // Clear the screen (black)
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_MEDIUM);
  myGLCD.setFont(BigFont);
  myGLCD.fillScr(255,255,255);               // Clear the screen (White)
//  myGLCD.drawBitmap(40, 0, 80, 80, v,3);       // Draw a pixel bitmap
  delay(3000);                                 //Delay to draw the image
  myGLCD.clrScr();                            // Clear the screen (black)
  Menu1();
  menu=1;
}

//Buttons coordinates
//Menu 1
//Button Gpro
int x11=20; int y11=20; int x12=150; int y12=100;
//Button Leds
int x21=180; int y21=20; int x22=300; int y22=100;
//Button Volto
int x31=20; int y31=120; int x32=150; int y32=200;
//Button Spare
int x41=180; int y41=120; int x42=300; int y42=200;
//Button G
int x51=10; int y51=10; int x52=160; int y52=220;
//Button Back
int x61=200; int y61=130; int x62=300; int y62=220;
//Front led
int x71=20; int y71=40; int x72=170; int y72=100;
//Back led
int x81=20; int y81=130; int x82=170; int y82=190;

void Menu1()    // MAIN MENU
{
   #if defined(__AVR__)
#define imagedatatype  unsigned int
#elif defined(__PIC32MX__)
#define imagedatatype  unsigned short
#elif defined(__arm__)
#define imagedatatype  unsigned short
#endif
 extern imagedatatype ajustes[];
  extern imagedatatype vl3[];
  int AJUSTES,l3_button;
  UTFT_Buttons  myButtons(&myGLCD, &myTouch);

  myGLCD.clrScr(); // Clear the screen (black)
  myGLCD.setFont(BigFont);
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(0, 0, 255);
  AJUSTES = myButtons.addButton( 15,  235, 100,  100, ajustes);  //X, Y , LARGO, ANCHO
  l3_button = myButtons.addButton( 395,  235, 100,  53, vl3);

  myButtons.drawButtons();

  menu =1;  
}

void Menu2()    //GPRO MENU
{
   #if defined(__AVR__)
#define imagedatatype  unsigned int
#elif defined(__PIC32MX__)
#define imagedatatype  unsigned short
#elif defined(__arm__)
#define imagedatatype  unsigned short
#endif
 extern imagedatatype ajustes[];
  extern imagedatatype vl3[];
  int AJUSTES,l3_button;
  UTFT_Buttons  myButtons(&myGLCD, &myTouch);

  myGLCD.clrScr(); // Clear the screen (black)
  myGLCD.setFont(BigFont);
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(0, 0, 255);
  AJUSTES = myButtons.addButton( 15,  235, 100,  100, ajustes);  //X, Y , LARGO, ANCHO
  l3_button = myButtons.addButton( 395,  235, 100,  53, vl3);

  myButtons.drawButtons();
 
  menu =2;
}




void loop()
{
     #if defined(__AVR__)
#define imagedatatype  unsigned int
#elif defined(__PIC32MX__)
#define imagedatatype  unsigned short
#elif defined(__arm__)
#define imagedatatype  unsigned short
#endif
 extern imagedatatype ajustes[];
  extern imagedatatype vl3[];

  UTFT_Buttons  myButtons(&myGLCD, &myTouch);

  myGLCD.clrScr(); // Clear the screen (black)
  myGLCD.setFont(BigFont);
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(0, 0, 255);
 
  int AJUSTES,l3_button;

  while (true)
  {
    if (myTouch.dataAvailable())
    {
            pressed_button = myButtons.checkButtons();
 

      if (menu ==1)  
      {
              if (pressed_button == AJUSTES) {
             
   
            menu =2;
            Menu2();
          }
   
      }

      if (menu==2)
      {
      if (pressed_button == l3_button) {
          {     
         
            menu =1;
            Menu1();
          }

        }
      }

    
    }
}
}


void button(int x1, int y1, int x2, int y2)
{
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect (x1,y1,x2,y2);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (x1,y1,x2,y2);
}

// Draw a red frame while a button is touched
void waitForIt(int x1, int y1, int x2, int y2)
{
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
  //while (myTouch.dataAvailable())
  //myTouch.read();
  delay(300);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
}
