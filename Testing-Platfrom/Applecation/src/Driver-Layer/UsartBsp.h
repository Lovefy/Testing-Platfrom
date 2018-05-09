#ifndef __USARTBSP_H
#define __USARTBSP_H

#include "BspCfg.h"


#define SOC_BAUD   	115200

__packed typedef struct
{
    void (*Uart4_init)(FunctionalState NewState);
    u8 (*Uart4_send)(u8 *dat, u8 len);
    void (*Uart4_CMD)(FunctionalState NewState);

} UartBspTypeDef;

UartBspTypeDef *GetUSARTBspData(void);
UartBspTypeDef *USART_AllBspInit(void);


#endif

