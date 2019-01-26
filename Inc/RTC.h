#ifndef RTC_H_
#define RTC_H_

#include "stm32f1xx_hal.h"
//char* days[] = {
//   "Sun",
//   "Mon",
//   "Tue",
//   "Wed",
//   "Thu",
//   "Fri",
//   "Sat"
//};


uint8_t RTC_ConvertFromDec(uint8_t c);
uint8_t RTC_ConvertFromBinDec(uint8_t c);
void I2C_WriteBuffer(I2C_HandleTypeDef hi, uint8_t DEV_ADDR);
uint8_t * I2C_ReadRawData(I2C_HandleTypeDef hi, uint8_t DEV_ADDR);
void I2C_ReadCalendarData(I2C_HandleTypeDef hi, uint8_t DEV_ADDR);

char * readSeconds();
char * readMinutes();
char * readHours();
char * readDay();
char * readDate();
char * readMonth();
char * readYear();

#endif /* RTC_H_ */
