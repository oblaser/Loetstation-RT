//
// Oliver Blaser
//
// 22.05.2017
// 22.05.2017
//

#ifndef _display_H_
#define _display_H_

#include "project.h"
#include "std_def.h"

#define DISP_SEG_E_			0
#define DISP_SEG_Z_			1
#define DISP_SEG_H_			2

#define DISP_SEG_A			0
#define DISP_SEG_B			1
#define DISP_SEG_C			2
#define DISP_SEG_D			3
#define DISP_SEG_E			4
#define DISP_SEG_F			5
#define DISP_SEG_G			6

#define DISP_SEG_EG			0x000001
#define DISP_SEG_EF			0x000002
#define DISP_SEG_EE			0x000004
#define DISP_SEG_ED			0x000008
#define DISP_SEG_EC			0x000010
#define DISP_SEG_EB			0x000020
#define DISP_SEG_EA			0x000040

#define DISP_SEG_ZG			0x000100
#define DISP_SEG_ZF			0x000200
#define DISP_SEG_ZE			0x000400
#define DISP_SEG_ZD			0x000800
#define DISP_SEG_ZC			0x001000
#define DISP_SEG_ZB			0x002000
#define DISP_SEG_ZA			0x004000

#define DISP_SEG_HG			0x010000
#define DISP_SEG_HF			0x020000
#define DISP_SEG_HE			0x040000
#define DISP_SEG_HD			0x080000
#define DISP_SEG_HC			0x100000
#define DISP_SEG_HB			0x200000
#define DISP_SEG_HA			0x400000

#define DISP_IMG_COOLDOWN	(DISP_SEG_HA | DISP_SEG_HD | DISP_SEG_HE | DISP_SEG_HF | DISP_SEG_ZA | DISP_SEG_ZB | DISP_SEG_ZC | DISP_SEG_ZD | DISP_SEG_ZE | DISP_SEG_ZF | DISP_SEG_ED | DISP_SEG_EE | DISP_SEG_EF)
#define DISP_IMG_Err		(DISP_SEG_HA | DISP_SEG_HD | DISP_SEG_HE | DISP_SEG_HF | DISP_SEG_HG | DISP_SEG_ZE | DISP_SEG_ZG | DISP_SEG_EE | DISP_SEG_EG)
#define DISP_IMG_iO			(DISP_SEG_HC | DISP_SEG_ZA | DISP_SEG_ZB | DISP_SEG_ZC | DISP_SEG_ZD | DISP_SEG_ZE | DISP_SEG_ZF)
#define DISP_IMG_OFF		(DISP_SEG_HA | DISP_SEG_HB | DISP_SEG_HC | DISP_SEG_HD | DISP_SEG_HE | DISP_SEG_HF | DISP_SEG_ZA | DISP_SEG_ZE | DISP_SEG_ZF | DISP_SEG_ZG | DISP_SEG_EA | DISP_SEG_EE | DISP_SEG_EF | DISP_SEG_EG)
#define DISP_IMG_LINE_MID	(DISP_SEG_EG | DISP_SEG_ZG | DISP_SEG_HG)
#define DISP_IMG_CLEAR		(0x000000)

#define __DISP_WRITE_ENABLE()	display_write_en=true
#define __DISP_WRITE_DISABLE()	display_write_en=false

bool display_write_en;

extern void display_numeric(uint16);
extern void display(uint32);

#endif
