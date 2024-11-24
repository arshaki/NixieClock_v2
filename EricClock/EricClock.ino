
#define PIN_LED_GROUP_0   8L
#define PIN_LED_GROUP_1   9L
#define PIN_LATCH         10L
#define PIN_BTN1   2L
#define PIN_BTN2   3L
#define PIN_BTN3   7L

#define TIMER1_INTERVAL_MS    4L
#define TIMER1SEC_COUNT     250L

#define USE_TIMER_1     true
#include "TimerInterrupt.h"
#include <SPI.h>

uint16_t g_iLEDDataGroup0 = 0x0000;
uint16_t g_iLEDDataGroup1 = 0x0000;
uint8_t g_iLEDGroup = 0;
bool g_StopwatchRunning = false;
uint32_t g_iStopwatchCounterSec = 0;
uint32_t g_i1SecCounter = 0;

 
void TimerHandler1(unsigned int param);
void TimerHandler1Sec(unsigned int param);
void Button1PressedrRaw();
void Button2PressedRaw();
void Button3PressedRaw();
void Button1Pressed();
void Button2Pressed();
void Button3Pressed();

void SetNumberGroup(uint8_t iGroup, uint8_t iNumber);
uint8_t ConvertDigitToNumber(uint8_t iNumber);

void SetNumberMinSec(uint32_t iSec);


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
  attachInterrupt(digitalPinToInterrupt(PIN_BTN1), Button1PressedRaw, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN2), Button2PressedRaw, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN3), Button3PressedRaw, RISING);

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
  for(int i = 0; i < 99; ++i)
  {
      //SetNumberGroup(1, i);
      //SetNumberGroup(0, i);
      delay(200);
  }
      
}

uint8_t iTemp = 0;

void Button1Pressed()
{
  g_StopwatchRunning = !g_StopwatchRunning;
  Serial.print(g_StopwatchRunning);
  Serial.println(" Button1Pressed");
}

void Button2Pressed()
{
   
}

void Button3Pressed()
{
  g_iStopwatchCounterSec = 0;
  
}


void TimerHandler1Sec(unsigned int param)
{
  Serial.println(" Timer 1 sec");

   if(g_StopwatchRunning)
   {
    g_iStopwatchCounterSec++;
   }

  SetNumberMinSec(g_iStopwatchCounterSec);
}

void SetNumberMinSec(uint32_t iSec)
{
  int32_t iDisplaySec = iSec % 60;
  int32_t iDisplayMin = iSec / 60;
  SetNumberGroup(1, iDisplaySec);
  SetNumberGroup(0, iDisplayMin);
}

void Button1PressedRaw()
{
  static unsigned long iLastInterrupt = 0;
  unsigned long iTimeNow = millis();
  if (iTimeNow - iLastInterrupt < 100) 
  {
    return;
  }
  iLastInterrupt = iTimeNow;  
  
  Button1Pressed();
}

void Button2PressedRaw()
{
  static unsigned long iLastInterrupt = 0;
  unsigned long iTimeNow = millis();
  if (iTimeNow - iLastInterrupt < 100) 
  {
    return;
  }
  iLastInterrupt = iTimeNow;   
  
  Button2Pressed();
}

void Button3PressedRaw()
{
  static unsigned long iLastInterrupt = 0;
  unsigned long iTimeNow = millis();
  if (iTimeNow - iLastInterrupt < 100) 
  {
    return;
  }
  iLastInterrupt = iTimeNow; 
    
  Button3Pressed();
}


void TimerHandler1(unsigned int param)
{
  LEDGroupControl(g_iLEDGroup, 1);
  LEDGroupControl(!g_iLEDGroup, 0);
  LEDWriteData(g_iLEDGroup ? g_iLEDDataGroup1 : g_iLEDDataGroup0);
 
  g_iLEDGroup = !g_iLEDGroup;

  // call 1 sec timer
  g_i1SecCounter++;
  if(g_i1SecCounter == TIMER1SEC_COUNT)
  {
    TimerHandler1Sec(0);
    g_i1SecCounter = 0;
  }
}

void SetNumberGroup(uint8_t iGroup, uint8_t iNumber)
{
  int8_t iDigit0 = 0;
  int8_t iDigit1 = 0;

  iDigit0 = iNumber/10;
  iDigit1 = iNumber%10;

  uint16_t iLEDDigit0 = ConvertDigitToNumber(iDigit0);
  uint16_t iLEDDigit1 = ConvertDigitToNumber(iDigit1);
  iLEDDigit1 *= 256;

  uint16_t oLEDFinal = iLEDDigit0 | iLEDDigit1;

  if (iGroup == 0)
  {
    g_iLEDDataGroup0 = oLEDFinal;
  }
  else  if (iGroup == 1)
  {
    g_iLEDDataGroup1 = oLEDFinal;
  }
}

uint8_t ConvertDigitToNumber(uint8_t iNumber)
{
  if(iNumber == 0)
  {
    return 0x3F;
  }
  else if(iNumber == 1)
  {
    return 0x6;
  }
  else if(iNumber == 2)
  {
    return 0x5B;
  }
  else if(iNumber == 3)
  {
    return 0x4F;
  }
  else if(iNumber == 4)
  {
    return 0x66;
  }
  else if(iNumber == 5)
  {
    return 0x6D;
  }
  else if(iNumber == 6)
  {
    return 0x7D;
  }
  else if(iNumber == 7)
  {
    return 0x7;
  }
  else if(iNumber == 8)
  {
    return 0x7F;
  }
  else if(iNumber == 9)
  {
    return 0x6F;
  }
}

