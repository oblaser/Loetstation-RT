#include "stm32f0xx_hal.h"
#include "project.h"
#include "std_def.h"

#define ADC_ANZ_MESSUNGEN	20

#define ADC_UART_EN			0

ADC_HandleTypeDef hadc;

extern void ADC_Init(void);
extern uint16 get_ADC_value();
