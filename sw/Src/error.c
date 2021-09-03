/*
 * Oliver Blaser
 *
 * c: 22.03.2017
 * e: 23.04.2017
 *
 */

#include "error.h"

#include "stm32f0xx_hal.h"
#include "project.h"
#include "HW.h"
#include "UART.h"
#include "PWM.h"
#include "display.h"

extern void my_Error_Handler(char * str)
{
	change_PWM(0);

	__DISP_WRITE_ENABLE();
	display(DISP_IMG_Err);

#if(PRJ_DEBUG)
	UART_printf(&hUART_PROG, "\n\nError Handler - Error Message:\n");
	UART_printf(&hUART_PROG, str);
	UART_printf(&hUART_PROG, "\n");
#endif

	while(1)
	{
#if(PRJ_PROG_IO_IN)
		if(HAL_GPIO_ReadPin(Prog_IO) == 0)
		{
			UART_printf(&hUART_PROG, "\n\nError Handler - Error Message:\n");
			UART_printf(&hUART_PROG, str);
			UART_printf(&hUART_PROG, "\n");
		}
#endif
		HAL_GPIO_WritePin(LED_Status, 1);
		HAL_Delay(100);
		HAL_GPIO_WritePin(LED_Status, 0);
		HAL_Delay(200);
		HAL_GPIO_WritePin(LED_Status, 1);
		HAL_Delay(100);
		HAL_GPIO_WritePin(LED_Status, 0);
		HAL_Delay(200);
		HAL_GPIO_WritePin(LED_Status, 1);
		HAL_Delay(100);
		HAL_GPIO_WritePin(LED_Status, 0);
		HAL_Delay(1000);
	}
}
