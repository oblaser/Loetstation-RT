//
// Oliver Blaser
//
// 10.05.2017
// 10.05.2017
//

#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f0xx_hal.h"
#include "project.h"
#include "std_def.h"

extern void SPI1_Init(void);
extern void SPI_send_len(uint8 *, uint16);

#endif
