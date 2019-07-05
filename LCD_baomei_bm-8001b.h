#ifndef __SYMA_X5_TRANSMITTER_LCD
#define __SYMA_X5_TRANSMITTER_LCD

#include "stdint.h"


void SPI_Send (uint8_t value, uint8_t length);
uint8_t setbit(const uint8_t value, const uint8_t position);
int unsetbit(const int value, const int position);
uint8_t checkbit(const uint8_t value, const uint8_t position);

void LCD_Init(void);
void LCD_Update(void);

// =============================================== //

// ---------------- pos = -5 -- 5 --------------//
void TrimThrottle(int8_t pos);
void TrimRudder(int8_t pos);
void TrimEler(int8_t pos);
void TrimElev(int8_t pos);
// ---------------- pos = -5 -- 5 --------------//

void SetMode(int8_t mode); // 1 -> mode1, 2 -> mode2
void DoubleRate(int8_t dr); // 0 -> L, 1 -> H
void Bat(int8_t pos); // 0 - 4, 0 -> low, 4 -> full
void Ant(int8_t on); // 0 -> off, 1 -> on
void Quadro(int8_t on); // 0 -> off, 1 -> on
void Throtle(uint8_t throttle); // 0 -- 11
void Elevator(int8_t pos); // -5 -- 5
void Eleron(int8_t pos); // -5 -- 5
void Rudder(int8_t pos); // -7 -- 7



#endif /* __SYMA_X5_TRANSMITTER_LCD */
