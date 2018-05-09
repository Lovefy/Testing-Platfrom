#include "BspCfg.h"

static OutInBspTypeDef  OutInBspInit;
struct IOPrvDataTypeDef IOPrvDataInit;

static void GPIOGeneralSetting(GPIOMode_TypeDef GPIO_Mode, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*DVD_POWER_EN控制*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

static void OutputEnable_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIOGeneralSetting(GPIO_Mode_Out_PP, SYS_PC_EN_GPIO, SYS_PC_EN_PIN);
    GPIOGeneralSetting(GPIO_Mode_Out_PP, UART_PC_GPIO, UART_PC_PIN);
    GPIOGeneralSetting(GPIO_Mode_Out_PP, CORE_POWER_GPIO, CORE_POWER_PIN);

    /*DEFAULT VALUE */
    ON_CORE_POWER_CTL;
    ON_SYS_PC_EN_CTL;
    ON_UART_EN_CTL;


}


static void OutputEnable_DeInit(void)
{
    /*DEFAULT VALUE */
    //OFF_SYS_PC_EN_CTL;
    //OFF_UART_EN_CTL;

    GPIOGeneralSetting(GPIO_Mode_AIN, SYS_PC_EN_GPIO, SYS_PC_EN_PIN);
    GPIOGeneralSetting(GPIO_Mode_AIN, UART_PC_GPIO, UART_PC_PIN);

}


static void Set_IO_Control(u8 Index, FunctionalState NewState)
{
    switch(Index)
    {
    case SYS_PC_EN_INDEX:
        if(ENABLE == NewState)
        {
            ON_SYS_PC_EN_CTL;
        }
        else
        {
            OFF_SYS_PC_EN_CTL;
        }
        break;

    case   UART_PC_INDEX:
        if(ENABLE == NewState)
        {
            ON_UART_EN_CTL;
        }
        else
        {
            OFF_UART_EN_CTL;
        }
        break;

    default:
        break;
    }

}




static u8 Get_IO_Control(u8 Index)
{
    switch(Index)
    {

    case SYS_PC_EN_INDEX:
    {

        return SYS_PC_EN_IN;

    }
    case   UART_PC_INDEX:
    {
        return UART_EN_CTL_IN;
    }

    default:
        return 0xff;/*返回一个固定在，调用时应先判断*/

    }

}


static u8 OutIn_Suspend(void)
{
    IOPrvDataInit.isStandby = ENABLE;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
    OutInBspInit.OutputEnable_DeInit();

#if USE_OUTINBSP_DEBUG==1
    LOG("OutIn_Suspend\r\n");
#endif

    return 0;
}

static u8 OutIn_Resume(void)
{
    IOPrvDataInit.isStandby = DISABLE;
    OutInBspInit.OutputEnable_Init();
#if USE_OUTINBSP_DEBUG==1
    LOG("OutIn_Resume\r\n");
#endif
    return 0;
}



OutInBspTypeDef *GetOutInBspData(void)
{
    return &OutInBspInit;
}


OutInBspTypeDef *OutInAllBspInit(void)
{

    OutInBspInit.pIOPrvDataInit = &IOPrvDataInit;
    OutInBspInit.OutputEnable_Init = OutputEnable_Init;
    OutInBspInit.OutputEnable_DeInit = OutputEnable_DeInit;
    OutInBspInit.Set_IO_Control = Set_IO_Control;
    OutInBspInit.Get_IO_Control = Get_IO_Control;

    OutInBspInit.OutIn_Suspend = OutIn_Suspend;
    OutInBspInit.OutIn_Resume = OutIn_Resume;

    OutputEnable_Init();


    return &OutInBspInit;
}


