#ifndef _IOBSP_H_
#define _IOBSP_H_

#include "BspCfg.h"


#define SW15_Pin GPIO_Pin_0
#define SW15_GPIO_Port GPIOC
#define SW12_Pin GPIO_Pin_1
#define SW12_GPIO_Port GPIOC
#define SW11_Pin GPIO_Pin_2
#define SW11_GPIO_Port GPIOC
#define SW14_Pin GPIO_Pin_3
#define SW14_GPIO_Port GPIOC
#define SW17_Pin GPIO_Pin_0
#define SW17_GPIO_Port GPIOA
#define SW10_Pin GPIO_Pin_1
#define SW10_GPIO_Port GPIOA
#define SW13_Pin GPIO_Pin_2
#define SW13_GPIO_Port GPIOA
#define SW16_Pin GPIO_Pin_3
#define SW16_GPIO_Port GPIOA
#define SW25_Pin GPIO_Pin_4
#define SW25_GPIO_Port GPIOA
#define SW23_Pin GPIO_Pin_4
#define SW23_GPIO_Port GPIOC
#define SW18_Pin GPIO_Pin_5
#define SW18_GPIO_Port GPIOC
#define SW19_Pin GPIO_Pin_0
#define SW19_GPIO_Port GPIOB
#define SW20_Pin GPIO_Pin_1
#define SW20_GPIO_Port GPIOB

#define LED1_Pin GPIO_Pin_6
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_Pin_9
#define LED2_GPIO_Port GPIOC

#define ACC_Pin GPIO_Pin_4
#define ACC_GPIO_Port GPIOB
#define LEFT_Pin GPIO_Pin_3
#define LEFT_GPIO_Port GPIOB

#define RIGHT_Pin GPIO_Pin_11
#define RIGHT_GPIO_Port GPIOC
#define REVERSE_Pin GPIO_Pin_10
#define REVERSE_GPIO_Port GPIOC


typedef enum
{
	/*		1	4		*/
	KEY_ACC			=  0x0001,	/*ACC----SW10*/
	KEY_RIGHT		=  0x0002,	/*RIGHT----SW11*/
	KEY_REVERSE 	=  0x0004,	/*REVERSE----SW12*/
	KEY_SPEED		=  0x0008,	/*SPEED----SW13*/
		
	KEY_LEFT		=  0x0010,	/*LEFT----SW14*/
	KEY_TURN		=  0x0020,	/*TURN----SW15*/
	KEY_RADAR		=  0x0040,	/*RADAR----SW16*/
	KEY_DOUBLE		=  0x0080,	/*DOUBLE----SW17*/

	KEY_STEER		=  0x0100,	/*STEER----SW18*/
	KEY_AUTO		=  0x0200,	/*AUTO----SW23*/
	KEY_WHEEL		=  0x0400,	/*WHEEL----SW25*/

	
	KEY_MAX			=  0xffff,	/*MAX----SW10*/

}KEY_NAME;




/*********************************
		Global	Funcation
*********************************/

void MX_GPIO_Init(void);
u32 get_key(void);



#endif


