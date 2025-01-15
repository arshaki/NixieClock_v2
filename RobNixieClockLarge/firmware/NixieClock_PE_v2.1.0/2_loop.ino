/* Структурная функция основного цикла
 *  Входные параметры: нет
 *  Выходные параметры: нет
 */

void setDig(byte digit) 
{
  digit = digitMask[digit];
  setPin(DECODER3, bitRead(digit, 0));
  setPin(DECODER1, bitRead(digit, 1));
  setPin(DECODER0, bitRead(digit, 2));
  setPin(DECODER2, bitRead(digit, 3));
}

int g_iData = 0;
 
void loop() 
{

  for(int i = 0; i < NUMTUB; ++i)
  {
    indiDigits[i] = g_iData;
  }

  g_iData++;
  if(g_iData > 9)
  {
    g_iData = 0;
  }


  delay(500);

  setPin(DOT, g_iData %2  == 1);
  
   
  //buttonsTick();                                  // кнопки

  //DCDCTick();                                     // анодное напряжение
}
