#include "RTC.h"
#include "i2c.h"
#include "usart.h"

//--------------------------------------
char str[100];
uint8_t data[256];
uint16_t size;
uint8_t readBuf[7];

char* days[] = { "Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat" };

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

	data[DS3231_CONTROL] = 0b00000000;		  //lol =) should be zero REGISTER SELECT from that will start
	data[DS3231_SECONDS] = 0x00;              //sec
	data[DS3231_MINUTES] = 0x53;              //min
	data[DS3231_HOURS] = 0x00;	              //hour
	data[DS3231_DAY] = 0x1;	                  //day
	data[DS3231_YEAR] = 0x19;                 //year
	data[DS3231_MONTH] = 0x01;                //month
	data[DS3231_DATE] = 0x27;                 //date

	while (HAL_I2C_Master_Transmit(&hi, (uint16_t) DEV_ADDR,
			(uint8_t*) &data, 8, (uint32_t) 1000) != HAL_OK) {
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

char * readTwoBytesAt(int addr) {
	static char res[2];
	uint8_t tempVal = readBuf[addr];
	tempVal = RTC_ConvertFromDec(tempVal);
	sprintf(res, "%c%c", ((tempVal / 10) % 10) + 0x30, (tempVal % 10) + 0x30);
	return res;
}

char * readSeconds() {
	return readTwoBytesAt(0);
}

char * readMinutes() {
	return readTwoBytesAt(1);
}

char * readHours() {
	return readTwoBytesAt(2);
}

char * readDay() {
	static char day[1];
	uint8_t tempVal = RTC_ConvertFromDec(readBuf[3]);
	sprintf(day, "%c", (tempVal % 10) + 0x30);
	return day;
}

char * readDate() {
	return readTwoBytesAt(4);
}

char * readMonth() {
	return readTwoBytesAt(5);
}

char * readYear() {
	return readTwoBytesAt(6);
}
