/**
* @file std_def.h
*
* @see std_def.c
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


#ifndef __std_def_h
#define __std_def_h

#include <stdint.h>

typedef enum
{
	false	= 0,
	true	= 1
} bool;

typedef uint8_t uint8;
typedef int8_t int8;

typedef uint16_t uint16;
typedef int16_t int16;

typedef uint32_t uint32;
typedef int32_t int32;

extern uint8 str_len(uint8 * string);
extern void nop(uint32 loops);
extern uint16_t median(uint16 *data, uint8 anzWerte, uint8 anzMittel);

#endif /* __std_def_h__ */
