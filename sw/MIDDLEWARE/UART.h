/**
* @file UART.h
*
* @see UART.c
*
* Erstellt am 24.01.2017
* Erstellt durch Simon Walker
*
* Aktuelle Version: 1.02
*
* release version 0.01 -- 24.01.2017 -- Simon Walker
* release version 1.00 -- 01.02.2017 -- Simon Walker
* release version 1.01 -- 06.02.2017 -- Simon Walker
* release version 1.02 -- 06.03.2017 -- Simon Walker
*
* Änderungen: 24.01.2017 -- Simon Walker -- File Erstellt
*             01.02.2017 -- Simon Walker -- 1. release (Alle Funktionen getestet und Funktionstüchtig)
*             06.02.2017 -- Simon Walker -- defines für bessere Lesbarkeit bsp hUART_PROG anstelle von huart1
*			  06.03.2017 -- Simon Walker -- Endzeichen für Sendefunktion als Define
*
* Dieses File enthält Funktionen, die für die Komunikation über Uart benötigt werden.
*
* Modulkürzel: UART
*/


#ifndef __UART_H
#define __UART_H

#include "std_def.h"
#include "project.h"
#include "stm32f0xx_hal.h"

#define UART_ERROR 1
#define UART_OK 0



#define UART_TX_MAX_STR_LENGTH		50



#if (PRJ_UART_PROG)
#define UART_SEND_END_CHAR			'\n'
extern void UART_printf(UART_HandleTypeDef *huart, char *pcString, ...);
#endif


//////////////////////////////////////////////////////////////////////////////
//PROG UART
//////////////////////////////////////////////////////////////////////////////
#if (PRJ_UART_PROG)
#define UART1_MAX_STR_LENGTH 		50
#define UART1_END_CHAR				'\n'
#define hUART_PROG huart1


UART_HandleTypeDef huart1;
#define hUART_PROG huart1

bool UART1_RX_COMPLET;

extern uint8_t UART1_Init();
extern uint8_t UART1_Start();
extern void UART1_ClearRXBuff (void);
extern uint8_t * UART1_ReadRXBuff (void);
#endif




#endif /* __UART_H */

/************************END OF FILE****/
