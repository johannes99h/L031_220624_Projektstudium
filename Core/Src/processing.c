#include "processing.h"

/* Definition der Variablen ---------------------------------------------------------*/

const static uint16_t 	LUT[151] = {		// mit Näherungsformel berechnete Widerstandswerte in 1-Grad-Schritten für Murata NXFT15XH103-NTCs
											28224, 	26978, 	25796, 	24674, 	23608, 	22595, 	21632, 	20717, 	19847, 	19019,
											18231, 	17481, 	16767, 	16087, 	15438, 	14820, 	14231, 	13669, 	13133, 	12622,
											12133, 	11667, 	11221, 	10796, 	10389, 	10000, 	9628, 	9272, 	8932, 	8606,
											8295,	7996,	7710,	7436,	7174,	6922,	6681,	6449, 	6227,	6014,
											5810,	5614,	5425,	5244,	5070,	4903,	4743,	4589,	4440,	4297,
											4160,	4028,	3901,	3779,	3661,	3547,	3438,	3333,	3231,	3133,
											3039,	2948,	2861,	2776,	2695,	2616,	2540,	2467,	2396,	2327,
											2261,	2197,	2136,	2076,	2019,	1963,	1909,	1857,	1807,	1758,
											1711,	1665,	1621,	1578,	1537,	1497,	1458,	1420,	1384,	1349,
											1315,	1281,	1249,	1218,	1188,	1158,	1130,	1102,	1076,	1050,
											1024,	1000,	976,	953,	930,	909,	888,	867,	847,	828,
											809,	790,	772,	755,	738,	722,	706,	690,	675,	660,
											646,	632,	619,	605,	593,	580,	568,	556,	544,	533,
											522,	511,	501,	491,	481,	471,	462,	452,	443,	435,
											426,	418,	410,	402,	394,	386,	379,	372,	365,	358,
											351
									};

			uint16_t 	LUToldValues[9] = { 0 };


/* Definition der Funktionen ---------------------------------------------------------*/

/*
 * 	@brief	Überfunktion zur Ermittlung der Temperaturen aus dem Array
 * 	@param	Pointer zum Temperaturarray
 * 	@param	Anzahl der Channel
 * 	@param	Pointer zum Lookup-Table der NTC-Temperaturen
 * 	@param	Pointer zum Array der ermittelten NTC-Widerstände
 * 	@param	Pointer zum Array der im vorherigen Zyklus ermittelten Temperaturen
 * 	@ret	gesamtes Temperatur-Array
 */

uint8_t *GetTempCArray(uint8_t *tempC, uint8_t adcChannel, const uint16_t *LUT, uint16_t *ntcResistance, uint16_t *LUToldValues)
{
	uint16_t startIndex = 0;

	for(int i = 0; i < adcChannel; i++)
	{
		// Zuweisung des Start-Indizes
		startIndex = LUToldValues[i];

		// Aufruf der Funktion, die den berechneten NTC-Widerstand mit dem LUT vergleicht
		tempC[i] = GetTempCfromLUT(LUT, ntcResistance[i], startIndex);

		// Speichern des LUT-Indizes (welcher zufällig auch der Temp. entspricht) als "Startwert" für den nächsten Zyklus
		LUToldValues[i] = tempC[i];
	}

	return tempC;
}


/*
 * 	@brief	Temperaturermittlung durch Abgleich von Verhältnis zwischen gemessenem und normalem NTC-Widerstand (298K)
 * 	@param	Pointer zum Lookup-Table
 * 	@param	Übergabe des durch den ADC ermittelten Widerstands eines NTCs
 * 	@ret	ermittelte Temperatur in Grad Celsius
 */

uint8_t GetTempCfromLUT(const uint16_t *LUT, uint16_t ntcResistance, uint8_t startIndex)
{
	// int i = 0;
	uint8_t tempC;

	// zusätzl. Fkt. die den Wert "-1" im tempC-Array verbietet
	if(ntcResistance == 0)
	{
		tempC = 0;
		return tempC;
	}
	else if(ntcResistance < LUT[0] && ntcResistance > LUT[151])
	{
		// "Suchalgorithmus"
		if(LUT[startIndex] > ntcResistance)
		{
			while(ntcResistance < LUT[startIndex])
			{
				startIndex++;
			}
		}
		else if(LUT[startIndex] < ntcResistance)
		{
			while(ntcResistance > LUT[startIndex])
			{
				startIndex--;
			}
		}

		// Temperaturzuweisung
		tempC = startIndex;

		// Plausibilitätskontrolle
		if(tempC < 0 || tempC > 150)
		{
			tempC = 255;
		}
	}
	return tempC;
}


