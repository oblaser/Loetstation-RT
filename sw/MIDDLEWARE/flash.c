//
// Oliver Blaser
//
// 10.05.2017
// 10.05.2017
//

#include "flash.h"

#include "stm32f0xx_hal.h"
#include "project.h"
#include "error.h"
#include "std_def.h"
#include "UART.h"

__attribute__((section(".EEPROM_DATA"))) uint32 FLASH_data;

extern uint16 FLASH_get_value(FLASH_data_pos select)
{
	switch(select)
	{
		case FLASH_M1:
			return ((FLASH_data & FLASH_mask_M1) >> FLASH_shift_M1);
			break;

		case FLASH_M2:
			return ((FLASH_data & FLASH_mask_M2) >> FLASH_shift_M2);
			break;

		case FLASH_Mact:
			return ((FLASH_data & FLASH_mask_Mact) >> FLASH_shift_Mact);
			break;

		case FLASH_Mselect:
			return ((FLASH_data & FLASH_mask_Mselect) >> FLASH_shift_Mselect);
			break;

		default:
			my_Error_Handler("flash.c - FLASH_get_value - unknown FLASH_data");
			return 0;
			break;
	}
}

extern void FLASH_set_value(FLASH_data_pos select, uint16 data)
{
	uint32 temporary = FLASH_data;

	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

	switch(select)
	{
		case FLASH_M1:
			temporary &= ~FLASH_mask_M1;
			temporary |= ((data << FLASH_shift_M1) & FLASH_mask_M1);
			break;

		case FLASH_M2:
			temporary &= ~FLASH_mask_M2;
			temporary |= ((data << FLASH_shift_M2) & FLASH_mask_M2);
			break;

		case FLASH_Mact:
			temporary &= ~FLASH_mask_Mact;
			temporary |= ((data << FLASH_shift_Mact) & FLASH_mask_Mact);
			break;

		case FLASH_Mselect:
			temporary &= ~FLASH_mask_Mselect;
			temporary |= ((data << FLASH_shift_Mselect) & FLASH_mask_Mselect);
			break;

		default:
			my_Error_Handler("flash.c - FLASH_set_value - unknown FLASH_data");
			break;
	}

	FLASH_PageErase(&FLASH_data);
	CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, &FLASH_data, temporary);

	HAL_FLASH_Lock();

#if(PRJ_DEBUG && FLASH_UART_ENABLE)
	UART_printf(&hUART_PROG, "flash.c - FLASH_set_value() - value: %d - data: %d\n", select, data);
#endif
}
