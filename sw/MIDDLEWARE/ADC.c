#include "ADC.h"

#include "error.h"
#include "UART.h"

static uint16 ADC_Result[ADC_ANZ_MESSUNGEN];
static uint8 ADC_WandlungenCounter;
static bool measure_compleded;

extern void ADC_Init(void)
{
	ADC_ChannelConfTypeDef sConfig;

	ADC_WandlungenCounter = 0;

	__HAL_RCC_ADC1_CLK_ENABLE();

	/* ADC interrupt init */
	/* ADC_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(ADC1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC1_IRQn);

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	*/
	hadc.Instance = ADC1;
	hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
	hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc.Init.LowPowerAutoWait = DISABLE;
	hadc.Init.LowPowerAutoPowerOff = DISABLE;
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.DMAContinuousRequests = DISABLE;
	hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	if (HAL_ADC_Init(&hadc) != HAL_OK)
	{
		my_Error_Handler("ADC.c - ADC_Init() - !HAL_OK - 1");
	}

	/**Configure for the selected ADC regular channel to be converted.
	*/
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	{
		my_Error_Handler("ADC.c - ADC_Init() - !HAL_OK - 2");
	}
}

extern uint16 get_ADC_value()
{
	uint16 value;

#if(PRJ_DEBUG && ADC_UART_EN)
	UART_printf(&hUART_PROG, "ADC get_ADC_value\n");
#endif

	// start ADC
	ADC_WandlungenCounter = 0;
	measure_compleded = true;

	/*switch(HAL_ADC_Start_IT(&hadc))
	{
		case HAL_OK:

			UART_printf(&hUART_PROG, "\nADC.c - get_ADC_value() - HAL_OK\n\n");

			break;

		case HAL_BUSY:

			UART_printf(&hUART_PROG, "\nADC.c - get_ADC_value() - HAL_BUSY\n\n");

			break;

		case HAL_ERROR:

			my_Error_Handler("\nADC.c - get_ADC_value() - HAL_ERROR");

			break;

		case HAL_TIMEOUT:

			UART_printf(&hUART_PROG, "\nADC.c - get_ADC_value() - HAL_TIMEOUT\n\n");

			break;

		default:

			my_Error_Handler("ADC.c - get_ADC_value() - default");

			break;
	}*/


	if(HAL_ADC_Start_IT(&hadc) != HAL_OK) my_Error_Handler("ADC.c - get_ADC_value() - !HAL_OK");

#if(PRJ_DEBUG && ADC_UART_EN)
	UART_printf(&hUART_PROG, "ADC started\n");
#endif

	// wait for compleded
	while(measure_compleded == false)
	{
		//nop(1);
		__NOP();
#if(PRJ_DEBUG && ADC_UART_EN)
		UART_printf(&hUART_PROG, "w");
#endif
	}

	value = median(ADC_Result, ADC_ANZ_MESSUNGEN, 2);

#if(PRJ_DEBUG && ADC_UART_EN)
	for(uint8 i = 0; i < ADC_ANZ_MESSUNGEN; i++)
	{
		UART_printf(&hUART_PROG, "ADC results %d: %d\n", i, ADC_Result[i]);
	}
#endif

#if(PRJ_DEBUG && ADC_UART_EN)
	UART_printf(&hUART_PROG, "ADC median: %d\n\n", value);
#endif

	return value;
}

//
// IRQHandler
//
void ADC1_IRQHandler(void)
{
#if(PRJ_DEBUG && 0)
	UART_printf(&hUART_PROG, "ADC ADC1_IRQHandler %d\n", ADC_WandlungenCounter);
#endif

	HAL_ADC_IRQHandler(&hadc);

	if(ADC_WandlungenCounter < ADC_ANZ_MESSUNGEN)
	{
		if(HAL_ADC_Start_IT(&hadc) != HAL_OK) my_Error_Handler("solder_tip.c - HAL_ADC_ConvCpltCallback() - HAL_ADC_Start_IT() != HAL_OK");
	}
	else
	{
		measure_compleded = true;
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
#if(PRJ_DEBUG && 0)
	UART_printf(&hUART_PROG, "ADC HAL_ADC_ConvCpltCallback %d\n", ADC_WandlungenCounter);
#endif

	if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOC))
	{
		ADC_Result[ADC_WandlungenCounter] = HAL_ADC_GetValue(hadc);

#if(PRJ_DEBUG  && ADC_UART_EN)
		UART_printf(&hUART_PROG, "ADC_Result (HAL_ADC_ConvCpltCallback) %d: %d\n", ADC_WandlungenCounter, ADC_Result[ADC_WandlungenCounter]);
#endif

		ADC_WandlungenCounter++;
	}
}
