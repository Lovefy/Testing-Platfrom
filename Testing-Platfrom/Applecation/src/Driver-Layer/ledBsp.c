#include "BspCfg.h"

//static LedBspTypeDef LedBspInit;

void Led_Init(GPIOMode_TypeDef GPIOMode)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(STATUS_LED_PERIPH_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = STATUS_LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIOMode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(STATUS_LED_PORT, &GPIO_InitStructure);
    SET_STATUS_LED;

}

//static void Led_Set(void)
//{
//	SET_STATUS_LED;
//}

//static void Led_Reset(void)
//{
//	CLR_STATUS_LED;
//}

//static void Led_Reverse(void)
//{
//	REV_STATUS_LED;
//}


//LedBspTypeDef *GetLedBspData(void)
//{
//	return &LedBspInit;
//}
//
//LedBspTypeDef* Led_AllBspInit(void)
//{
////	LedBspInit.Init=Led_Init;
////	LedBspInit.Set=Led_Set;
////	LedBspInit.Reset=Led_Reset;
////	LedBspInit.Reverse=Led_Reverse;
//	Led_Init(GPIO_Mode_AF_PP);
//	return &LedBspInit;
//}
