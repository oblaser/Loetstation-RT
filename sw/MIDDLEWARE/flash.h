//
// Oliver Blaser
//
// 10.05.2017
// 10.05.2017
//

#ifndef _flash_H_
#define _flash_H_

#include "project.h"
#include "std_def.h"

#define FLASH_UART_ENABLE		0

#define FLASH_shift_M1			(4 * 0)
#define FLASH_shift_M2			(4 * 2)
#define FLASH_shift_Mact		(4 * 4)
#define FLASH_shift_Mselect		(4 * 6)

#define FLASH_mask_M1			0x000000FF
#define FLASH_mask_M2			0x0000FF00
#define FLASH_mask_Mact			0x00FF0000
#define FLASH_mask_Mselect		0x03000000

typedef enum
{
	FLASH_M1,
	FLASH_M2,
	FLASH_Mact,
	FLASH_Mselect
} FLASH_data_pos;

extern uint16 FLASH_get_value(FLASH_data_pos);
extern void FLASH_set_value(FLASH_data_pos, uint16);

#endif
