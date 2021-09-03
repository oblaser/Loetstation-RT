/*
 * Oliver Blaser
 *
 * 01.02.2017
 *
 * Hauptapplikation
 */

#ifndef _app_main_h_
#define _app_main_h_

#include "project.h"
#include "std_def.h"
#include "settings.h"

///////////////////////////////////////////////////////////
//
// User settings
//
///////////////////////////////////////////////////////////

#ifndef _SETTINGS_H_

#define EN_DISPLAY_HYST				1

#endif

///////////////////////////////////////////////////////////
//
// end User settings
//
///////////////////////////////////////////////////////////

#define T_STARTUP					1000
#define T_BUT_HOLD					1000
#define T_BUT_HOLD_FAST				100
#define T_DISP_TEMPERATURE_SOLL		3000
#define T_BUT_OTHER					100

#if(T_BUT_HOLD <= T_BUT_HOLD_FAST)
#error "T_BUT_HOLD is smaller than T_BUT_HOLD_FAST"
#endif
#if(T_BUT_HOLD >= T_DISP_TEMPERATURE_SOLL)
#error "T_BUT_HOLD is bigger than T_DISP_TEMPERATURE_SOLL"
#endif

#define T_h_m_s(h, m, s)			((h * 60 * 60 * 1000) + (m * 60 * 1000) + (s * 1000))

#define T_AUTO_TURN_OFF				T_h_m_s(0, 45, 0)

#if((T_AUTO_TURN_OFF > 4294967295) || (T_AUTO_TURN_OFF < 1))
#error "unvalid T_ON!"
#endif

extern void app_main();
extern void app_time_handler();

#endif
