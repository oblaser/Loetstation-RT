//
// Oliver Blaser
//
// 10.05.2017
// 30.05.2017
//

#ifndef _HW_H_
#define _HW_H_

#include "stm32f0xx_hal.h"
#include "project.h"
#include "std_def.h"

//
// OUT
//
#define DISP_LATCH_Pin 				GPIO_PIN_6
#define DISP_LATCH_GPIO_Port 		GPIOA
#define DISP_LATCH					DISP_LATCH_GPIO_Port,DISP_LATCH_Pin

#define LED_Status_Pin 				GPIO_PIN_10
#define LED_Status_GPIO_Port 		GPIOA
#define LED_Status					LED_Status_GPIO_Port,LED_Status_Pin

#define LED_M1_Pin 					GPIO_PIN_11
#define LED_M1_GPIO_Port 			GPIOA
#define LED_M1						LED_M1_GPIO_Port,LED_M1_Pin

#define LED_M2_Pin 					GPIO_PIN_12
#define LED_M2_GPIO_Port 			GPIOA
#define LED_M2						LED_M2_GPIO_Port,LED_M2_Pin

//
// IN
//
#define BUT_minus_Pin 				GPIO_PIN_4
#define BUT_minus_GPIO_Port 		GPIOB
#define BUT_minus					BUT_minus_GPIO_Port,BUT_minus_Pin

#define BUT_plus_Pin 				GPIO_PIN_5
#define BUT_plus_GPIO_Port 			GPIOB
#define BUT_plus					BUT_plus_GPIO_Port,BUT_plus_Pin

#define BUT_M1_Pin 					GPIO_PIN_6
#define BUT_M1_GPIO_Port 			GPIOB
#define BUT_M1						BUT_M1_GPIO_Port,BUT_M1_Pin

#define BUT_M2_Pin 					GPIO_PIN_7
#define BUT_M2_GPIO_Port 			GPIOB
#define BUT_M2						BUT_M2_GPIO_Port,BUT_M2_Pin

//
// PROGIO
//
#if(PRJ_PROGIO_OUT || PRJ_PROGIO_IN)
#define Prog_IO_Pin					GPIO_PIN_8
#define Prog_IO_GPIO_Port 			GPIOA
#define Prog_IO						Prog_IO_GPIO_Port,Prog_IO_Pin
#endif

extern void HW_init();

#endif
