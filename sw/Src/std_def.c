/**
* @file std_def.c
*
* @see std_def.h
*
* Erstellt am 06.02.2017
* Erstellt durch Oliver Blaser
*
* Aktuelle Version: 1.02
*
* Version history:
* ver  --    date    --    author     -- Änderungen
* 1.00 -- 06.02.2017 -- Oliver Blaser -- 1. Release
* 1.01 -- 07.03.2017 -- Simon Walker  -- nop hinzugefügt, welches nicht optimiert wird.
* 1.02 -- 07.03.2017 -- Simon Walker  -- median Funktion hinzugefügt
*
*/

#include "std_def.h"

extern uint8 str_len(uint8 * string)
{
	uint8 len = 0;

	while(string[len] != '\n')
	{
		len++;
	}

	// "+1" um die anzahl Zeichen (mit \n) zu bekommen
	return len + 1;
}

extern __attribute__((optimize("O0"))) void nop(uint32 loops)
{
	volatile uint32_t _loops = loops;
	while(_loops)
	{
		_loops--;
	}
}

extern uint16_t median(uint16 *data, uint8 anzWerte, uint8 anzMittel)
{
	uint16_t value[anzWerte];
	uint16_t sortTemp = 0;
	uint32_t sum = 0;
	uint16_t result = 0;
	uint8_t i = 0;
	bool Change = true;
	for(i = 0;i<anzWerte;i++) //Array kopieren
	{

		value[i] = *data;
		data++;
	}

	//Der reihe nach Sortiern
	while(Change == true)
	{
		Change = false;
		for(i = 0; i<(anzWerte-1); i++)
		{
			//Wenn der Untere Wert grösser ist als der obere Wert...
			if(value[i] > value[i + 1])
			{//...Werden die Positionen getauschen
				sortTemp = value[i + 1];
				value[i + 1] = value[i];
				value[i] = sortTemp;
				Change = true;
			}
		}
	}
	//Durchschnitt berechnen
	result = 0;
	for (i = 0; i<anzMittel; i++)
	{

		sum += value[((anzWerte/2)-(anzMittel/2)+i)];
	}
	result = sum / anzMittel;
	return (result);
}
