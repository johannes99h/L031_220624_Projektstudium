 #include "transmission.h"

/* Definition der Variablen ---------------------------------------------------------*/

uint8_t 	adcChannels = 9;
uint8_t 	howmany; 				// Anzahl der Sternchen!
uint32_t	millis = 0;


/* Definition der Funktionen ---------------------------------------------------------*/

/*
 * 	@brief Senden der NTC-Nummer, der Temperatur, des zugehörigen CRCs & mehrerer "Stop"-Zeichen
 * 		***** NTC1 * 25°C * CRC1 ** NTC2 * 30°C * CRC2 ** ... **********
 */

void TxUART(uint8_t adcChannels, uint8_t *tempC, uint32_t millis)
{
	uint8_t ntcNumber = 0;

	// "Header"
	blankTxUART(5);

	// "Timer-Information"
	timeTxUART(millis);
	blankTxUART(2);

	for(int j = 0; j < adcChannels; j++)
	{
		// Übergabe der Temperatur aus dem Array an temp. Integer
		uint8_t *tmpTempC = &tempC[j];

		// Aufruf der eigentlichen Übertragungsfkt.
		singleTempTxUART(ntcNumber, tmpTempC);

		blankTxUART(2);
	}

	// Ende der Nachricht -> NewLine um in Matlab mit den Daten besser umgehen zu können
	uint8_t endLine[2] = "\n\r";			// evtl. auch CR/ASCII-13 notw. um an Zeilenanfang zu gelangen
	HAL_UART_Transmit(&huart2, endLine, sizeof(endLine), 10);
}


void singleTempTxUART(uint8_t ntcNumber, uint8_t *tempC)
{
	uint8_t bufferTempSize;
	char bufferTemp[3];

	bufferTempSize = sprintf(bufferTemp, "%d", tempC[ntcNumber]);

	HAL_UART_Transmit(&huart2, (uint8_t *) bufferTemp, bufferTempSize, 10);
}


/*
 * 	@brief	Übertragung der Nummer des zugehörigen NTCs
 * 	@param	Nummer des NTCs
 */

void ntcNumberTxUART(uint8_t ntcNumber)
{
	uint8_t bufferNtcNumberSize;
	char bufferNtcNumber[5];

	bufferNtcNumberSize = sprintf(bufferNtcNumber, "NTC%d", ntcNumber);


	HAL_UART_Transmit(&huart2, (uint8_t *)bufferNtcNumber, bufferNtcNumberSize, 10);
	// beim Empfang mittels Bitshift zerlegen & ablehnen, falls Bits vor der ersten 1 ungleich Null sind!
}


/*
 * 	@brief "Zwischenframe", Häufigkeit des Vorkommens ermöglicht Schluss auf Fortschritt der Nachricht
 * 	@param	Anzahl der Zwischenframes (Stern in ASCII-Codierung)
 */

void blankTxUART(uint8_t howmany)
{
	uint8_t divider[1] = " ";

	for(int i = 0; i < howmany; i++)
	{
		HAL_UART_Transmit(&huart2, divider, sizeof(divider), 10);
	}
}

/*
 * 	@brief 	CRC-Frame für je eine NTC-Temperatur
 * 	@param	Nummer des entsprechenden NTCs (ganzes Array wird übergeben)
 * 	@param	Array mit generierten CRC-Werten
 */

void timeTxUART(uint32_t millis)
{
	uint8_t bufferTimeSize;
	char bufferTime[8];

	bufferTimeSize = sprintf(bufferTime, "%ld", millis);

	HAL_UART_Transmit(&huart2, (uint8_t *)bufferTime, bufferTimeSize, 10);
}

/*
 * Einstellungen für Logic Analyzer
 * 		2400 Bit/s
 * 		7 Bits per Transfer
 * 		1 Stop Bit
 * 		Even Parity
 * 		LSB First
 */
