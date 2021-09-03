//
// Oliver Blaser
//
// 10.05.2017
// 10.05.2017
//

#ifndef _solder_tip_H_
#define _solder_tip_H_

#include "project.h"
#include "std_def.h"
#include "settings.h"

///////////////////////////////////////////////////////////
//
// User settings
//
///////////////////////////////////////////////////////////

#define T_REGEL						70

#define FAKTOR_POS					1
#define FAKTOR_NEG					2


///////////////////////////////////////////////////////////
//
// end User settings
//
///////////////////////////////////////////////////////////

#define TASK_UART_ENABLE			0
#define WATCHDOG_UART_ENABLE		0

#define T_COOLDOWN					(30 * 1000)	// zeit bis erneut geheizt werden darf nachdem der pulse_value_watch_dog() eingesetzt hat

#define T_DUTCY_MAX					(500)		// maximale zeit, die ti >= DUTCY_MAX_OVER_TIME sein darf
#define DUTCY_MAX_OVER_TIME			500			// /1000 abs-max=900

#define TEMP_EN_PULSE_WATCHDOG		(300 / 5)

#define TEMP_MAX_DEFAULT			(340 / 5)
#define TEMP_MIN					(50 / 5)

uint16 TEMP_MAX;

extern void set_temp_soll_in_app_main(uint16 temp);

extern void solder_tip_init();
extern void solder_tip_set_temp(uint16);
extern uint16 solder_tip_get_temp();
extern void solder_tip_time_handler();
extern void solder_tip_start();
extern void solder_tip_stop();

#endif
