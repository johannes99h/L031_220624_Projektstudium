#ifndef INC_TRANSMISSION_H_
#define INC_TRANSMISSION_H_

#include "stdio.h"
#include "stdint.h"
#include "stm32l0xx_hal.h"


/* externe Definitionen ---------------------------------------------------------*/

extern UART_HandleTypeDef	huart2;
extern DMA_HandleTypeDef 	hdma_usart2_tx;
// extern TIM_HandleTypeDef 	htim21;


/* Deklaration der Variablen ---------------------------------------------------------*/

uint8_t 	divider[1];
uint8_t 	adcChannels;
uint8_t 	howmany;
uint32_t	millis;

/* Deklaration der Funktionen ---------------------------------------------------------*/

void	TxUART(uint8_t adcChannels, uint8_t *tempC, uint32_t millis);
void	singleTempTxUART(uint8_t ntcNumber, uint8_t *transmitTemp);
void	ntcNumberTxUART(uint8_t ntcNumber);
void	blankTxUART(uint8_t howmany);
void 	timeTxUART(uint32_t millis);

#endif /* INC_TRANSMISSION_H_ */
