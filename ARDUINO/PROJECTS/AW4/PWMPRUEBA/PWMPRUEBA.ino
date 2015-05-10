#include <PWM.h>

//use pin 11 on the Mega instead, otherwise there is a frequency cap at 31 Hz
int led = 10;                // the pin that the LED is attached to
int brightness = 0;         // how bright the LED is
int fadeAmount = 5;         // how many points to fade the LED by
int32_t frequency = 1000; //frequency (in Hz)

void setup()
{
  //initialize all timers except for 0, to save time keeping functions
  InitTimersSafe(); 

  //sets the frequency for the specified pin
  bool success = SetPinFrequencySafe(led, frequency);
  
  //if the pin frequency was set successfully, pin 13 turn on
  if(success) {
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);    
  }
}

void loop()
{
  //use this functions instead of analogWrite on 'initialized' pins
  pwmWrite(led, brightness);

  brightness = brightness + fadeAmount;

  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ; 
  }     
  
  delay(30);      
}
