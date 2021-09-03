/*
 * Oliver Blaser
 *
 * 09.02.2017
 *
 * Hauptapplikation Sensorboard
 */

#include "app_main.h"

#include "stm32f0xx_hal.h"
#include "UART.h"
#include "project.h"
#include "error.h"
#include "std_def.h"
#include "solder_tip.h"
#include "flash.h"
#include "edge.h"
#include "display.h"
#include "PWM.h"

typedef enum
{
	STATE_INIT,
	STATE_STARTUP,
	STATE_STARTUP_W,
	STATE_WAIT,
	STATE_OFF,
	STATE_STANDBY
}SM_STATE;

static uint16 tmr_but_plus = T_BUT_HOLD;
static uint16 tmr_but_minus = T_BUT_HOLD;
static uint16 tmr_but_M1 = T_BUT_HOLD;
static uint16 tmr_but_M2 = T_BUT_HOLD;
static uint16 tmr_disp_temperature_soll = 0;
static uint16 tmr_disp_act = 0;
static uint16 tmr_status_led = 0;
static uint16 tmr_but_on = 0;
static uint16 tmr_but_other = 0;
static uint16 tmr_standby = 0;
static uint32 tmr_auto_turn_off = T_AUTO_TURN_OFF;

static uint16 act_temp_old = 0;
static uint16 act_temp = 0;

static uint16 temperature_soll = 0;

static bool temperature_soll_setted = false;

static void state_machine()
{
	static SM_STATE state = STATE_INIT;

#if(PRJ_DEBUG)

	static SM_STATE state_old = STATE_WAIT;

	if(state != state_old)
	{
		UART_printf(&hUART_PROG, "\nstate: %d\n", state);
		state_old = state;
	}
#endif

	// auto turn off
	if(BUT_plus_pos || BUT_minus_pos || BUT_M1_pos || BUT_M2_pos) tmr_auto_turn_off = T_AUTO_TURN_OFF;
	if((tmr_auto_turn_off == 0) && (state != STATE_OFF) && (state != STATE_STANDBY)) state = STATE_OFF;

	switch(state)
	{
		case STATE_INIT:

			TEMP_MAX = TEMP_MAX_DEFAULT;

			solder_tip_init();

			PWM_start();

			__DISP_WRITE_ENABLE();

			state = STATE_OFF;

			break;

		case STATE_STARTUP:

			if(FLASH_get_value(FLASH_M1) == 0) FLASH_set_value(FLASH_M1, 54);		// 270°C
			if(FLASH_get_value(FLASH_M2) == 0) FLASH_set_value(FLASH_M2, 64);		// 320°C
			if(FLASH_get_value(FLASH_Mact) == 0) FLASH_set_value(FLASH_Mact, 10);	//  50°C

			if(FLASH_get_value(FLASH_Mselect) == 1)
			{
				HAL_GPIO_WritePin(LED_M1, 1);
				HAL_GPIO_WritePin(LED_M2, 0);

				temperature_soll = FLASH_get_value(FLASH_M1);
			}

			else if(FLASH_get_value(FLASH_Mselect) == 2)
			{
				HAL_GPIO_WritePin(LED_M1, 0);
				HAL_GPIO_WritePin(LED_M2, 1);

				temperature_soll = FLASH_get_value(FLASH_M2);
			}

			else
			{
				HAL_GPIO_WritePin(LED_M1, 0);
				HAL_GPIO_WritePin(LED_M2, 0);

				temperature_soll = FLASH_get_value(FLASH_Mact);
			}

			if(temperature_soll > TEMP_MAX) temperature_soll = TEMP_MAX;
			if(temperature_soll < TEMP_MIN) temperature_soll = TEMP_MIN;

			solder_tip_set_temp(temperature_soll);

			display(DISP_IMG_CLEAR);

			tmr_disp_temperature_soll = T_DISP_TEMPERATURE_SOLL;
			act_temp_old = 0;
			display_numeric(temperature_soll * 5);

			solder_tip_start();

			state = STATE_STARTUP_W;

			break;

		case STATE_STARTUP_W:

			if(tmr_standby == 0) state = STATE_WAIT;

			break;

		case STATE_WAIT:

			//
			// But plus
			//
			if(BUT_plus_pos)
			{
				tmr_but_plus = T_BUT_HOLD;

				if(temperature_soll < TEMP_MAX && tmr_disp_temperature_soll > 0 && tmr_but_other == 0)
				{
					temperature_soll++;

					HAL_GPIO_WritePin(LED_M1, 0);
					HAL_GPIO_WritePin(LED_M2, 0);

					FLASH_set_value(FLASH_Mselect, 3);
				}

				tmr_but_other = T_BUT_OTHER;

				tmr_disp_temperature_soll = T_DISP_TEMPERATURE_SOLL;
				act_temp_old = 0;
				display_numeric(temperature_soll * 5);
			}

			if(BUT_plus_neu && tmr_but_plus == 0)
			{
				if(BUT_minus_neu)
				{
					state = STATE_OFF;
				}

				else
				{
					tmr_but_plus = T_BUT_HOLD_FAST;
					if(temperature_soll < TEMP_MAX && tmr_disp_temperature_soll > 0) temperature_soll++;

					HAL_GPIO_WritePin(LED_M1, 0);
					HAL_GPIO_WritePin(LED_M2, 0);

					tmr_disp_temperature_soll = T_DISP_TEMPERATURE_SOLL;
					act_temp_old = 0;
					display_numeric(temperature_soll * 5);
				}
			}

			//
			// But minus
			//
			if(BUT_minus_pos)
			{
				tmr_but_minus = T_BUT_HOLD;

				if(temperature_soll > TEMP_MIN && tmr_disp_temperature_soll > 0 && tmr_but_other == 0)
				{
					temperature_soll--;

					HAL_GPIO_WritePin(LED_M1, 0);
					HAL_GPIO_WritePin(LED_M2, 0);

					FLASH_set_value(FLASH_Mselect, 3);
				}

				tmr_but_other = T_BUT_OTHER;

				tmr_disp_temperature_soll = T_DISP_TEMPERATURE_SOLL;
				act_temp_old = 0;
				display_numeric(temperature_soll * 5);
			}

			if(BUT_minus_neu && tmr_but_minus == 0)
			{
				if(BUT_plus_neu)
				{
					state = STATE_OFF;
				}

				else
				{
					tmr_but_minus = T_BUT_HOLD_FAST;

					if(temperature_soll > TEMP_MIN && tmr_disp_temperature_soll > 0) temperature_soll--;

					HAL_GPIO_WritePin(LED_M1, 0);
					HAL_GPIO_WritePin(LED_M2, 0);

					tmr_disp_temperature_soll = T_DISP_TEMPERATURE_SOLL;
					act_temp_old = 0;
					display_numeric(temperature_soll * 5);
				}
			}

			//
			// But M1
			//
			if(BUT_M1_pos)
			{
				tmr_but_M1 = 0;
			}

			if(BUT_M1_neg)
			{
				tmr_but_M1 = 0;

				HAL_GPIO_WritePin(LED_M1, 1);
				HAL_GPIO_WritePin(LED_M2, 0);

				FLASH_set_value(FLASH_Mselect, 1);

				//UART_printf(&hUART_PROG, "M %d\n", FLASH_get_value(FLASH_Mselect));

				temperature_soll = FLASH_get_value(FLASH_M1);

				tmr_disp_temperature_soll = T_DISP_TEMPERATURE_SOLL;
				act_temp_old = 0;
				display_numeric(temperature_soll * 5);
			}

			if(BUT_M1_neu && tmr_but_M1 >= T_BUT_HOLD)
			{
				tmr_but_M1 = 0;

				HAL_GPIO_WritePin(LED_M1, 1);
				HAL_GPIO_WritePin(LED_M2, 0);

				FLASH_set_value(FLASH_M1, temperature_soll);

				display(DISP_IMG_iO);

				FLASH_set_value(FLASH_Mselect, 1);

				//UART_printf(&hUART_PROG, "M %d\n", FLASH_get_value(FLASH_Mselect));

				tmr_disp_temperature_soll = T_DISP_TEMPERATURE_SOLL;
				act_temp_old = 0;
			}

			//
			// But M2
			//
			if(BUT_M2_pos)
			{
				tmr_but_M2 = 0;
			}

			if(BUT_M2_neg)
			{
				tmr_but_M2 = 0;

				HAL_GPIO_WritePin(LED_M1, 0);
				HAL_GPIO_WritePin(LED_M2, 1);

				FLASH_set_value(FLASH_Mselect, 2);

				//UART_printf(&hUART_PROG, "M %d\n", FLASH_get_value(FLASH_Mselect));

				temperature_soll = FLASH_get_value(FLASH_M2);

				tmr_disp_temperature_soll = T_DISP_TEMPERATURE_SOLL;
				act_temp_old = 0;
				display_numeric(temperature_soll * 5);
			}

			if(BUT_M2_neu && tmr_but_M2 >= T_BUT_HOLD)
			{
				tmr_but_M2 = 0;

				HAL_GPIO_WritePin(LED_M1, 0);
				HAL_GPIO_WritePin(LED_M2, 1);

				FLASH_set_value(FLASH_M2, temperature_soll);

				display(DISP_IMG_iO);

				FLASH_set_value(FLASH_Mselect, 2);

				//UART_printf(&hUART_PROG, "M %d\n", FLASH_get_value(FLASH_Mselect));

				tmr_disp_temperature_soll = T_DISP_TEMPERATURE_SOLL;
				act_temp_old = 0;
			}

			//
			// set temperature_soll
			//
			if(tmr_disp_temperature_soll == 0)
			{
				if(temperature_soll_setted == false)
				{
					solder_tip_set_temp(temperature_soll);

					if(FLASH_get_value(FLASH_Mselect == 3)) FLASH_set_value(FLASH_Mact, temperature_soll);

					temperature_soll_setted = true;
				}

			}

			else temperature_soll_setted = false;

			//
			// display/get actual temperature
			//
			if(tmr_disp_temperature_soll == 0 && tmr_disp_act == 0)
			{
				tmr_disp_act = 200;

				act_temp = solder_tip_get_temp();

#if(EN_DISPLAY_HYST)
				if((act_temp_old >= (act_temp + 3)) || (act_temp_old <= (act_temp - 3)) || (act_temp == temperature_soll)) // anzeige "hysterese"
				{
					act_temp_old = act_temp;

					display_numeric(act_temp * 5);
				}
#else
				if(act_temp_old != act_temp)
				{
					act_temp_old = act_temp;

					display_numeric(act_temp * 5);
				}
#endif
			}

			//
			// Status LED
			//
			if(tmr_status_led == 0)
			{
				tmr_status_led = 200;

#if(EN_DISPLAY_HYST)
				if((temperature_soll >= (act_temp + 2)) || (temperature_soll <= (act_temp - 2))) // anzeige "hysterese"
				{
					HAL_GPIO_TogglePin(LED_Status);
				}

				else
				{
					HAL_GPIO_WritePin(LED_Status, 1);
				}
#else
				if(temperature_soll != act_temp)
				{
					HAL_GPIO_TogglePin(LED_Status);
				}

				else
				{
					HAL_GPIO_WritePin(LED_Status, 1);
				}
#endif
			}

			break;

		case STATE_OFF:

			solder_tip_stop();

			change_PWM(0);

			display(DISP_IMG_OFF);

			HAL_GPIO_WritePin(LED_M1, 0);
			HAL_GPIO_WritePin(LED_M2, 0);
			HAL_GPIO_WritePin(LED_Status, 0);

			tmr_standby = T_STARTUP;

			state = STATE_STANDBY;

			break;

		case STATE_STANDBY:

			if(tmr_standby == 0)
			{
				if((HAL_GPIO_ReadPin(BUT_plus) == 0) || (HAL_GPIO_ReadPin(BUT_minus) == 0)) tmr_but_on = T_BUT_HOLD;

				if(tmr_but_on == 0)
				{
					// reset max temperature
					TEMP_MAX = TEMP_MAX_DEFAULT;

					tmr_standby = T_STARTUP;

					state = STATE_STARTUP;
				}
			}

			break;

		default:
			my_Error_Handler("unvalid state");
			break;
	}
}

extern void app_main()
{
	edge_detect();

	state_machine();
}

extern void set_temp_soll_in_app_main(uint16 temp)
{
	temperature_soll = temp;
}

extern void app_time_handler()
{
	edge_time_handler();

	if(tmr_but_plus > 0) tmr_but_plus--;
	if(tmr_but_minus > 0) tmr_but_minus--;
	if(tmr_but_on > 0) tmr_but_on--;
	if(tmr_standby > 0) tmr_standby--;
	if(tmr_but_other > 0) tmr_but_other--;

	if(tmr_auto_turn_off > 0) tmr_auto_turn_off--;

	if(tmr_but_M1 < T_BUT_HOLD) tmr_but_M1++;
	if(tmr_but_M2 < T_BUT_HOLD) tmr_but_M2++;

	if(tmr_disp_temperature_soll > 0) tmr_disp_temperature_soll--;
	if(tmr_disp_act > 0) tmr_disp_act--;
	if(tmr_status_led > 0) tmr_status_led--;
}
