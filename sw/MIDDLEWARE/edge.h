#ifndef _EDGE_H_
#define _EDGE_H_

#include "std_def.h"
#include "project.h"

#define T_SAMPLE	10

bool BUT_M1_neu;
bool BUT_M1_pos;
bool BUT_M1_neg;

bool BUT_M2_neu;
bool BUT_M2_pos;
bool BUT_M2_neg;

bool BUT_minus_neu;
bool BUT_minus_pos;
bool BUT_minus_neg;

bool BUT_plus_neu;
bool BUT_plus_pos;
bool BUT_plus_neg;

#if(PRJ_PROGIO_IN)
bool Prog_IO_neu;
bool Prog_IO_pos;
bool Prog_IO_neg;
#endif

extern void edge_detect();
extern void edge_time_handler();

#endif // _EDGE_H_
