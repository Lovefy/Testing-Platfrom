#include "timerledbsp.h"

static TimeLedBpsTypeDef  TimPrvDataInit;

static void TIM7_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    TIM_SetCounter(TIM7, 0);
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
    //TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM7, ENABLE);  /*Ê¹ÄÜTIMx	*/
}


static void Timer_Suspend(void)
{
    TIM_Cmd(TIM7, DISABLE);
}
static void Timer_Resume(void)
{
    TIM_Cmd(TIM7, ENABLE);
}

TimeLedBpsTypeDef  *TimerLedBspInit(void)
{
    TimPrvDataInit.TIM7_Int_Init = TIM7_Init;
    TimPrvDataInit.Timer_Suspend = Timer_Suspend;
    TimPrvDataInit.Timer_Resume = Timer_Resume;

    TIM7_Init(TIM7_ARR_VAL, TIM7_PRC_VAL);

    return &TimPrvDataInit;
}

TimeLedBpsTypeDef *GetTimerLedBspData(void)
{
    return &TimPrvDataInit;

}


