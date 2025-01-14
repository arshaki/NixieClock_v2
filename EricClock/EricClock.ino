
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


volatile uint16_t g_iLEDDataGroup0 = 0x0000;
volatile uint16_t g_iLEDDataGroup1 = 0x0000;
volatile bool g_bSS_Celsius = false;
volatile bool g_bSS_PM = false;
volatile bool g_bSS_Timer = false;
volatile bool g_bSS_Fahrenheit = false;

volatile uint8_t g_iLEDGroup = 0;
volatile bool g_StopwatchRunning = false;
volatile bool g_TimerRunning = false;
volatile uint32_t g_iStopwatchCounterSec = 0;
volatile uint32_t g_iTimerCounterSec = 0;
volatile uint32_t g_i1SecCounter = 0;
volatile uint8_t TimerMin = 0;
volatile uint8_t TimerSec = 0;

volatile bool g_TimerOrStopwatch = false;
volatile uint32_t g_iBtn2LongPressCounter = 0;

 
void TimerHandler1(unsigned int param);
void TimerHandler1Sec(unsigned int param);
void Button1PressedRaw();
void Button2PressedRaw();
void Button3PressedRaw();
void Button1Pressed();
void Button2Pressed();
void Button3Pressed();
void Btn2LongPressed();

void SetNumberGroup(uint8_t iGroup, uint8_t iNumber);
uint8_t ConvertDigitToNumber(uint8_t iNumber);

void SetNumberMinSec(uint32_t iSec);

void SetSpecialSegment();


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
  //int16_t iSet = 1;
  //LEDGroupControl(1, 1);
  //LEDGroupControl(1, 1);
  for(int i = 0; i < 4; ++i)
  {

      delay(1000);

      //iSet <<= 1;
  }
      
}

uint8_t iTemp = 0;

void Button1Pressed()
{
  //Serial.print(g_StopwatchRunning);
  Serial.println(" Button1Pressed");

  if(g_TimerOrStopwatch)
  {
    // timer
    g_iTimerCounterSec++;
  }
  else
  {
    //stopwatch
    g_StopwatchRunning = !g_StopwatchRunning;
  }
}

void Button2Pressed()
{
  Serial.println(" Button2Pressed");

  if(g_TimerOrStopwatch)
  {
    // timer
    g_TimerRunning = !g_TimerRunning;
  }
  else
  {
    //stopwatch
  }

  
}

void Button3Pressed()
{
  Serial.println(" Button3Pressed");

  if(g_TimerOrStopwatch)
  {
    // timer
    g_iTimerCounterSec--;
  }
  else
  {
    //stopwatch
    g_iStopwatchCounterSec = 0;
  }

}

void Btn2LongPressed()
{
  Serial.println(" Button2LongPressed");
  g_TimerOrStopwatch = !g_TimerOrStopwatch;
  g_iTimerCounterSec = 0;

  g_bSS_Timer = g_TimerOrStopwatch;
  g_bSS_PM = !g_TimerOrStopwatch;

  SetSpecialSegment();
}

void HandleLongPressBtn2()
{
  if(digitalRead(PIN_BTN2) == LOW)
  {
      g_iBtn2LongPressCounter++;
      if(g_iBtn2LongPressCounter == 3)
      {
        Btn2LongPressed();
      }
  }
  else
  {
    g_iBtn2LongPressCounter = 0;
  }
}

void TimerHandler1Sec(unsigned int param)
{
  if(g_TimerOrStopwatch == false)
  {
    //stopwatch
    Serial.println(" Timer 1 sec");

    if(g_StopwatchRunning)
    {
      g_iStopwatchCounterSec++;
    }

    SetNumberMinSec(g_iStopwatchCounterSec);
  }
  else
  {
    //timer
    if((g_TimerRunning) && (g_iTimerCounterSec>0))
    {
      g_iTimerCounterSec--;
    }
  }
 
  HandleLongPressBtn2();
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

  
  g_i1SecCounter++;
  if(g_i1SecCounter == TIMER1SEC_COUNT)
  {
    // call 1 sec timer
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

  SetSpecialSegment();
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


void SetSpecialSegment()
{
    if(g_bSS_Celsius)
    {
      g_iLEDDataGroup0 |= 0x80;
    }
    else
    {
      g_iLEDDataGroup0 &= ~0x80;
    }
  
    if(g_bSS_PM)
    {
      g_iLEDDataGroup0 |= 0x8000;
    }
    else
    {
      g_iLEDDataGroup0 &= ~0x8000;
    }
 
     if(g_bSS_Timer)
    {
      Serial.println("adding bit");
      g_iLEDDataGroup1 |= 0x80;
    }
    else
    {
      g_iLEDDataGroup1 &= ~0x80;
    }   
 
    if(g_bSS_Fahrenheit)
    {
      g_iLEDDataGroup1 |= 0x8000;
    }
    else
    {
      g_iLEDDataGroup1 &= ~0x8000;
    }   
  
}
