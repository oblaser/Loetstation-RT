//
// Oliver Blaser
//
// 10.05.2017
// 10.05.2017
//

#include "solder_tip.h"

#include "stm32f0xx_hal.h"
#include "project.h"
#include "error.h"
#include "HW.h"
#include "std_def.h"
#include "UART.h"
#include "PWM.h"
#include "ADC.h"
#include "display.h"

static bool started = false;
//static bool measure_en = false;

static uint16 temp_soll = 0;
static uint16 temp_ist = 0;

static uint32 tmr_regel = 0;
static uint32 tmr_cooldown = 0;

static uint16 pulse_value = 0;
static uint32 tmr_pulse_value = 0;

static uint16 calc_float(uint16 adc)
{
	float result;

	result = ((float)0.1282 * (float)adc + (float)12.42) / (float)5;

	return (uint16)result;
}

static void solder_tip_task()
{
	int16 diff;

#if(PRJ_DEBUG && TASK_UART_ENABLE)
	UART_printf(&hUART_PROG, "\n+++++++++++++++++++++++++++++++\n");
#endif

	//
	// measure
	//
	//if(TIM14->CNT >= TIM14->CCR1)	// wenn PWM gerade 0 ist
	//{
		temp_ist = get_ADC_value();

#if(PRJ_DEBUG && TASK_UART_ENABLE && 0)
		UART_printf(&hUART_PROG, "solder_tip.c - solder_tip_measure() - adc: %d\n", temp_ist);
#endif

		temp_ist = calc_float(temp_ist);
	//}

#if(PRJ_DEBUG && TASK_UART_ENABLE && 0)
	UART_printf(&hUART_PROG, "solder_tip.c - solder_tip_measure() - Temp ist:  %d => %d\n", temp_ist, temp_ist * 5);
	UART_printf(&hUART_PROG, "solder_tip.c - solder_tip_measure() - Temp soll: %d => %d\n", temp_soll, temp_soll * 5);
#endif

	//
	// diff
	//
	diff = temp_soll - temp_ist;

#if(PRJ_DEBUG && TASK_UART_ENABLE)
	UART_printf(&hUART_PROG, "solder_tip.c - solder_tip_task() - diff: %d\n", diff);
#endif

	if(diff > 0) diff *= FAKTOR_POS;
	else diff *= FAKTOR_NEG;

#if(PRJ_DEBUG && TASK_UART_ENABLE)
	UART_printf(&hUART_PROG, "solder_tip.c - solder_tip_task() - diff: %d\n", diff);
#endif

	//
	// modify pulse
	//
#if(PRJ_DEBUG && TASK_UART_ENABLE)
	pulse_value = change_PWM_add(diff);
	UART_printf(&hUART_PROG, "solder_tip.c - solder_tip_task() - pulse: %d\n", pulse_value);
#else
	pulse_value = change_PWM_add(diff);
#endif

#if(PRJ_DEBUG && TASK_UART_ENABLE)
	UART_printf(&hUART_PROG, "+++++++++++++++++++++++++++++++\n");
#endif
}

static void pulse_value_watch_dog()
{
#if(PRJ_DEBUG && WATCHDOG_UART_ENABLE)
	uint8 x = (tmr_pulse_value < T_DUTCY_MAX);
	uint8 y = (temp_soll >= TEMP_EN_PULSE_WATCHDOG);
	uint8 z = (pulse_value >= DUTCY_MAX_OVER_TIME);
#endif

	if((tmr_pulse_value < T_DUTCY_MAX) && (temp_soll >= TEMP_EN_PULSE_WATCHDOG) && (pulse_value >= DUTCY_MAX_OVER_TIME)) tmr_pulse_value++;
	if((tmr_pulse_value > 0) && (tmr_pulse_value < T_DUTCY_MAX) && (pulse_value < DUTCY_MAX_OVER_TIME)) tmr_pulse_value--;

#if(PRJ_DEBUG && WATCHDOG_UART_ENABLE)
	static uint8 x_ = 3;
	static uint8 y_ = 3;
	static uint8 z_ = 3;

	if(x != x_ || y != y_ || z != z_)
	{
		UART_printf(&hUART_PROG, "\ntmr: %d - >=300C: %d - pulse: %d\n", x, y, z);

		x_ = x;
		y_ = y;
		z_ = z;
	}

	static uint32 tmr_pulse_value_ = T_DUTCY_MAX + 1;

	if(tmr_pulse_value != tmr_pulse_value_)
	{
		//UART_printf(&hUART_PROG, "solder_tip.c - pulse_value_watch_dog() - tmr_pulse_value: %d\n", tmr_pulse_value);
		UART_printf(&hUART_PROG, "tmr_pulse_value: %d\n", tmr_pulse_value);

		tmr_pulse_value_ = tmr_pulse_value;
	}
#endif

	// max temp reached
	if(tmr_pulse_value >= T_DUTCY_MAX)
	{
#if(PRJ_DEBUG && WATCHDOG_UART_ENABLE)
		UART_printf(&hUART_PROG, "solder_tip.c - pulse_value_watch_dog() - max temp reached at: %d°C\n", (temp_ist * 5));
#endif

		TEMP_MAX = temp_ist - 2;
		set_temp_soll_in_app_main(TEMP_MAX);

		change_PWM(0);
		tmr_pulse_value = 0;

		tmr_cooldown = T_COOLDOWN;
		display(DISP_IMG_COOLDOWN);
	}
}

extern void set_temp_ist(uint16 T)
{
	temp_ist = T;
}

extern void solder_tip_init()
{
	temp_soll = 10;
	temp_ist = temp_soll + 1;
	tmr_regel = T_REGEL;
	//measure_en = true;
}

extern void solder_tip_set_temp(uint16 temperature)
{
	temp_soll = temperature;

#if(PRJ_DEBUG  && TASK_UART_ENABLE)
	UART_printf(&hUART_PROG, "solder_tip.c - solder_tip_set_temp() - temp_soll: %d\n", temp_soll);
#endif
}

extern uint16 solder_tip_get_temp()
{
	return temp_ist;
}

extern void solder_tip_time_handler()
{
	if(started == true)
	{
		if(tmr_regel > 0) tmr_regel--;
		else if(tmr_cooldown == 0)
		{
#if(PRJ_DEBUG && 0)
			UART_printf(&hUART_PROG, "solder_tip.c - solder_tip_time_handler() before solder_tip_task()\n");
#endif

			if(TIM14->CNT >= TIM14->CCR1) // wenn PWM gerade 0 ist
			{
				solder_tip_task();

				tmr_regel = T_REGEL;
			}

			pulse_value_watch_dog();

#if(PRJ_DEBUG && 0)
			UART_printf(&hUART_PROG, "solder_tip.c - solder_tip_time_handler() after solder_tip_task()\n");
#endif
		}
		else
		{
			change_PWM(0);
		}
	}

	if(tmr_cooldown > 0) tmr_cooldown--;
}

extern void solder_tip_start()
{
	started = true;
}

extern void solder_tip_stop()
{
	started = false;
	change_PWM(0);
}
