#include "edge.h"
#include "HW.h"

static bool BUT_M1_alt = 0;
static bool BUT_M2_alt = 0;
static bool BUT_minus_alt = 0;
static bool BUT_plus_alt = 0;

#if(PRJ_PROGIO_IN)
static bool Prog_IO_alt = 0;
#endif

static uint16 tmr_edge = 0;

extern void edge_detect()
{
	if(tmr_edge <= 0)
	{
		tmr_edge = T_SAMPLE;

		BUT_M1_neu = HAL_GPIO_ReadPin(BUT_M1);

		BUT_M2_neu = HAL_GPIO_ReadPin(BUT_M2);

		BUT_minus_neu = HAL_GPIO_ReadPin(BUT_minus);

		BUT_plus_neu = HAL_GPIO_ReadPin(BUT_plus);

#if(PRJ_PROGIO_IN)
		Prog_IO_neu = HAL_GPIO_ReadPin(Prog_IO);
#endif

	}

	BUT_M1_pos = ~BUT_M1_alt & BUT_M1_neu;
	BUT_M1_neg = BUT_M1_alt & ~BUT_M1_neu;
	BUT_M1_alt = BUT_M1_neu;

	BUT_M2_pos = ~BUT_M2_alt & BUT_M2_neu;
	BUT_M2_neg = BUT_M2_alt & ~BUT_M2_neu;
	BUT_M2_alt = BUT_M2_neu;

	BUT_minus_pos = ~BUT_minus_alt & BUT_minus_neu;
	BUT_minus_neg = BUT_minus_alt & ~BUT_minus_neu;
	BUT_minus_alt = BUT_minus_neu;

	BUT_plus_pos = ~BUT_plus_alt & BUT_plus_neu;
	BUT_plus_neg = BUT_plus_alt & ~BUT_plus_neu;
	BUT_plus_alt = BUT_plus_neu;

#if(PRJ_PROGIO_IN)
	Prog_IO_pos = ~Prog_IO_alt & Prog_IO_neu;
	Prog_IO_neg = Prog_IO_alt & ~Prog_IO_neu;
	Prog_IO_alt = Prog_IO_neu;
#endif
}

extern void edge_time_handler()
{
	if(tmr_edge > 0) tmr_edge--;
}
