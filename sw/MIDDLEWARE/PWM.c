#include "PWM.h"

#include "error.h"
#include "UART.h"

#include "solder_tip.h"		// fuer interrupt

TIM_OC_InitTypeDef sConfigOC;

extern void TIM14_Init(void)
{
	__HAL_RCC_TIM14_CLK_ENABLE();

	//HAL_NVIC_SetPriority(TIM14_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(TIM14_IRQn);

	htim14.Instance = TIM14;
	htim14.Init.Prescaler = PWM_PS;
	htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim14.Init.Period = 1000;
	htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
	{
		my_Error_Handler("solder_tip.c - TIM14_Init() - !HAL_OK - 1");
	}

	if (HAL_TIM_PWM_Init(&htim14) != HAL_OK)
	{
		my_Error_Handler("solder_tip.c - TIM14_Init() - !HAL_OK - 2");
	}

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		my_Error_Handler("solder_tip.c - TIM14_Init() - !HAL_OK - 3");
	}

	//
	// post msp init
	//
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF4_TIM14;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

extern void PWM_start()
{
	//HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start_IT(&htim14, TIM_CHANNEL_1);
}

extern void change_PWM(uint16 value) // 0..1000[pro mille]
{
	if(sConfigOC.Pulse != value)
	{
		sConfigOC.Pulse = value;

		HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);

		if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
		{
			my_Error_Handler("solder_tip.c - change_PWM() - !HAL_OK");
		}

		HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
	}
}

extern uint16 change_PWM_add(int16 value) // 0..1000[pro mille]
{
#if(PRJ_DEBUG && PWM_UART_ENABLE)
	UART_printf(&hUART_PROG, "PWM.c - change_PWM_add() - Pulse: %d\n", sConfigOC.Pulse);
	UART_printf(&hUART_PROG, "PWM.c - change_PWM_add() - value: %d\n", value);
#endif

	if((int16)(sConfigOC.Pulse + value) > (int16)PWM_MAX_PERIOD) sConfigOC.Pulse = PWM_MAX_PERIOD;
	else if((int16)(sConfigOC.Pulse + value) < (int16)0) sConfigOC.Pulse = 0;
	else sConfigOC.Pulse += value;

#if(PRJ_DEBUG && PWM_UART_ENABLE)
	UART_printf(&hUART_PROG, "PWM.c - change_PWM_add() - Pulse: %d\n", sConfigOC.Pulse);
#endif

	HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);

	if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		my_Error_Handler("solder_tip.c - change_PWM() - !HAL_OK");
	}

	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);

	return sConfigOC.Pulse;
}

void TIM14_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim14);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM14)
	{
		//HAL_GPIO_TogglePin(Prog_IO);
		//enable_measure();
	}
}
