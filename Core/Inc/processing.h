/*
 * processing.h
 *
 *  Created on: Jul 6, 2021
 *      Author: johan
 */

#ifndef INC_PROCESSING_H_
#define INC_PROCESSING_H_

#include "stdint.h"
#include "stm32l0xx_hal.h"

/* Deklaration der Variablen ---------------------------------------------------------*/

const 	uint16_t LUT[151];
		uint32_t tempCRC;
		uint16_t LUToldValues[9];


/* Deklaration der Funktionen ---------------------------------------------------------*/

uint8_t 	*GetTempCArray(uint8_t *tempC, uint8_t adcChannel, const uint16_t *LUT, uint16_t *ntcResistance, uint16_t *LUToldValues);
uint8_t 	GetTempCfromLUT(const uint16_t *LUT, uint16_t ntcResistance, uint8_t startIndex);

#endif /* INC_PROCESSING_H_ */
