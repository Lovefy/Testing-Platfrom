#include "BspCfg.h"


static TimPrvDataTypedef TimPrvDataInit;
static TimerBspTypeDef TimerBspInit;

static u32 Set_Cmr3_NewCompareVal(TIM_TypeDef *TIMx, u32 Cmr_Time);


void TIM1_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_InternalClockConfig(TIM1);

    TIM_DeInit(TIM1);
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    TIM_SetCounter(TIM1, 0);
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    TIM_ARRPreloadConfig(TIM1, DISABLE);
    /*
    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE );
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    */
    TIM_Cmd(TIM1, ENABLE);
}

void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_ClearFlag(TIM1, TIM_IT_Update);

        printf("TIM1_UP_IRQHandler\r\n");
    }
}



void TIM2_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef 	NVIC_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = 5999;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);

    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);
    TIM_ClearFlag(TIM2, TIM_IT_CC3);
    TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE );

    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}


void TIM2_IRQHandler(void)
{
    static u8 Count = 0;
    if(TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
    {
        TIM_ClearFlag(TIM2, TIM_IT_CC3);
        Set_Cmr3_NewCompareVal(TIM2, 9); /*10*100us=1ms*/

        if(Count++ >= 99) /*100ms*/
        {
            Count = 0;

            /*提供固定定时标志*/
            TimPrvDataInit.TimOver_100ms_Flag[0] = ENABLE;
            TimPrvDataInit.TimOver_100ms_Flag[1] = ENABLE;
            TimPrvDataInit.TimOver_100ms_Flag[2] = ENABLE;
            TimPrvDataInit.TimOver_100ms_Flag[3] = 0x77; /*内部G-SENSER 外部G-SEN1 外部G-SEN2  NULL  延时关机*/
            TimPrvDataInit.TimOver_100ms_Flag[4] = ENABLE; /*CAN1 Callback TimerFlag*/
            TimPrvDataInit.TimOver_100ms_Flag[5] = ENABLE; /*CAN2 Callback TimerFlag*/
            TimPrvDataInit.TimOver_100ms_Flag[6] = ENABLE; /*CAN1/2 Callback TimerFlag*/


            TimPrvDataInit.Tim_1s_Cnt++;
            if(TimPrvDataInit.Tim_1s_Cnt >= 10)
            {
                TimPrvDataInit.Tim_1s_Cnt = 0;
                TimPrvDataInit.TimOver_1S_Flag = ENABLE;
            }
            /*提供可调定时功能*/
            TimPrvDataInit.General_Cnt[0]++;
            if(TimPrvDataInit.General_Cnt[0] >= TimPrvDataInit.General_Compare_Val[0])
            {
                TimPrvDataInit.General_Cnt[0] = 0;
                TimPrvDataInit.General_Over_Flag[0] = ENABLE;
            }
        }

        /*Create a LINK-LIST TIMER*/
        Timer_Link_Callback();
    }


}


static void TIM5_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    TIM_SetCounter(TIM5, 0);
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE );

    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /*TIM_Cmd(TIM5, ENABLE);  //使能TIMx	*/
}


static void TIM6_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
    TIM_SetCounter(TIM6, 0);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE );

    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /*TIM_Cmd(TIM6, ENABLE);  //使能TIMx	*/
}

static void TIM7_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    TIM_SetCounter(TIM7, 0);
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE );

    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /*TIM_Cmd(TIM7, ENABLE);  使能TIMx	*/
}



static u32 Set_Cmr1_NewCompareVal(TIM_TypeDef *TIMx, u32 Cmr_Time)
{
    u32 CmrVal = 0;
    TIM_ClearFlag(TIMx, TIM_IT_CC1);
    CmrVal = TIM_GetCounter(TIMx) + Cmr_Time;
    if(CmrVal >= 65536)
        CmrVal -= 65536;
    TIM_SetCompare1(TIMx, CmrVal);
    return CmrVal;
}

static u32 Set_Cmr2_NewCompareVal(TIM_TypeDef *TIMx, u32 Cmr_Time)
{
    u32 CmrVal = 0;
    TIM_ClearFlag(TIMx, TIM_IT_CC2);
    CmrVal = TIM_GetCounter(TIMx) + Cmr_Time;
    if(CmrVal >= 65536)
        CmrVal -= 65536;
    TIM_SetCompare2(TIMx, CmrVal);
    return CmrVal;
}

static u32 Set_Cmr3_NewCompareVal(TIM_TypeDef *TIMx, u32 Cmr_Time)
{
    u32 CmrVal = 0;
    TIM_ClearFlag(TIMx, TIM_IT_CC3);
    CmrVal = TIM_GetCounter(TIMx) + Cmr_Time;
    if(CmrVal >= 65536)
        CmrVal -= 65536;
    TIM_SetCompare3(TIMx, CmrVal);
    return CmrVal;
}

static u32 Set_Cmr4_NewCompareVal(TIM_TypeDef *TIMx, u32 Cmr_Time)
{
    u32 CmrVal = 0;
    TIM_ClearFlag(TIMx, TIM_IT_CC4);
    CmrVal = TIM_GetCounter(TIMx) + Cmr_Time;
    if(CmrVal >= 65536)
        CmrVal -= 65536;
    TIM_SetCompare4(TIMx, CmrVal);
    return CmrVal;
}

static void Timer_Suspend(void)
{

}
static void Timer_Resume(void)
{

}

TimerBspTypeDef *GetTimerBspData(void)
{
    return &TimerBspInit;
}

pTimerBspTypeDef All_Timer_Bsp_init(void)
{
    TimerBspInit.TIM1_Int_Init = TIM1_Int_Init;
    TimerBspInit.TIM2_Int_Init = TIM2_Int_Init;
    TimerBspInit.TIM5_Int_Init = TIM5_Int_Init;
    TimerBspInit.TIM6_Int_Init = TIM6_Int_Init;
    TimerBspInit.TIM7_Int_Init = TIM7_Int_Init;

    TimerBspInit.Set_Cmr1_NewCompareVal = Set_Cmr1_NewCompareVal;
    TimerBspInit.Set_Cmr2_NewCompareVal = Set_Cmr2_NewCompareVal;
    TimerBspInit.Set_Cmr3_NewCompareVal = Set_Cmr3_NewCompareVal;
    TimerBspInit.Set_Cmr4_NewCompareVal = Set_Cmr4_NewCompareVal;
    TimerBspInit.TIM_GetFlagStatus = TIM_GetFlagStatus;
    TimerBspInit.Timer_Resume = Timer_Resume;
    TimerBspInit.Timer_Suspend = Timer_Suspend;

    TimerBspInit.pTimPrvDataInit = &TimPrvDataInit;

    /*Initial Timer-link point*/
    pTmrlistInit = InitTmrLinkedList();

    TIM1_Int_Init(TIM1_ARR_VAL, TIM1_PRC_VAL);
    TIM2_Int_Init(TIM2_ARR_VAL, TIM2_PRC_VAL);
    TIM5_Int_Init(TIM5_ARR_VAL, TIM5_PRC_VAL);
    TIM6_Int_Init(TIM6_ARR_VAL, TIM6_PRC_VAL);
    TIM7_Int_Init(TIM7_ARR_VAL, TIM7_PRC_VAL);

    return &TimerBspInit;
}
























