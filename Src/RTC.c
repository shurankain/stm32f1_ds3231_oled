#include "RTC.h"
#include "i2c.h"
#include "usart.h"

//--------------------------------------
char str[100];
uint8_t data[256];
uint16_t size;
uint8_t readBuf[7];

char* days[] = { "Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat" };

struct Calendar {
	uint8_t sec, min, hour, day, date, month, year;
} calendar;

uint8_t RTC_ConvertFromDec(uint8_t c) {
	uint8_t ch = ((c >> 4) * 10 + (0x0F & c));
	return ch;
}

uint8_t RTC_ConvertFromBinDec(uint8_t c) {
	uint8_t ch = ((c / 10) << 4) | (c % 10);
	return ch;
}

//--------------------------------------
void DS3231_sendData(I2C_HandleTypeDef hi, uint8_t DEV_ADDR) {
	uint8_t writeBuf[1];
	writeBuf[0] = 0;
	while (HAL_I2C_Master_Transmit(&hi, (uint16_t) DEV_ADDR,
			(uint8_t*) &writeBuf, sizeof(writeBuf), (uint32_t) 1000) != HAL_OK) {
		if (HAL_I2C_GetError(&hi) != HAL_I2C_ERROR_AF) {
			size = sprintf((char *) data, "DS3231 write failed\n");
			HAL_UART_Transmit(&huart1, data, size, 1000);
		}
	}
}
//--------------------------------------
void DS3231_setDate(I2C_HandleTypeDef hi, uint8_t DEV_ADDR) {
	uint8_t data[8];

	data[DS3231_CONTROL] = 0;// should be zero REGISTER SELECT from that will start
	data[DS3231_SECONDS] = calendar.sec;   //sec
	data[DS3231_MINUTES] = calendar.min;   //min
	data[DS3231_HOURS] = calendar.hour;  //hour
	data[DS3231_DAY] = calendar.day;   //day
	data[DS3231_YEAR] = calendar.year;  //year
	data[DS3231_MONTH] = calendar.month; //month
	data[DS3231_DATE] = calendar.date;  //date

	while (HAL_I2C_Master_Transmit(&hi, (uint16_t) DEV_ADDR, (uint8_t*) &data,
			8, (uint32_t) 1000) != HAL_OK) {
		if (HAL_I2C_GetError(&hi) != HAL_I2C_ERROR_AF) {
			size = sprintf((char *) data, "DS3231 write failed\n");
			HAL_UART_Transmit(&huart1, data, size, 1000);
		}
	}
}
//--------------------------------------
uint8_t * I2C_ReadRawData(I2C_HandleTypeDef hi, uint8_t DEV_ADDR) {
	while (HAL_I2C_Master_Receive(&hi, (uint16_t) DEV_ADDR, (uint8_t*) &readBuf,
			(uint16_t) 7, (uint32_t) 1000) != HAL_OK) {
		if (HAL_I2C_GetError(&hi) != HAL_I2C_ERROR_AF) {
			size = sprintf((char *) data, "DS3231 read failed\n");
			HAL_UART_Transmit(&huart1, data, size, 1000);
		}

	}
	return readBuf;
}

void I2C_ReadCalendarData(I2C_HandleTypeDef hi, uint8_t DEV_ADDR) {
	while (HAL_I2C_Master_Receive(&hi, (uint16_t) DEV_ADDR, (uint8_t*) &readBuf,
			(uint16_t) 7, (uint32_t) 1000) != HAL_OK) {
		if (HAL_I2C_GetError(&hi) != HAL_I2C_ERROR_AF) {
			size = sprintf((char *) data, "DS3231 read failed\n");
			HAL_UART_Transmit(&huart1, data, size, 1000);
		}
	}
	calendar.sec = readBuf[0];
	calendar.min = readBuf[1];
	calendar.hour = readBuf[2];
	calendar.day = readBuf[3];
	calendar.date = readBuf[4];
	calendar.month = readBuf[5];
	calendar.year = readBuf[6];
}

char * retrnAsString(uint8_t data) {
	static char res[2];
	uint8_t tempVal = RTC_ConvertFromDec(data);
	sprintf(res, "%c%c", ((tempVal / 10) % 10) + 0x30, (tempVal % 10) + 0x30);
	return res;
}

uint8_t readIntSeconds() {
	return calendar.sec;
}

char * readSeconds() {
	return retrnAsString(calendar.sec);
}

char * readMinutes() {
	return retrnAsString(calendar.min);
}

char * readHours() {
	return retrnAsString(calendar.hour);
}

char * readDay() {
	static char res[2];
	uint8_t tempVal = RTC_ConvertFromDec(calendar.day);
	sprintf(res, "%c", (tempVal % 10) + 0x30);
	return res;
}

char * readDate() {
	return retrnAsString(calendar.date);
}

char * readMonth() {
	return retrnAsString(calendar.month);
}

char * readYear() {
	return retrnAsString(calendar.year);
}

void setSeconds(uint8_t sec) {
	calendar.sec = sec;
}

void setMinutes(uint8_t min) {
	calendar.min = min;
}

void setHour(uint8_t hour) {
	calendar.hour = hour;
}

void setDay(uint8_t day) {
	calendar.day = day;
}

void setDate(uint8_t date) {
	calendar.date = date;
}

void setMonth(uint8_t month) {
	calendar.month = month;
}

void setYear(uint8_t year) {
	calendar.year = year;
}

