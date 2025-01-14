// библиотеки

#include <Wire.h>
#include <EEPROM.h>


volatile int8_t indiDigits[NUMTUB];               // цифры, которые должны показать индикаторы (0-10)
volatile int8_t curIndi;                          // текущий индикатор (0-5)


/* регулировка напряжения */
const uint8_t iduty = 10;                         // период интегрирования ошибки напряжения
uint8_t idcounter = 0;                            // текущий период интегрирования
const int8_t maxerrduty = 10;                     // максимальное значение интегрированной ошибки напряжения
int duty_delta = 0;                               // текущая интегральная ошибка
const int nominallevel = 550;                     // номинальное значение напряжения
const uint8_t maxduty = 220;                      // защита от чрезмерного повышения напряжения
const uint8_t minduty = 10;                       // защита от выключения
uint8_t r_duty;                                   // актуальная скважность ШИМ анодного напряжения
int8_t startup_delay = 5;                         // задержка в применении нового значения ШИМ после запуска

/* макроопределения битовых операций */
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) 
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))


volatile boolean dotFlag = false;                 // признак фазы внутри половины секунды

int8_t hrs, mins, secs;                           // часы, минуты, секунды


/* переменнные из исходного скетча */


boolean currentDigit = false;
int8_t changeHrs, changeMins;


byte anodeStates = 0x3F;                          // в оригинальном скетче было массивом логических переменных
                                                  // заменено на байт, биты (начиная с 0), которого определяют
           
byte currentLamp, flipEffectStages;
bool trainLeaving;

/* Быстрое управление выходными пинами Ардуино (аналог digitalWrite)
 *  Входные параметры:
 *    byte pin: номер пина в нотации Ардуино, подлежащий изменению
 *    byte х: значение, в которое устанавливается pin (0 или 1)
 *  Выходные параметры: нет
 */
void setPin(byte pin, byte x) {
  switch (pin) {                                  // откл pwm
    case 3:                                       // 2B
      bitClear(TCCR2A, COM2B1);
      break;
    case 5:                                       // 0B
      bitClear(TCCR0A, COM0B1);
      break;
    case 6:                                       // 0A
      bitClear(TCCR0A, COM0A1);
      break;
    case 9:                                       // 1A
      bitClear(TCCR1A, COM1A1);
      break;
    case 10:                                      // 1B
      bitClear(TCCR1A, COM1B1);
      break;
    case 11:                                      // 2A
      bitClear(TCCR2A, COM2A1);
      break;
  }

  x = ((x != 0) ? 1 : 0);
  if (pin < 8) bitWrite(PORTD, pin, x);
  else if (pin < 14) bitWrite(PORTB, (pin - 8), x);
  else if (pin < 20) bitWrite(PORTC, (pin - 14), x);
  else return;
}


/* Быстрое управление ШИМ Ардуино (аналог analogWrite). Работает только на пинах с аппаратной поддержкой.
 *  Входные параметры:
 *    byte pin: номер пина в нотации Ардуино, подлежащий изменению
 *    byte duty: относительное значение длительности импульса на выбранном pin
 *  Выходные параметры: нет
 */
void setPWM(byte pin, byte duty) {
  if (duty == 0) setPin(pin, LOW);
  else {
    switch (pin) {
      case 5:
        bitSet(TCCR0A, COM0B1);
        OCR0B = duty;
        break;
      case 6:
        bitSet(TCCR0A, COM0A1);
        OCR0A = duty;
        break;
      case 10:
        bitSet(TCCR1A, COM1B1);
        OCR1B = duty;
        break;
      case 9:
        bitSet(TCCR1A, COM1A1);
        OCR1A = duty;
        break;
      case 3:
        bitSet(TCCR2A, COM2B1);
        OCR2B = duty;
        break;
      case 11:
        bitSet(TCCR2A, COM2A1);
        OCR2A = duty;
        break;
      default:
        break;
    }
  }
}
