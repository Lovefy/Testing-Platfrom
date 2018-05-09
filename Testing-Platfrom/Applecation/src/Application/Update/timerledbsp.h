#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"
#define TIM7_ARR_VAL 9999
#define TIM7_PRC_VAL 720  /*10uS*/
__packed typedef struct
{
    void (*TIM7_Int_Init)(u16 arr, u16 psc);
    void (*Timer_Suspend)(void);
    void (*Timer_Resume)(void);

} TimeLedBpsTypeDef;

TimeLedBpsTypeDef  *TimerLedBspInit(void);
TimeLedBpsTypeDef *GetTimerLedBspData(void);

#endif





