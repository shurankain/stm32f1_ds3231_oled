#ifndef RTC_H_
#define RTC_H_

#include "stm32f1xx_hal.h"

/* Registers location */
#define DS3231_CONTROL				0
#define DS3231_SECONDS				1
#define DS3231_MINUTES				2
#define DS3231_HOURS				3
#define DS3231_DAY					4
#define DS3231_DATE					5
#define DS3231_MONTH				6
#define DS3231_YEAR					7

uint8_t RTC_ConvertFromDec(uint8_t c);
uint8_t RTC_ConvertFromBinDec(uint8_t c);
void DS3231_sendData(I2C_HandleTypeDef hi, uint8_t DEV_ADDR);
void DS3231_setDate(I2C_HandleTypeDef hi, uint8_t DEV_ADDR);
uint8_t * I2C_ReadRawData(I2C_HandleTypeDef hi, uint8_t DEV_ADDR);
void I2C_ReadCalendarData(I2C_HandleTypeDef hi, uint8_t DEV_ADDR);

char * readSeconds();
char * readMinutes();
char * readHours();
char * readDay();
char * readDate();
char * readMonth();
char * readYear();

void setSeconds(uint8_t sec);
void setMinutes(uint8_t min);
void setHour(uint8_t hour);
void setDay(uint8_t day);
void setDate(uint8_t date);
void setMonth(uint8_t month);
void setYear(uint8_t year);

uint8_t readIntSeconds();

#endif /* RTC_H_ */
