//
// Oliver Blaser
//
// 10.05.2017
// 10.05.2017
//

#ifndef _project_h_
#define _project_h_

#define PRJ_DEBUG				0

#if(PRJ_DEBUG)

#define PRJ_UART_PROG			1

#define PRJ_PROGIO_OUT			0
#define PRJ_PROGIO_IN			1
#define PRJ_PROGIO_CLKOUT		0

#else

#define PRJ_UART_PROG			0

#define PRJ_PROGIO_OUT			0
#define PRJ_PROGIO_IN			1
#define PRJ_PROGIO_CLKOUT		0

#if(PRJ_PROGIO_IN == 0)
#error "Prog_IO has to be input in non-debugging mode!"
#endif

#endif

#if((PRJ_PROGIO_OUT + PRJ_PROGIO_IN + PRJ_PROGIO_CLKOUT) > 1)
#error "Prog_IO has more than one function!"
#endif

#endif
