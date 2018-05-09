#include"I2cBus.h"

I2CBusTypedef I2CBusInit[IICBUS_NUMBER_MAX];

/********************BUS1 FUNCTION**********************/
static void BUS1_General_Setting(GPIOMode_TypeDef Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(BUS1_RCC_APBPeriph, ENABLE);

    GPIO_InitStructure.GPIO_Pin = BUS1_SCL_PIN | BUS1_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = Mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(BUS1_GPIO, &GPIO_InitStructure);
}

static void BUS1_IIC_Init(void)
{
    BUS1_General_Setting(GPIO_Mode_Out_PP);
}
static void BUS1_IIC_DeInit(void)
{
    BUS1_General_Setting(GPIO_Mode_AIN);
}

static	void BUS1_SDAOutCfg(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = BUS1_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(BUS1_GPIO, &GPIO_InitStructure);
}

static void BUS1_SDAInCfg(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = BUS1_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(BUS1_GPIO, &GPIO_InitStructure);
}

static void BUS1_SetSDA(void)
{
    BUS1_SET_SDA;
}

static u8 BUS1_ReadSDA(void)
{
    BUS1_READ_SDA;
}

static void BUS1_SetSCL(void)
{
    BUS1_SET_SCL;
}

static void BUS1_ClrSDA(void)
{
    BUS1_CLR_SDA;
}

static void BUS1_ClrSCL(void)
{
    BUS1_CLR_SCL;
}


/********************BUS2 FUNCTION**********************/
static void BUS2_General_Setting(GPIOMode_TypeDef Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(BUS2_RCC_APBPeriph, ENABLE);

    GPIO_InitStructure.GPIO_Pin = BUS2_SCL_PIN | BUS2_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = Mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(BUS2_GPIO, &GPIO_InitStructure);
}

static void BUS2_IIC_Init(void)
{
    BUS2_General_Setting(GPIO_Mode_Out_PP);
}
static void BUS2_IIC_DeInit(void)
{
    BUS2_General_Setting(GPIO_Mode_AIN);
}


static	void BUS2_SDAOutCfg(void)
{
    BUS2_IIC_Init();
}

static void BUS2_SDAInCfg(void)
{
    BUS2_General_Setting(GPIO_Mode_IPU);
}

static void BUS2_SetSDA(void)
{
    BUS2_SET_SDA;
}

static u8 BUS2_ReadSDA(void)
{
    BUS2_READ_SDA;
}

static void BUS2_SetSCL(void)
{
    BUS2_SET_SCL;
}

static void BUS2_ClrSDA(void)
{
    BUS2_CLR_SDA;
}

static void BUS2_ClrSCL(void)
{
    BUS2_CLR_SCL;
}


/********************BUS3 FUNCTION**********************/
static void BUS3_General_Setting(GPIOMode_TypeDef Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(BUS3_RCC_APBPeriph, ENABLE);

    GPIO_InitStructure.GPIO_Pin = BUS3_SCL_PIN | BUS3_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = Mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(BUS3_GPIO, &GPIO_InitStructure);
}

static void BUS3_IIC_Init(void)
{
    BUS3_General_Setting(GPIO_Mode_Out_PP);
}
static void BUS3_IIC_DeInit(void)
{
    BUS3_General_Setting(GPIO_Mode_AIN);
}

static	void BUS3_SDAOutCfg(void)
{
    BUS3_IIC_Init();
}

static void BUS3_SDAInCfg(void)
{
    BUS3_General_Setting(GPIO_Mode_IPU);
}

static void BUS3_SetSDA(void)
{
    BUS3_SET_SDA;
}

static u8 BUS3_ReadSDA(void)
{
    BUS3_READ_SDA;
}

static void BUS3_SetSCL(void)
{
    BUS3_SET_SCL;
}

static void BUS3_ClrSDA(void)
{
    BUS3_CLR_SDA;
}

static void BUS3_ClrSCL(void)
{
    BUS3_CLR_SCL;
}




u8 Mutil_I2CBus_Init(void)
{
    I2CBusInit[I2CBUS_NO1].Index = I2CBUS_NO1;
    I2CBusInit[I2CBUS_NO2].Index = I2CBUS_NO2;
    I2CBusInit[I2CBUS_NO3].Index = I2CBUS_NO3;

    I2CBusInit[I2CBUS_NO1].Base.ClrSCL = BUS1_ClrSCL;
    I2CBusInit[I2CBUS_NO1].Base.SDAOutCfg = BUS1_SDAOutCfg;
    I2CBusInit[I2CBUS_NO1].Base.SDAInCfg = BUS1_SDAInCfg;
    I2CBusInit[I2CBUS_NO1].Base.SetSDA = BUS1_SetSDA;
    I2CBusInit[I2CBUS_NO1].Base.ReadSDA = BUS1_ReadSDA;
    I2CBusInit[I2CBUS_NO1].Base.SetSCL = BUS1_SetSCL;
    I2CBusInit[I2CBUS_NO1].Base.ClrSDA = BUS1_ClrSDA;
    I2CBusInit[I2CBUS_NO1].Base.ClrSCL = BUS1_ClrSCL;
    I2CBusInit[I2CBUS_NO1].Base.IIC_Init = BUS1_IIC_Init;
    I2CBusInit[I2CBUS_NO1].Base.IIC_DeInit = BUS1_IIC_DeInit;

    I2CBusInit[I2CBUS_NO2].Base.ClrSCL = BUS2_ClrSCL;
    I2CBusInit[I2CBUS_NO2].Base.SDAOutCfg = BUS2_SDAOutCfg;
    I2CBusInit[I2CBUS_NO2].Base.SDAInCfg = BUS2_SDAInCfg;
    I2CBusInit[I2CBUS_NO2].Base.SetSDA = BUS2_SetSDA;
    I2CBusInit[I2CBUS_NO2].Base.ReadSDA = BUS2_ReadSDA;
    I2CBusInit[I2CBUS_NO2].Base.SetSCL = BUS2_SetSCL;
    I2CBusInit[I2CBUS_NO2].Base.ClrSDA = BUS2_ClrSDA;
    I2CBusInit[I2CBUS_NO2].Base.ClrSCL = BUS2_ClrSCL;
    I2CBusInit[I2CBUS_NO2].Base.IIC_Init = BUS2_IIC_Init;
    I2CBusInit[I2CBUS_NO2].Base.IIC_DeInit = BUS2_IIC_DeInit;

    I2CBusInit[I2CBUS_NO3].Base.ClrSCL = BUS3_ClrSCL;
    I2CBusInit[I2CBUS_NO3].Base.SDAOutCfg = BUS3_SDAOutCfg;
    I2CBusInit[I2CBUS_NO3].Base.SDAInCfg = BUS3_SDAInCfg;
    I2CBusInit[I2CBUS_NO3].Base.SetSDA = BUS3_SetSDA;
    I2CBusInit[I2CBUS_NO3].Base.ReadSDA = BUS3_ReadSDA;
    I2CBusInit[I2CBUS_NO3].Base.SetSCL = BUS3_SetSCL;
    I2CBusInit[I2CBUS_NO3].Base.ClrSDA = BUS3_ClrSDA;
    I2CBusInit[I2CBUS_NO3].Base.ClrSCL = BUS3_ClrSCL;
    I2CBusInit[I2CBUS_NO3].Base.IIC_Init = BUS3_IIC_Init;
    I2CBusInit[I2CBUS_NO3].Base.IIC_DeInit = BUS3_IIC_DeInit;


}
