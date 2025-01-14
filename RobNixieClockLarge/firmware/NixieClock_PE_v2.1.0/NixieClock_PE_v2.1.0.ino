

// ************************** НАСТРОЙКИ **************************
#define BOARD_TYPE 3
#define DUTY 180                                            // начальная скважность ШИМ

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

// распиновка ламп
#if (BOARD_TYPE == 0)
const byte digitMask[] = {7, 3, 6, 4, 1, 9, 8, 0, 5, 2};    // маска дешифратора платы in12_turned (цифры нормальные)
const byte opts[] = {KEY0, KEY1, KEY2, KEY3, KEY4, KEY5};   // порядок индикаторов слева направо
const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3};  // порядок катодов in12

#elif (BOARD_TYPE == 1)
const byte digitMask[] = {2, 8, 1, 9, 6, 4, 3, 5, 0, 7};    // маска дешифратора платы in12 (цифры вверх ногами)
const byte opts[] = {KEY5, KEY4, KEY3, KEY2, KEY1, KEY0};   // порядок индикаторов справа налево (для IN-12 turned) и ин-14
const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3};  // порядок катодов in12

#elif (BOARD_TYPE == 2)
const byte digitMask[] = {9, 8, 0, 5, 4, 7, 3, 6, 2, 1};    // маска дешифратора платы in14
const byte opts[] = {KEY5, KEY4, KEY3, KEY2, KEY1, KEY0};   // порядок индикаторов справа налево (для IN-12 turned) и ин-14
const byte cathodeMask[] = {1, 0, 2, 9, 3, 8, 4, 7, 5, 6};  // порядок катодов in14

#elif (BOARD_TYPE == 3)
const byte digitMask[] = {8, 9, 0, 1, 5, 2, 4, 6, 7, 3};    // маска дешифратора платы COVID-19 (подходит для ИН-14 и ИН-12)
const byte opts[] = {KEY0, KEY1, KEY2, KEY3, KEY4, KEY5};   // порядок индикаторов слева направо
const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3};  // порядок катодов

#elif (BOARD_TYPE == 4)
const byte digitMask[] = {8, 9, 0, 1, 5, 2, 4, 6, 7, 3};    // Ладушки ИН-12
const byte opts[] = {KEY3, KEY2, KEY1, KEY0};               // порядок индикаторов слева направо
const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3};  // порядок катодов

#elif (BOARD_TYPE == 5)
const byte digitMask[] = {0, 8, 3, 9, 7, 6, 4, 2, 5, 1};    // Ладушки ИН-14
/*----- временно для проверки ------*/
//const byte digitMask[] = {8, 9, 0, 1, 5, 2, 4, 6, 7, 3};    // маска дешифратора платы COVID-19 (подходит для ИН-14 и ИН-12)
const byte opts[] = {KEY0, KEY1, KEY2, KEY3};               // порядок индикаторов слева направо
const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3};  // порядок катодов

#endif

/*
  ард ног ном
  А0  7   4
  А1  6   2
  А2  4   8
  А3  3   1
*/
