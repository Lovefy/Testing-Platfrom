#ifndef __I2C_BUS_H
#define __I2C_BUS_H

#include "sys.h"

#define IICBUS_NUMBER_MAX 3

#define BUS1_RCC_APBPeriph	RCC_APB2Periph_GPIOB
#define BUS1_GPIO	  		GPIOB
#define BUS1_SCL_PIN   		GPIO_Pin_6
#define BUS1_SDA_PIN   		GPIO_Pin_7

#define BUS1_SET_SCL     	GPIO_SetBits(BUS1_GPIO,BUS1_SCL_PIN)
#define BUS1_CLR_SCL     	GPIO_ResetBits(BUS1_GPIO,BUS1_SCL_PIN)

#define BUS1_SET_SDA     	GPIO_SetBits(BUS1_GPIO,BUS1_SDA_PIN)
#define BUS1_CLR_SDA     	GPIO_ResetBits(BUS1_GPIO,BUS1_SDA_PIN)
#define BUS1_READ_SDA    	GPIO_ReadInputDataBit(BUS1_GPIO,BUS1_SDA_PIN)

#define BUS2_RCC_APBPeriph	RCC_APB2Periph_GPIOB
#define BUS2_GPIO	  		GPIOB
#define BUS2_SCL_PIN   		GPIO_Pin_10
#define BUS2_SDA_PIN   		GPIO_Pin_11

#define BUS2_SET_SCL     	GPIO_SetBits(BUS2_GPIO,BUS2_SCL_PIN)
#define BUS2_CLR_SCL     	GPIO_ResetBits(BUS2_GPIO,BUS2_SCL_PIN)

#define BUS2_SET_SDA     	GPIO_SetBits(BUS2_GPIO,BUS2_SDA_PIN)
#define BUS2_CLR_SDA     	GPIO_ResetBits(BUS2_GPIO,BUS2_SDA_PIN)
#define BUS2_READ_SDA    	GPIO_ReadInputDataBit(BUS2_GPIO,BUS2_SDA_PIN)

#define BUS3_RCC_APBPeriph	RCC_APB2Periph_GPIOC
#define BUS3_GPIO	  		GPIOC
#define BUS3_SCL_PIN   		GPIO_Pin_8
#define BUS3_SDA_PIN   		GPIO_Pin_7

#define BUS3_SET_SCL     	GPIO_SetBits(BUS3_GPIO,BUS3_SCL_PIN)
#define BUS3_CLR_SCL     	GPIO_ResetBits(BUS3_GPIO,BUS3_SCL_PIN)

#define BUS3_SET_SDA     	GPIO_SetBits(BUS3_GPIO,BUS3_SDA_PIN)
#define BUS3_CLR_SDA     	GPIO_ResetBits(BUS3_GPIO,BUS3_SDA_PIN)
#define BUS3_READ_SDA    	GPIO_ReadInputDataBit(BUS3_GPIO,BUS3_SDA_PIN)


typedef enum
{
    I2CBUS_NO1 = 0,
    I2CBUS_NO2,
    I2CBUS_NO3,
} I2CBUS_NO_ENUM;

__packed typedef struct
{
    void (*SDAOutCfg)(void);
    void (*SDAInCfg)(void);

    void (*SetSDA)(void);
    u8 (*ReadSDA)(void);
    void (*SetSCL)(void);
    void (*ClrSDA)(void);
    void (*ClrSCL)(void);

    void (*IIC_Init)(void);
    void (*IIC_DeInit)(void);

}
BusInterfaceTypedef;

__packed typedef struct
{
    BusInterfaceTypedef Base;
    I2CBUS_NO_ENUM Index; /*i2c bus index,@I2CBUS_NO_ENUM*/
}
I2CBusTypedef;

extern I2CBusTypedef I2CBusInit[IICBUS_NUMBER_MAX];
extern u8 Mutil_I2CBus_Init(void);



#endif

