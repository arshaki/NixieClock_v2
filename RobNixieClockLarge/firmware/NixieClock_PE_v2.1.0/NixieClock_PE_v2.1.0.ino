

// ************************** НАСТРОЙКИ **************************
#define BOARD_TYPE 3
#define DUTY 250                                            // начальная скважность ШИМ

// ---------   ПИНЫ    ---------
#define RTC_SYNC          2                                 // - подключение SQW выхода
#define PIEZO             1                                 // - подключение пищалки
#define AV_CTRL           A6                                // - вход контроля анодного напряжения


#define KEY0              8                                 // - часы (десятки)
#define KEY1              3                                 // - часы (единицы)
#define KEY2              4                                 // - минуты (десятки)
#define KEY3              13                                // - минуты (единицы)
#define KEY4              0                                 // - секунды (десятки) - исправить в v.1 подключение!!!! было подключено к 6
#define KEY5              7                                 // - секунды (единицы)

#define GEN               9                                 // - генератор
#define DOT               10                                // - точка

// дешифратор
#define DECODER0          A0                                // -
#define DECODER1          A1                                // -
#define DECODER2          A2                                // -
#define DECODER3          A3                                // -
// A4, A5 - I2C
// A7 - аналоговая клавиатура


#define NUMTUB            6                                 // количество разрядов (ламп)

#define MAX_BRIGHTNESS  23L

// распиновка ламп

const byte digitMask[] = {8, 9, 0, 1, 5, 2, 4, 6, 7, 3};    // маска дешифратора платы COVID-19 (подходит для ИН-14 и ИН-12)
const byte opts[] = {KEY0, KEY1, KEY2, KEY3, KEY4, KEY5};   // порядок индикаторов слева направо
const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3};  // порядок катодов

volatile int8_t indiDimm[NUMTUB];      // величина диммирования (0-24)
volatile int8_t indiCounter[NUMTUB];   // счётчик каждого индикатора (0-24)
volatile int8_t indiDigits[NUMTUB];    // цифры, которые должны показать индикаторы (0-10)
volatile int8_t curIndi;          // текущий индикатор (0..NUMTUB-1)
boolean anodeStates[] = {1, 1, 1, 1};

/*
  ард ног ном
  А0  7   4
  А1  6   2
  А2  4   8
  А3  3   1
*/
