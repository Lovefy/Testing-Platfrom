#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"

#define	Use_UsartDebug			1

#define DEBUG_BAUD 	115200

#define PRINTF_DEBUG_POS  {LOG("%s %s\r\n",__FILE__,__LINE__);}



void PrintfStream(u8 *pData, u16 Size);
void Uart_Debug_init(u32 bound);

#endif


