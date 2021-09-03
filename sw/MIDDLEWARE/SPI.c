//
// Oliver Blaser
//
// 10.05.2017
// 10.05.2017
//

#include "SPI.h"

#include "stm32f0xx_hal.h"
#include "project.h"
#include "error.h"
#include "std_def.h"
#include "UART.h"

SPI_HandleTypeDef hspi1;

static bool is_init = false;

extern void SPI1_Init(void)
{
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_LSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		my_Error_Handler("SPI.c - SPI1_Init() - !HAL_OK");
	}

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	is_init = true;
}

extern void SPI_send_len(uint8 * data, uint16 len)
{
	HAL_StatusTypeDef retval = HAL_OK;

	if(is_init != false) retval = HAL_SPI_Transmit(&hspi1, data, len, 1000);

#if(PRJ_DEBUG && 0)
	UART_printf(&hUART_PROG, "SPI.c - SPI_send_len() - retval: %d\n", retval);
#endif

	if(retval != HAL_OK) my_Error_Handler("SPI.c - SPI_send_len() - !HAL_OK");
}
