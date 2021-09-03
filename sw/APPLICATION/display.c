//
// Oliver Blaser
//
// 22.05.2017
// 22.05.2017
//

#include "display.h"

#include "stm32f0xx_hal.h"
#include "project.h"
#include "error.h"
#include "SPI.h"
#include "std_def.h"
#include "UART.h"

const uint32 DISP_SEG[3][7] =
{
		{ DISP_SEG_EA, DISP_SEG_EB, DISP_SEG_EC, DISP_SEG_ED, DISP_SEG_EE, DISP_SEG_EF, DISP_SEG_EG },
		{ DISP_SEG_ZA, DISP_SEG_ZB, DISP_SEG_ZC, DISP_SEG_ZD, DISP_SEG_ZE, DISP_SEG_ZF, DISP_SEG_ZG },
		{ DISP_SEG_HA, DISP_SEG_HB, DISP_SEG_HC, DISP_SEG_HD, DISP_SEG_HE, DISP_SEG_HF, DISP_SEG_HG }
};

static void latch()
{
	uint16 tmr;

	HAL_GPIO_WritePin(DISP_LATCH, 1);

	tmr = 10000;
	while(tmr--);

	HAL_GPIO_WritePin(DISP_LATCH, 0);
}

extern void display_numeric(uint16 data)
{
	uint8 tmp[3];
	uint8 seg;
	uint32 disp = 0;

	if(display_write_en)
	{
		tmp[0] = data % 10;
		tmp[1] = (data / 10) % 10;
		tmp[2] = (data / 100) % 10;

		seg = 0;

		if(tmp[seg] == 0) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_E] | DISP_SEG[seg][DISP_SEG_F]);

		if(tmp[seg] == 1) disp |= (DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C]);

		if(tmp[seg] == 2) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_D] |
									DISP_SEG[seg][DISP_SEG_E] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 3) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 4) disp |= (DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 5) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 6) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_E] | DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 7) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C]);

		if(tmp[seg] == 8) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_E] | DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 9) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		seg = 1;

		if(tmp[seg] == 0 && tmp[seg + 1] > 0) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_E] | DISP_SEG[seg][DISP_SEG_F]);

		if(tmp[seg] == 1) disp |= (DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C]);

		if(tmp[seg] == 2) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_D] |
									DISP_SEG[seg][DISP_SEG_E] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 3) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 4) disp |= (DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 5) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 6) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_E] | DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 7) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C]);

		if(tmp[seg] == 8) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_E] | DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 9) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		seg = 2;

		if(tmp[seg] == 1) disp |= (DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C]);

		if(tmp[seg] == 2) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_D] |
									DISP_SEG[seg][DISP_SEG_E] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 3) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 4) disp |= (DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 5) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 6) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_E] | DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 7) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C]);

		if(tmp[seg] == 8) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_E] | DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		if(tmp[seg] == 9) disp |= (DISP_SEG[seg][DISP_SEG_A] | DISP_SEG[seg][DISP_SEG_B] | DISP_SEG[seg][DISP_SEG_C] |
									DISP_SEG[seg][DISP_SEG_D] | DISP_SEG[seg][DISP_SEG_F] | DISP_SEG[seg][DISP_SEG_G]);

		display(disp);
	}
}

extern void display(uint32 data)
{
	uint8 disp[3];

	if(display_write_en)
	{
		disp[0] = data & 0x0000FF;
		disp[1] = (data & 0x00FF00) >> 8;
		disp[2] = (data & 0xFF0000) >> 16;

		SPI_send_len(disp, 3);

		latch();

#if(PRJ_DEBUG && 0)
		UART_printf(&hUART_PROG, "sent to display: %x - %x %x %x\n", data, disp[2], disp[1], disp[0]);
#endif
	}
}
