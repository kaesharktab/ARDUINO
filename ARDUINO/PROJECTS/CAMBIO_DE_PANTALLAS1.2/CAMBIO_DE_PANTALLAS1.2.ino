#include <UTFT.h>
#include <UTouch.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t SevenSeg_XXXL_Num[];
extern unsigned int v[0x400];            //Image loaded as a sourche file (later to move to SD)

int x, y,g,menu;

UTFT myGLCD(TFT01_70, 38, 39, 40, 41);       //Screen initialization
UTouch        myTouch(6, 5, 4, 3, 2);        //Touch initialization

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);                   // Initialize the display
  myGLCD.clrScr();                             // Clear the screen (black)
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
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
  myGLCD.clrScr(); // Clear the screen (black)
  myGLCD.setFont(BigFont);
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(0, 0, 255);
  //BUTTON Gpro
  button (x11,y11,x12,y12);  myGLCD.print("Gpro",35,50);
  //BUTTON Leds
  button (x21,y21,x22,y22);  myGLCD.print("Leds",210,50);    
  //BUTTON Volto
  button (x31,y31,x32,y32);  myGLCD.print("Volto",35,150);     
  //BUTTON Spare
  button (x41,y41,x42,y42);  myGLCD.print("Spare",210,150); 
  menu =1;  
}

void Menu2()    //GPRO MENU
{
   myGLCD.clrScr(); // Clear the screen (black)
  //BUTTON Gpro
  button (x51,y51,x52,y52);
  //BUTTON Back
  myGLCD.setBackColor(0, 0, 255);
  button (x61,y61,x62,y62);   myGLCD.setFont(BigFont);  myGLCD.print("Back",230,200);   
  menu =2;
}

void Menu3()    //GPRO MENU
{
  myGLCD.clrScr(); // Clear the screen (black)
  //Front led
  button (x71,y71,x72,y72);
  myGLCD.setFont(BigFont);    myGLCD.print("Front Led",30,70); 
  //Back Led
  button (x81,y81,x82,y82);
  myGLCD.setBackColor(0, 0, 255);
  myGLCD.setFont(BigFont);    myGLCD.print("Back Led",30,160);   
  button (x61,y61,x62,y62);   myGLCD.setFont(BigFont);  myGLCD.print("Back",230,200);   
  menu =3;
}


void loop()
{
  while (true)
  {
    if (myTouch.dataAvailable())
    {
      myTouch.read();
      x=myTouch.getX();
      y=myTouch.getY();

      if (menu ==1)  
      {
        if ((y>=y11) && (y<=y12))  // Upper row
        {      
          if ((x>=x11) && (x<=x12))  // Button: Gpro
          {        
            
            menu =2;
            Menu2();
          }
          if ((x>=x21) && (x<=x22))  // Button: Leds
           {
             
            menu =3;
            Menu3();
           }
        }
        if ((y>=y31) && (y<=y32))  // Lower row
        {     
          if ((x>=x31) && (x<=x32)){}  // Button: Volto
            
          if ((x>=x41) && (x<=x42)){}  // Button: Spare
            
        }
      }

      if (menu==2)
      {
        int g=random(6);
    //    myGLCD.setFont(SevenSeg_XXXL_Num);
        myGLCD.printNumI(g,35,50);
        delay(1000);
        if ((y>=y61) && (y<=y62))  // Button: Back
        {     
          if ((x>=x61) && (x<=x62))  // Button: Back
          {     
            
            menu =1;
            Menu1();
          }

        }
      }
      if (menu==3)
      {
        if ((y>=y61) && (y<=y62))  // Button: Back
        {     
          if ((x>=x61) && (x<=x62))  // Button: Back
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
