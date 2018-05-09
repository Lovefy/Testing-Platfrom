#ifndef __OUTINBSP_H
#define __OUTINBSP_H

#include "sys.h"

#define USE_OUTINBSP_DEBUG  1

#define SYS_PC_EN_GPIO 			GPIOA
#define SYS_PC_EN_PIN 			GPIO_Pin_0

#define UART_PC_GPIO 				GPIOA
#define UART_PC_PIN 				GPIO_Pin_1

#define DVD_POWER_GPIO 			GPIOB
#define DVD_POWER_PIN 			GPIO_Pin_9

#define CCD_POWER_GPIO 			GPIOB
#define CCD_POWER_PIN 			GPIO_Pin_15

#define CORE_POWER_GPIO 		GPIOB
#define CORE_POWER_PIN 			GPIO_Pin_14

#define ON_SYS_PC_EN_CTL 	      GPIO_ResetBits(SYS_PC_EN_GPIO,SYS_PC_EN_PIN)
#define ON_UART_EN_CTL 	        GPIO_SetBits(UART_PC_GPIO,UART_PC_PIN)
#define ON_CORE_POWER_CTL 	    GPIO_SetBits(CORE_POWER_GPIO,CORE_POWER_PIN)

#define OFF_SYS_PC_EN_CTL 	    GPIO_SetBits(SYS_PC_EN_GPIO,SYS_PC_EN_PIN)
#define OFF_UART_EN_CTL 	      GPIO_ResetBits(UART_PC_GPIO,UART_PC_PIN)
#define OFF_CORE_POWER_CTL 	      GPIO_ResetBits(CORE_POWER_GPIO,CORE_POWER_PIN)



#define SYS_PC_EN_IN   		      GPIO_ReadInputDataBit(SYS_PC_EN_GPIO,SYS_PC_EN_PIN)
#define UART_EN_CTL_IN    		 	GPIO_ReadInputDataBit(UART_PC_GPIO,UART_PC_PIN)



/*end_su*/


__packed struct IOPrvDataTypeDef
{
    u8 Status;

    u8 isStandby;
};




typedef enum
{
    SYS_PC_EN_INDEX = 0,
    UART_PC_INDEX,
}
IO_CTL_INDEX_ENUM;



__packed typedef struct
{
    void (*OutputEnable_Init)(void);
    void (*OutputEnable_DeInit)(void);
    u8 (*OutIn_Suspend)(void);
    u8 (*OutIn_Resume)(void);
    void (*Set_IO_Control)(u8 Index, FunctionalState NewState);
    u8 (*Get_IO_Control)(u8 Index);

    struct IOPrvDataTypeDef *pIOPrvDataInit;


}
OutInBspTypeDef, *pOutInBspTypeDef;

OutInBspTypeDef *GetOutInBspData(void);
OutInBspTypeDef *OutInAllBspInit(void);


#endif

