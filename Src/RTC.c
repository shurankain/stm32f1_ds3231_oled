#include "RTC.h"
#include "i2c.h"
#include "usart.h"

//--------------------------------------
char str[100];
uint8_t data[256];
uint16_t size;
uint8_t readBuf[7];

char* days[] = {
   "Sun",
   "Mon",
   "Tue",
   "Wed",
   "Thr",
   "Fri",
   "Sat"
};

uint8_t RTC_ConvertFromDec(uint8_t c) {
	uint8_t ch = ((c >> 4) * 10 + (0x0F & c));
	return ch;
}

uint8_t RTC_ConvertFromBinDec(uint8_t c) {
	uint8_t ch = ((c / 10) << 4) | (c % 10);
	return ch;
}

//--------------------------------------
void I2C_WriteBuffer(I2C_HandleTypeDef hi, uint8_t DEV_ADDR) {
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
}

char * readSeconds() {
	static char sec[2];
	uint8_t tempVal = readBuf[0];
	tempVal = RTC_ConvertFromDec(tempVal);
	sprintf(sec, "%c%c", ((tempVal / 10) % 10) + 0x30, (tempVal % 10) + 0x30);
	return sec;
}

char * readMinutes() {
	static char min[2];
	uint8_t tempVal = RTC_ConvertFromDec(readBuf[1]);
	sprintf(min, "%c%c", ((tempVal / 10) % 10) + 0x30, (tempVal % 10) + 0x30);
	return min;
}

char * readHours() {
	static char hour[2];
	uint8_t tempVal = RTC_ConvertFromDec(readBuf[2]);
	sprintf(hour, "%c%c", ((tempVal / 10) % 10) + 0x30, (tempVal % 10) + 0x30);
	return hour;
}

char * readDay() {
	static char day[1];
	uint8_t tempVal = RTC_ConvertFromDec(readBuf[3]);
	sprintf(day, "%c", (tempVal % 10) + 0x30);
	return day;
}

char * readDate() {
	static char date[2];
	uint8_t tempVal = RTC_ConvertFromDec(readBuf[4]);
	sprintf(date, "%c%c", ((tempVal / 10) % 10) + 0x30, (tempVal % 10) + 0x30);
	return date;
}

char * readMonth() {
	static char month[2];
	uint8_t tempVal = RTC_ConvertFromDec(readBuf[5]);
	sprintf(month, "%c%c", ((tempVal / 10) % 10) + 0x30, (tempVal % 10) + 0x30);
	return month;
}

char * readYear() {
	static char year[2];
	uint8_t tempVal = RTC_ConvertFromDec(readBuf[6]);
	sprintf(year, "%c%c", ((tempVal / 10) % 10) + 0x30, (tempVal % 10) + 0x30);
	return year;
}
