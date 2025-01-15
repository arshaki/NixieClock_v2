/* Структурная функция начальной установки
 *  Входные параметры: нет
 *  Выходные параметры: нет
 */
void setup() 
{
  // случайное зерно для генератора случайных чисел
  randomSeed(analogRead(6) + analogRead(7));


  // настройка пинов на выход
  pinMode(DECODER0, OUTPUT);
  pinMode(DECODER1, OUTPUT);
  pinMode(DECODER2, OUTPUT);
  pinMode(DECODER3, OUTPUT);
  pinMode(KEY0, OUTPUT);
  pinMode(KEY1, OUTPUT);
  pinMode(KEY2, OUTPUT);
  pinMode(KEY3, OUTPUT);
  pinMode(KEY4, OUTPUT);
  pinMode(KEY5, OUTPUT);

  pinMode(PIEZO, OUTPUT);
  pinMode(GEN, OUTPUT);
  pinMode(DOT, OUTPUT);
  
  digitalWrite(GEN, 0);                           // устранение возможного "залипания" выхода генератора
 
  // настройка быстрого чтения аналогового порта (mode 4)
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  analogRead(A6);                                 // устранение шума
  analogRead(A7);


  curIndi = 0;
  for(int i = 0; i < NUMTUB; ++i)
  {
    indiDimm[i] = MAX_BRIGHTNESS;
    indiCounter[i] = 0;
    indiDigits[i] = i;
  }

  
  TCCR1B = TCCR1B & 0b11111000 | 1;               // ставим делитель 1
  TCCR2B = (TCCR2B & B11111000) | 2;    // делитель 8
  TCCR2A |= (1 << WGM21);   // включить CTC режим для COMPA
  TIMSK2 |= (1 << OCIE2A);  // включить прерывания по совпадению COMPA 
  




  // EEPROM
  if (EEPROM.read(1023) != 103) 
  {                 // первый запуск
    EEPROM.put(1023, 103);
  }


  // turn on the DC-DC PWM
  setPWM(GEN, DUTY);



}
