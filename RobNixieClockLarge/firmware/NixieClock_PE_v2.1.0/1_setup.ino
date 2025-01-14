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
  // ------------------

      
  // задаем частоту ШИМ на 9 и 10 выводах 31 кГц
  TCCR1B = TCCR1B & 0b11111000 | 1;               // ставим делитель 1

  // перенастраиваем частоту ШИМ на пинах 3 и 11 для соответствия таймеру 0
  // Пины D3 и D11 - 980 Гц
  TCCR2B = 0b00000011;                            // x32
  TCCR2A = 0b00000001;                            // phase correct

  // EEPROM
  if (EEPROM.read(1023) != 103) 
  {                 // первый запуск
    EEPROM.put(1023, 103);
  }
 

  // включаем ШИМ
  r_duty = DUTY;
  setPWM(GEN, r_duty);
 
}
