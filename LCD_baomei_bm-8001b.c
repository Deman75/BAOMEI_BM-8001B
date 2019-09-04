#include "LCD_baomei_bm-8001b.h"
#include "main.h"

#define LCD_CS_0 HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET)
#define	LCD_CS_1 HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET)

#define MOSI_1 HAL_GPIO_WritePin(LCD_DA_GPIO_Port, LCD_DA_Pin, GPIO_PIN_SET)
#define MOSI_0 HAL_GPIO_WritePin(LCD_DA_GPIO_Port, LCD_DA_Pin, GPIO_PIN_RESET)

#define CLK_1	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET)
#define CLK_0	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_RESET)

uint8_t data[16] = {0x00, 0x00, 0x09, 0x02, 0x80, 0x80, 0x00, 0x28, 0x00, 0x00, 0x90, 0x82, 0x00, 0x08, 0x88, 0x20};


void SPI_Send (uint8_t value, uint8_t length) {
	int8_t i;

	for (i = length - 1; i >= 0; i--) {
		CLK_0;

		if (checkbit(value, i)) {
			MOSI_1;
		} else {
			MOSI_0;
		}

		CLK_1;
	}
}

void LCD_Init() {

	int8_t i, j;

	uint8_t init[8] = {0x52, 0x30, 0x00, 0x0A, 0x02, 0x06, 0xC0, 0x10};
	uint8_t bufInit[16] = {0x00, 0x00, 0x09, 0x02, 0x80, 0x80, 0x00, 0x28, 0x00, 0x00, 0x90, 0x82, 0x00, 0x08, 0x88, 0x20};

	for (i = 0; i < 8; i++) {
		LCD_CS_0;
		SPI_Send(0x08, 4);
		for (j = 0; j < 5; j++);
		SPI_Send(init[i], 8);
		LCD_CS_1;
	}

	LCD_CS_0;

	SPI_Send(0x05, 3);
	SPI_Send(0x00, 6);

	for (i = 0; i < 16; i++) {
		SPI_Send(bufInit[i], 8);
		for (j = 0; j < 10; j++);
	}

	LCD_CS_1;

	MOSI_1;

}


uint8_t checkbit(const uint8_t value, const uint8_t position) {
    return ((value & (1 << position)) != 0);
}


uint8_t setbit(const uint8_t value, const uint8_t position) {
    return (value | (1 << position));
}


int unsetbit(const int value, const int position) {
    return (value & ~(1 << position));
}

void LCD_Update() {
	uint8_t i;
	LCD_CS_0;

	SPI_Send(0x05, 3);
	SPI_Send(0x00, 6);

	for (i = 0; i < 16; i++) {
		SPI_Send(data[i], 8);
	}

	LCD_CS_1;
}

// =========================================================== //


void TrimThrottle(int8_t pos) {
	uint8_t posV[11] = {3,2,1,0,4,5,6,7,2,1,0};

	data[15] = 0;
	data[14] = unsetbit(data[14], 0);
	data[14] = unsetbit(data[14], 1);
	data[14] = unsetbit(data[14], 2);


	if (pos + 5 > 7)
		data[14] = setbit(data[14], posV[pos + 5]);
	else
		data[15] = setbit(data[15], posV[pos + 5]);
}

void TrimElev(int8_t pos) {
	uint8_t posV[11] = {7,6,5,4,0,1,2,3,6,5,4};

	data[3] = 0;
	data[4] = setbit(data[4], 7);
	data[4] = unsetbit(data[4], 4);
	data[4] = unsetbit(data[4], 5);
	data[4] = unsetbit(data[4], 6);


	if (pos + 5 > 7)
		data[4] = setbit(data[4], posV[pos + 5]);
	else
		data[3] = setbit(data[3], posV[pos + 5]);
}

void TrimEler(int8_t pos) {
	uint8_t posV[11] = {0,1,2,7,6,5,4,0,1,2,3};

	data[7] = 0;
	data[7] = setbit(data[7], 3);
	data[6] = unsetbit(data[6], 0);
	data[6] = unsetbit(data[6], 1);
	data[6] = unsetbit(data[6], 2);
	data[6] = unsetbit(data[6], 3);

	if (pos + 5 > 6)
		data[6] = setbit(data[6], posV[pos + 5]);
	else
		data[7] = setbit(data[7], posV[pos + 5]);
}

 //----------------------------------------------------------//
void SetMode(int8_t mode) { // 1 or 2
	if (mode == 1) {
		data[14] = setbit(data[14], 5);
		data[14] = unsetbit(data[14], 4);
	} else if (mode == 2) {
		data[14] = setbit(data[14], 4);
		data[14] = unsetbit(data[14], 5);
	}
}

//------------------------------------------------------------//
void Throtle(uint8_t throttle){
	uint8_t posLeft[11]  = {3,2,1,0,4,5,6,7,2,1,0};
	uint8_t posRight[11] = {7,6,5,4,0,1,2,3,6,5,4};

	data[13] = 0;
	data[13] = setbit(data[13], 3);
	data[5] = 0;
	data[5] = setbit(data[5], 7);
	data[12] = unsetbit(data[12], 3);
	data[12] = unsetbit(data[12], 2);
	data[12] = unsetbit(data[12], 1);
	data[12] = unsetbit(data[12], 0);
	data[6] = unsetbit(data[6], 7);
	data[6] = unsetbit(data[6], 6);
	data[6] = unsetbit(data[6], 5);
	data[6] = unsetbit(data[6], 4);

	int8_t i;

	for (i = 0; i < throttle; i++) {
		if (i < 4) {
			data[12] = setbit(data[12], posLeft[i]);
			data[6] = setbit(data[6], posRight[i]);
		} else {
			data[13] = setbit(data[13], posLeft[i]);
			data[5] = setbit(data[5], posRight[i]);
		}
	}
}

//------------------------------------------------------------//

void Elevator(int8_t pos){
	uint8_t posE[11]  = {3,2,1,0,6,4,5,4,5,6,7};

	data[10] = 0;
	data[10] = setbit(data[10], 7);

	data[9] = unsetbit(data[9], 7);
	data[9] = unsetbit(data[9], 6);
	data[9] = unsetbit(data[9], 5);
	data[9] = unsetbit(data[9], 4);

	int8_t i;

	if (pos >= 0) {
		for (i = 5; i <= pos + 5; i++) {
			if (i > 6) {
				data[9] = setbit(data[9], posE[i]);
			} else {
				data[10] = setbit(data[10], posE[i]);
			}
		}
	} else {
		for (i = 5; i >= pos + 5; i--) {
			if (i > 6) {
				data[9] = setbit(data[9], posE[i]);
			} else {
				data[10] = setbit(data[10], posE[i]);
			}
		}
	}
}

//-----------------------------------------------------------------//

void Eleron(int8_t pos) {
	uint8_t posE[11]  = {3,7,6,5,4,4,0,1,2,3,3};

	data[1] = unsetbit(data[1], 3);
	data[1] = unsetbit(data[1], 7);
	data[1] = unsetbit(data[1], 6);
	data[1] = unsetbit(data[1], 5);
	data[1] = unsetbit(data[1], 4);
	data[9] = unsetbit(data[9], 0);
	data[9] = unsetbit(data[9], 1);
	data[9] = unsetbit(data[9], 2);
	data[9] = unsetbit(data[9], 3);
	data[8] = unsetbit(data[8], 3);

	int8_t i;

	if (pos >= 0) {
		for (i = 5; i <= pos + 5; i++) {
			if (i > 5 && i < 10) {
				data[9] = setbit(data[9], posE[i]);
			} else if (i == 10) {
				data[8] = setbit(data[8], posE[i]);
			}
		}
	} else {
		for (i = 5; i >= pos + 5; i--) {
			data[1] = setbit(data[1], posE[i]);
		}
	}
}

//----------------------------------------------------------------//

void Rudder(int8_t pos){
	uint8_t posE[14]  = {0,1,2,4,5,6,7,7,6,5,4,2,1,0};
	int8_t i;
	for (i = 0; i < 8; i++) {
		if (i != 3)
			data[8] = unsetbit(data[8], i);
	}
	data[1] = unsetbit(data[1], 0);
	data[1] = unsetbit(data[1], 1);
	data[1] = unsetbit(data[1], 2);
	data[2] = unsetbit(data[2], 4);
	data[2] = unsetbit(data[2], 5);
	data[2] = unsetbit(data[2], 6);
	data[2] = unsetbit(data[2], 7);


	if (pos >= 0) {
		for (i = 7; i <= pos + 7; i++) {
			data[8] = setbit(data[8], posE[i]);
		}
	} else {
		for (i = 6; i >= pos + 6; i--) {
			if (i > 2) {
				data[2] = setbit(data[2], posE[i]);
			} else {
				data[1] = setbit(data[1], posE[i]);
			}
		}
	}
}

//----------------------------------------------------------------//
void TrimRudder(int8_t pos){
	uint8_t posV[11] = {7,6,5,4,0,1,2,3,6,5,4};

	data[11] = 0;
	data[11] = setbit(data[11], 7);
	data[12] = unsetbit(data[12], 4);
	data[12] = unsetbit(data[12], 5);
	data[12] = unsetbit(data[12], 6);
	data[12] = unsetbit(data[12], 7);

	if (pos + 5 < 4)
		data[12] = setbit(data[12], posV[pos + 5]);
	else
		data[11] = setbit(data[11], posV[pos + 5]);
}

//---------------------------------------------------------------//

void DoubleRate(int8_t dr) {
	if (dr == 0) {
		data[2] = unsetbit(data[2], 1);
		data[2] = setbit(data[2], 2);
	} else {
		data[2] = setbit(data[2], 1);
		data[2] = unsetbit(data[2], 2);
	}
}

//----------------------------------------------------------------//

void Bat(int8_t pos){
	uint8_t posV[4]  = {3,2,1,0};

	data[4] = unsetbit(data[4], 3);
	data[4] = unsetbit(data[4], 2);
	data[4] = unsetbit(data[4], 1);
	data[4] = unsetbit(data[4], 0);

	int8_t i;

	for (i = 0; i < pos; i++) {
		if (i < 4) {
			data[4] = setbit(data[4], posV[i]);
		}
	}
}

//---------------------------------------------------------------//

void Ant(int8_t on) {
	if (on)
		data[14] = setbit(data[14], 6);
	else
		data[14] = unsetbit(data[14], 6);
}

//-------------------------------------------------------------//

void Quadro(int8_t on){
	if (on)
		data[2] = setbit(data[2], 3);
	else
		data[2] = unsetbit(data[2], 3);
}
