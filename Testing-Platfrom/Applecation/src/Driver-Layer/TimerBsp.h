#ifndef __TIMERBSP_H
#define __TIMERBSP_H

#include "sys.h"


#define TIM1_ARR_VAL 999
#define TIM1_PRC_VAL 7200 /*100US*/

#define TIM2_ARR_VAL 65535
#define TIM2_PRC_VAL 7200 /*10US*/
///*REAR SIGNAL*/
#define TIM3_ARR_VAL 999 /*10US*1000=100MS filter*/
#define TIM3_PRC_VAL 720 /*10US*/
///*LEFT SIGNAL*/
//#define TIM5_ARR_VAL 999
//#define TIM5_PRC_VAL 720 /*10US*/
///*RIGHT SIGNAL*/
//#define TIM6_ARR_VAL 999
//#define TIM6_PRC_VAL 720 /*10US*/
///*ACC SIGNAL*/
//#define TIM7_ARR_VAL 999
//#define TIM7_PRC_VAL 7200 /**/

__packed typedef struct
{
    u8 TimOver_1ms_Flag[2];
    u8 TimOver_10ms_Flag[2];
    u8 TimOver_100ms_Flag[2];
    u8 Tim_1s_Cnt;
    u8 TimOver_1S_Flag;

    u16 General_Cnt[1];
    u16 General_Compare_Val[1];
    u8 General_Over_Flag[1];

} TimPrvDataTypedef;


#if 0
__packed typedef struct
{
    void (*TIM1_Int_Init)(u16 arr, u16 psc);
    void (*TIM2_Int_Init)(u16 arr, u16 psc);
    void (*TIM5_Int_Init)(u16 arr, u16 psc);
    void (*TIM6_Int_Init)(u16 arr, u16 psc);
    void (*TIM7_Int_Init)(u16 arr, u16 psc);

    u32 (*Set_Cmr1_NewCompareVal)(TIM_TypeDef *TIMx, u32 Cmr_Time);
    u32 (*Set_Cmr2_NewCompareVal)(TIM_TypeDef *TIMx, u32 Cmr_Time);
    u32 (*Set_Cmr3_NewCompareVal)(TIM_TypeDef *TIMx, u32 Cmr_Time);
    u32 (*Set_Cmr4_NewCompareVal)(TIM_TypeDef *TIMx, u32 Cmr_Time);
    FlagStatus (*TIM_GetFlagStatus)(TIM_TypeDef *TIMx, uint16_t TIM_FLAG);

    void (*Timer_Suspend)(void);
    void (*Timer_Resume)(void);

    TimPrvDataTypedef *pTimPrvDataInit;
}
TimerBspTypeDef, *pTimerBspTypeDef;

#endif
extern TimPrvDataTypedef GsTimPrvDataInit;

//TimerBspTypeDef *GetTimerBspData(void);
//pTimerBspTypeDef All_Timer_Bsp_init(void);


void AllTimerInit(void);




#endif
