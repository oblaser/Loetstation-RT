#include "stm32f0xx_hal.h"
#include "project.h"
#include "std_def.h"
#include "settings.h"

TIM_HandleTypeDef htim14;


#define PWM_FREQ				300		// Hz

///////////////////////////////////////////////////////////
//
// end User settings
//
///////////////////////////////////////////////////////////

#define PWM_MAX_PERIOD			900

#define PWM_PS					(36000 / PWM_FREQ)

#if((PWM_PS > 65535) || (PWM_PS < 0))
#error "non valid Prescaler value!"
#endif

#define PWM_UART_ENABLE			0

extern void TIM14_Init(void);

extern void PWM_start();

extern void change_PWM(uint16);

extern uint16 change_PWM_add(int16);
