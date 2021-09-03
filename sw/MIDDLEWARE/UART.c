/**
* @file UART.c
*
* @see UART.h
*
* Erstellt am 24.01.2017
* Erstellt durch Simon Walker
*
* Aktuelle Version: 1.02
*
* release version 0.01 -- 24.01.2017 -- Simon Walker
* release version 1.00 -- 01.02.2017 -- Simon Walker
* release version 1.01 -- 06.03.2017 -- Simon Walker
* release version 1.02 -- 06.03.2017 -- Oliver Blaser
*
* Änderungen: 24.01.2017 -- Simon Walker	-- File Erstellt
*             01.02.2017 -- Simon Walker	-- 1. release (Alle Funktionen getestet und Funktionstüchtig)
*			  06.03.2017 -- Simon Walker	-- Fehler mit den Endzeichen in der Sendefunktion behoben
*			  06.03.2017 -- Oliver Blaser	-- huartX.RxXferCount zuruecksetzen in UARTx_ClearRXBuff(), sonst konnten nur UARTx_MAX_STR_LENGTH Zeichen empfangen werden.
*
* Dieses File enthält Funktionen, die für die Komunikation über Uart benötigt werden.
*
* Modulkürzel: UART
*/

#include "UART.h"
#include <stdarg.h>
#include "project.h"
#include "stm32f0xx_hal.h"

//static uint32_t Timeout = 0;
static const char * const g_pcHex = "0123456789abcdef"; // A mapping from an integer between 0 and 15 to its ASCII character equivalent.
static uint8_t *pUART_END_CHAR;

#if(PRJ_UART_PROG)
static uint8_t UART1_RX_BUFF[UART1_MAX_STR_LENGTH];
#endif

//////////////////////////////////////////////////////////////////////////////
//Prog UART
//////////////////////////////////////////////////////////////////////////////
#if (PRJ_UART_PROG)
extern uint8_t UART1_Init()
{
	//variable initialization
	UART1_RX_COMPLET = false;


	__HAL_RCC_USART1_CLK_ENABLE(); //Enable the USART1 Clock

	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0); //Sets the priority level of the UART1 Interrupt to 0 sub-priority to 0
	HAL_NVIC_EnableIRQ(USART1_IRQn); //Enables the UART1 Interrupt

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
	huart1.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
	huart1.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
	huart1.pRxBuffPtr = UART1_RX_BUFF;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		return (UART_ERROR);
	}
	return (UART_OK);
}


void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart1);
	pUART_END_CHAR = huart1.pRxBuffPtr;
	pUART_END_CHAR--;
	if(*pUART_END_CHAR == UART1_END_CHAR)
	{
		UART1_RX_COMPLET = 1;
	}
}

extern uint8_t UART1_Start(void)
{
	if (HAL_UART_Receive_IT(&huart1, UART1_RX_BUFF,UART1_MAX_STR_LENGTH) != HAL_OK) return UART_ERROR;
	return (UART_OK);
}

extern void UART1_ClearRXBuff (void)
{
	huart1.pRxBuffPtr = UART1_RX_BUFF;
	huart1.RxXferCount = 0;
}

extern uint8_t * UART1_ReadRXBuff (void)
{
	return UART1_RX_BUFF;
}
#endif


//////////////////////////////////////////////////////////////////////////////
//UART 3-6 Interrupt
//////////////////////////////////////////////////////////////////////////////
#if (PRJ_UART_MB | PRJ_UART_DB | PRJ_UART_SB)
void USART3_6_IRQHandler(void)
{
#if(PRJ_UART_MB)
	HAL_UART_IRQHandler(&huart3);

	pUART_END_CHAR = huart3.pRxBuffPtr;
	pUART_END_CHAR--;
	if(*pUART_END_CHAR == UART3_END_CHAR)
	{
		UART3_RX_COMPLET = 1;
	}

#endif

#if (PRJ_UART_DB)
	HAL_UART_IRQHandler(&huart5);

	pUART_END_CHAR = huart5.pRxBuffPtr;
	pUART_END_CHAR--;
	if(*pUART_END_CHAR == UART5_END_CHAR)
	{
		UART5_RX_COMPLET = 1;
	}

#endif

#if (PRJ_UART_SB)
	HAL_UART_IRQHandler(&huart6);

	pUART_END_CHAR = huart6.pRxBuffPtr;
	pUART_END_CHAR--;
	if(*pUART_END_CHAR == UART6_END_CHAR)
	{
		UART6_RX_COMPLET = 1;
	}

#endif
}
#endif


//////////////////////////////////////////////////////////////////////////////
//Mainboard UART
//////////////////////////////////////////////////////////////////////////////
#if (PRJ_UART_MB)
extern uint8_t UART3_Init()
{
	//variable initialization
	UART3_RX_COMPLET = false;


	__HAL_RCC_USART3_CLK_ENABLE(); //Enable the USART3 Clock

	HAL_NVIC_SetPriority(USART3_6_IRQn, 0, 0); //Sets the priority level of the UART3-6 Interrupt to 0 sub-priority to 0
	HAL_NVIC_EnableIRQ(USART3_6_IRQn); //Enables the UART3-6 Interrupt

	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
	huart3.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
	huart3.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
	if (HAL_UART_Init(&huart3) != HAL_OK)
	{
		return (UART_ERROR);
	}
	return (UART_OK);
}


extern uint8_t UART3_Start(void)
{
	if (HAL_UART_Receive_IT(&huart3, UART3_RX_BUFF,UART3_MAX_STR_LENGTH) != HAL_OK) return UART_ERROR;
	return (UART_OK);
}

extern void UART3_ClearRXBuff (void)
{
	huart3.pRxBuffPtr = UART3_RX_BUFF;
	huart3.RxXferCount = 0;
}

extern uint8_t * UART3_ReadRXBuff (void)
{
	return UART3_RX_BUFF;
}
#endif


//////////////////////////////////////////////////////////////////////////////
//DriverBoard UART
//////////////////////////////////////////////////////////////////////////////
#if (PRJ_UART_DB)
extern uint8_t UART5_Init()
{
	//variable initialization
	UART5_RX_COMPLET = false;


	__HAL_RCC_USART5_CLK_ENABLE(); //Enable the USART5 Clock

	HAL_NVIC_SetPriority(USART3_6_IRQn, 0, 0); //Sets the priority level of the UART3-6 Interrupt to 0 sub-priority to 0
	HAL_NVIC_EnableIRQ(USART3_6_IRQn); //Enables the UART3-6 Interrupt

	huart5.Instance = USART5;
	huart5.Init.BaudRate = 115200;
	huart5.Init.WordLength = UART_WORDLENGTH_8B;
	huart5.Init.StopBits = UART_STOPBITS_1;
	huart5.Init.Parity = UART_PARITY_NONE;
	huart5.Init.Mode = UART_MODE_TX_RX;
	huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart5.Init.OverSampling = UART_OVERSAMPLING_16;
	huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
	huart5.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
	huart5.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
	if (HAL_UART_Init(&huart5) != HAL_OK)
	{
		return (UART_ERROR);
	}
	return (UART_OK);
}

extern uint8_t UART5_Start(void)
{
	if (HAL_UART_Receive_IT(&huart5, UART5_RX_BUFF,UART5_MAX_STR_LENGTH) != HAL_OK) return UART_ERROR;
	return (UART_OK);
}

extern void UART5_ClearRXBuff (void)
{
	huart5.pRxBuffPtr = UART5_RX_BUFF;
	huart5.RxXferCount = 0;
}

extern uint8_t * UART5_ReadRXBuff (void)
{
	return UART5_RX_BUFF;
}
#endif



//////////////////////////////////////////////////////////////////////////////
//SensorBoard UART
//////////////////////////////////////////////////////////////////////////////
#if (PRJ_UART_SB)
extern uint8_t UART6_Init()
{
	//variable initialization
	UART6_RX_COMPLET = false;


	__HAL_RCC_USART6_CLK_ENABLE(); //Enable the USART6 Clock

	HAL_NVIC_SetPriority(USART3_6_IRQn, 0, 0); //Sets the priority level of the UART3-6 Interrupt to 0 sub-priority to 0
	HAL_NVIC_EnableIRQ(USART3_6_IRQn); //Enables the UART3-6 Interrupt

	huart6.Instance = USART6;
	huart6.Init.BaudRate = 115200;
	huart6.Init.WordLength = UART_WORDLENGTH_8B;
	huart6.Init.StopBits = UART_STOPBITS_1;
	huart6.Init.Parity = UART_PARITY_NONE;
	huart6.Init.Mode = UART_MODE_TX_RX;
	huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart6.Init.OverSampling = UART_OVERSAMPLING_16;
	huart6.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart6.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
	huart6.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
	huart6.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
	if (HAL_UART_Init(&huart6) != HAL_OK)
	{
		return (UART_ERROR);
	}
	return (UART_OK);
}

extern uint8_t UART6_Start(void)
{
	if (HAL_UART_Receive_IT(&huart6, UART5_RX_BUFF,UART6_MAX_STR_LENGTH) != HAL_OK) return UART_ERROR;
	return (UART_OK);
}

extern void UART6_ClearRXBuff (void)
{
	huart6.pRxBuffPtr = UART6_RX_BUFF;
	huart6.RxXferCount = 0;
}

extern uint8_t * UART6_ReadRXBuff (void)
{
	return UART6_RX_BUFF;
}
#endif


//*****************************************************************************
//
//! Writes a string of characters to the UART output.
//!
//! \param pcBuf points to a buffer containing the string to transmit.
//! \param ulLen is the length of the string to transmit.
//!
//! This function will transmit the string to the UART output.  The number of
//! characters transmitted is determined by the \e ulLen parameter.  This
//! function does no interpretation or translation of any characters.  Since
//! the output is sent to a UART, any LF (/n) characters encountered will be
//! replaced with a CRLF pair.
//!
//! Besides using the \e ulLen parameter to stop transmitting the string, if a
//! null character (0) is encountered, then no more characters will be
//! transmitted and the function will return.
//!
//! In non-buffered mode, this function is blocking and will not return until
//! all the characters have been written to the output FIFO.  In buffered mode,
//! the characters are written to the UART transmit buffer and the call returns
//! immediately.  If insufficient space remains in the transmit buffer,
//! additional characters are discarded.
//!
//! \return Returns the count of characters written.
//
//*****************************************************************************
static int UART_write(UART_HandleTypeDef *huart ,char *pcBuf, uint32_t ulLen)
{
	//
	// Send the characters
	//
	HAL_UART_Transmit(huart, (uint8_t*)pcBuf, ulLen, 20);
	//
	// Return the number of characters written.
	//
	return(ulLen);
}


//*****************************************************************************
//
//! A simple UART based printf function supporting \%c, \%d, \%p, \%s, \%u,
//! \%x, and \%X.
//!
//! \param pcString is the format string.
//! \param ... are the optional arguments, which depend on the contents of the
//! format string.
//!
//! This function is very similar to the C library <tt>fprintf()</tt> function.
//! All of its output will be sent to the UART.  Only the following formatting
//! characters are supported:
//!
//! - \%c to print a character
//! - \%d to print a decimal value
//! - \%s to print a string
//! - \%u to print an unsigned decimal value
//! - \%x to print a hexadecimal value using lower case letters
//! - \%X to print a hexadecimal value using lower case letters (not upper case
//! 	  letters as would typically be used)
//! - \%p to print a pointer as a hexadecimal value
//! - \%\% to print out a \% character
//!
//! For \%s, \%d, \%u, \%p, \%x, and \%X, an optional number may reside
//! between the \% and the format character, which specifies the minimum number
//! of characters to use for that value; if preceded by a 0 then the extra
//! characters will be filled with zeros instead of spaces.  For example,
//! ``\%8d'' will use eight characters to print the decimal value with spaces
//! added to reach eight; ``\%08d'' will use eight characters as well but will
//! add zeroes instead of spaces.
//!
//! The type of the arguments after \e pcString must match the requirements of
//! the format string.  For example, if an integer was passed where a string
//! was expected, an error of some kind will most likely occur.
//!
//! \return None.
//
//*****************************************************************************
extern void UART_printf(UART_HandleTypeDef *huart, char *pcString,...)
{
		unsigned long ulIdx, ulValue, ulPos, ulCount, ulBase, ulNeg;
		char *pcStr, pcBuf[16], cFill;
		//char*pcString;
		va_list vaArgP;


		//
		// Start the varargs processing.
		//
		va_start(vaArgP, pcString);
		//pcString = va_arg(vaArgP, char*);

		//
		// Loop while there are more characters in the string.
		//
		while((*pcString != 0x00)/* && (*pcString != UART_SEND_END_CHAR)*/)
		{
			//
			// Find the first non-% character, or the end of the string.
			//
			for(ulIdx = 0; (pcString[ulIdx] != '%') && (pcString[ulIdx] != '\0'); ulIdx++)
			{
			}

			//
			// Write this portion of the string.
			//
			UART_write(huart, pcString, ulIdx);

			//
			// Skip the portion of the string that was written.
			//
			pcString += ulIdx;

			//
			// See if the next character is a %.
			//
			if(*pcString == '%')
			{
				//
				// Skip the %.
				//
				pcString++;

				//
				// Set the digit count to zero, and the fill character to space
				// (i.e. to the defaults).
				//
				ulCount = 0;
				cFill = ' ';

				//
				// It may be necessary to get back here to process more characters.
				// Goto's aren't pretty, but effective.  I feel extremely dirty for
				// using not one but two of the beasts.
				//
	again:

				//
				// Determine how to handle the next character.
				//
				switch(*pcString++)
				{
					//
					// Handle the digit characters.
					//
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
					{
						//
						// If this is a zero, and it is the first digit, then the
						// fill character is a zero instead of a space.
						//
						if((pcString[-1] == '0') && (ulCount == 0))
						{
							cFill = '0';
						}

						//
						// Update the digit count.
						//
						ulCount *= 10;
						ulCount += pcString[-1] - '0';

						//
						// Get the next character.
						//
						goto again;
					}

					//
					// Handle the %c command.
					//
					case 'c':
					{
						//
						// Get the value from the varargs.
						//
						ulValue = va_arg(vaArgP, unsigned long);

						//
						// Print out the character.
						//
						UART_write(huart, (char*)&ulValue, 1);

						//
						// This command has been handled.
						//
						break;
					}

					//
					// Handle the %d, %i command.
					//
					case 'd':
					case 'i':
					{
						//
						// Get the value from the varargs.
						//
						ulValue = va_arg(vaArgP, unsigned long);

						//
						// Reset the buffer position.
						//
						ulPos = 0;

						//
						// If the value is negative, make it positive and indicate
						// that a minus sign is needed.
						//
						if((long)ulValue < 0)
						{
							//
							// Make the value positive.
							//
							ulValue = -(long)ulValue;

							//
							// Indicate that the value is negative.
							//
							ulNeg = 1;
						}
						else
						{
							//
							// Indicate that the value is positive so that a minus
							// sign isn't inserted.
							//
							ulNeg = 0;
						}

						//
						// Set the base to 10.
						//
						ulBase = 10;

						//
						// Convert the value to ASCII.
						//
						goto convert;
					}

					//
					// Handle the %s command.
					//
					case 's':
					{
						//
						// Get the string pointer from the varargs.
						//
						pcStr = va_arg(vaArgP, char *);

						//
						// Determine the length of the string.
						//
						/*for(ulIdx = 0; pcStr[ulIdx] != '\0'; ulIdx++)
						{
						}*/


						for(ulIdx = 0; ulIdx <= UART_TX_MAX_STR_LENGTH; ulIdx++)
						{
							if(pcStr[ulIdx] == '\0')
							{
								break;
							}
						}

						//
						// Write the string.
						//
						UART_write(huart, pcStr, ulIdx);

						//
						// Write any required padding spaces
						//
						if(ulCount > ulIdx)
						{
							ulCount -= ulIdx;
							while(ulCount--)
							{
								UART_write(huart, " ", 1);
							}
						}
						//
						// This command has been handled.
						//
						break;
					}

					//
					// Handle the %u command.
					//
					case 'u':
					{
						//
						// Get the value from the varargs.
						//
						ulValue = va_arg(vaArgP, unsigned long);

						//
						// Reset the buffer position.
						//
						ulPos = 0;

						//
						// Set the base to 10.
						//
						ulBase = 10;

						//
						// Indicate that the value is positive so that a minus sign
						// isn't inserted.
						//
						ulNeg = 0;

						//
						// Convert the value to ASCII.
						//
						goto convert;
					}

					//
					// Handle the %x and %X commands.  Note that they are treated
					// identically; i.e. %X will use lower case letters for a-f
					// instead of the upper case letters is should use.  We also
					// alias %p to %x.
					//
					case 'x':
					case 'X':
					case 'p':
					{
						//
						// Get the value from the varargs.
						//
						ulValue = va_arg(vaArgP, unsigned long);

						//
						// Reset the buffer position.
						//
						ulPos = 0;

						//
						// Set the base to 16.
						//
						ulBase = 16;

						//
						// Indicate that the value is positive so that a minus sign
						// isn't inserted.
						//
						ulNeg = 0;

						//
						// Determine the number of digits in the string version of
						// the value.
						//
						convert:
						for(ulIdx = 1;
							(((ulIdx * ulBase) <= ulValue) &&
							 (((ulIdx * ulBase) / ulBase) == ulIdx));
							ulIdx *= ulBase, ulCount--)
						{
						}

						//
						// If the value is negative, reduce the count of padding
						// characters needed.
						//
						if(ulNeg)
						{
							ulCount--;
						}

						//
						// If the value is negative and the value is padded with
						// zeros, then place the minus sign before the padding.
						//
						if(ulNeg && (cFill == '0'))
						{
							//
							// Place the minus sign in the output buffer.
							//
							pcBuf[ulPos++] = '-';

							//
							// The minus sign has been placed, so turn off the
							// negative flag.
							//
							ulNeg = 0;
						}

						//
						// Provide additional padding at the beginning of the
						// string conversion if needed.
						//
						if((ulCount > 1) && (ulCount < 16))
						{
							for(ulCount--; ulCount; ulCount--)
							{
								pcBuf[ulPos++] = cFill;
							}
						}

						//
						// If the value is negative, then place the minus sign
						// before the number.
						//
						if(ulNeg)
						{
							//
							// Place the minus sign in the output buffer.
							//
							pcBuf[ulPos++] = '-';
						}

						//
						// Convert the value into a string.
						//
						for(; ulIdx; ulIdx /= ulBase)
						{
							pcBuf[ulPos++] = g_pcHex[(ulValue / ulIdx) % ulBase];
						}

						//
						// Write the string.
						//
						UART_write(huart, pcBuf, ulPos);

						//
						// This command has been handled.
						//
						break;
					}

					//
					// Handle the %% command.
					//
					case '%':
					{
						//
						// Simply write a single %.
						//
						UART_write(huart, pcString - 1, 1);

						//
						// This command has been handled.
						//
						break;
					}

					//
					// Handle all other commands.
					//
					default:
					{
						//
						// Indicate an error.
						//
						UART_write(huart, "ERROR", 5);

						//
						// This command has been handled.
						//
						break;
					}
				}
			}
		}

		//
		// End the varargs processing.
		//
		va_end(vaArgP);

		//__HAL_RCC_USART2_CLK_DISABLE();
}

