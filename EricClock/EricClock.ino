
#define PIN_LED_GROUP_0   8L
#define PIN_LED_GROUP_1   9L
#define PIN_LATCH         10L
#define PIN_BTN1   2L
#define PIN_BTN2   3L
#define PIN_BTN3   7L

#define TIMER1_INTERVAL_MS    3L

#define USE_TIMER_1     true
#include "TimerInterrupt.h"
#include <SPI.h>

uint16_t g_iLEDDataGroup0 = 0x0000;
uint16_t g_iLEDDataGroup1 = 0x0000;
uint8_t g_iLEDGroup = 0;

 
void TimerHandler1(unsigned int param);
void Button1Pressed();
void Button2Pressed();
void Button3Pressed();


inline void LEDGroupControl(uint8_t iGroup, bool bOn)
{
  if(iGroup == 0)
  {
    digitalWrite(PIN_LED_GROUP_0, bOn ? LOW : HIGH);
  }
  else if(iGroup == 1)
  {
    digitalWrite(PIN_LED_GROUP_1, bOn ? LOW : HIGH);
  }
}

inline void LEDWriteData(uint16_t iData)
{
  SPI.transfer16(iData);
  
  // latch it
  digitalWrite(10, HIGH);
  delayMicroseconds(3);
  digitalWrite(10, LOW);  
}


// the setup function runs once when you press reset or power the board
void setup() 
{
  pinMode(PIN_LED_GROUP_0, OUTPUT); // LED group 1
  pinMode(PIN_LED_GROUP_1, OUTPUT); // LED group 2

  pinMode(PIN_LATCH, OUTPUT); // latch
  
  pinMode(15, OUTPUT); // SPI Clock
  pinMode(16, OUTPUT); // SPI MOSI
  pinMode(14, INPUT);  // SPI MISO


  pinMode(PIN_BTN1, INPUT_PULLUP); 
  pinMode(PIN_BTN2, INPUT_PULLUP); 
  pinMode(PIN_BTN3, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(PIN_BTN1), Button1Pressed, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN2), Button2Pressed, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN3), Button3Pressed, RISING);

  digitalWrite(PIN_LED_GROUP_0, HIGH);
  digitalWrite(PIN_LED_GROUP_1, HIGH);
  digitalWrite(PIN_LATCH, LOW);
  
  SPI.begin(); 
  Serial.begin(9600);

  ITimer1.init(1);
  unsigned int iTimerParam = 0;
  ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, iTimerParam);

  
}


// the loop function runs over and over again forever
void loop() 
{
   delay(500);   
}


void Button1Pressed()
{
  static unsigned long iLastInterrupt = 0;
  unsigned long iTimeNow = millis();
  if (iTimeNow - iLastInterrupt < 100) 
  {
    return;
  }
  iLastInterrupt = iTimeNow;  
  
  g_iLEDDataGroup0++;
}

void Button2Pressed()
{
  static unsigned long iLastInterrupt = 0;
  unsigned long iTimeNow = millis();
  if (iTimeNow - iLastInterrupt < 100) 
  {
    return;
  }
  iLastInterrupt = iTimeNow;   
  
  g_iLEDDataGroup0++;  
  g_iLEDDataGroup1++;  
}

void Button3Pressed()
{
  static unsigned long iLastInterrupt = 0;
  unsigned long iTimeNow = millis();
  if (iTimeNow - iLastInterrupt < 100) 
  {
    return;
  }
  iLastInterrupt = iTimeNow; 
    
  g_iLEDDataGroup1++;  
}


void TimerHandler1(unsigned int param)
{
   
  LEDGroupControl(g_iLEDGroup, 1);
  LEDGroupControl(!g_iLEDGroup, 0);
  LEDWriteData(g_iLEDGroup ? g_iLEDDataGroup1 : g_iLEDDataGroup0);
 
  g_iLEDGroup = !g_iLEDGroup;
}
