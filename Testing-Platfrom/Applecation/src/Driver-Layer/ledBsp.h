#ifndef __LEDBSP_H
#define __LEDBSP_H

#include "BspCfg.h"

#define STATUS_LED_PORT  	 				GPIOC
#define STATUS_LED_PIN    				GPIO_Pin_6
#define STATUS_LED_PERIPH_CLK  		RCC_APB2Periph_GPIOC

#define SET_STATUS_LED 		GPIO_SetBits(STATUS_LED_PORT,STATUS_LED_PIN)
#define CLR_STATUS_LED 		GPIO_ResetBits(STATUS_LED_PORT,STATUS_LED_PIN)
#define REV_STATUS_LED 		GPIO_Reverse(STATUS_LED_PORT,STATUS_LED_PIN)



void Led_Init(GPIOMode_TypeDef GPIOMode);

//__packed typedef struct
//{
//   void (*Init)(GPIOMode_TypeDef GPIOMode);
//	 void	(*Set)(void);
//	 void	(*Reset)(void);
//	 void	(*Reverse)(void);
//
//}
//LedBspTypeDef;

//LedBspTypeDef *GetLedBspData(void);
//LedBspTypeDef* Led_AllBspInit(void);


#endif
