#include "BspCfg.h"

static UartBspTypeDef UartBspInit;

static void Uart4_init(FunctionalState NewState)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    USART_DeInit(UART4);
    //USART_DeInit(USART3);
    GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, DISABLE);
    if(NewState == ENABLE)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        USART_InitStructure.USART_BaudRate = SOC_BAUD;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Tx;
        USART_Init(UART4, &USART_InitStructure);
        USART_ClearFlag(UART4, USART_FLAG_TC);
        USART_Cmd(UART4, ENABLE);
    }
    else
    {

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        USART_Cmd(UART4, DISABLE);


    }

}

static u8 Uart4_send(u8 *dat, u8 len)
{

    u8 i;

    while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
    for(i = 0; i < len; i++)
    {

        USART_SendData(UART4, *dat);
        while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
        dat++;

    }

    return 0;
}


UartBspTypeDef *GetUSARTBspData(void)
{

    return &UartBspInit;

}
UartBspTypeDef *USART_AllBspInit(void)
{
    UartBspInit.Uart4_init = Uart4_init;
    UartBspInit.Uart4_send = Uart4_send;
    UartBspInit.Uart4_init(DISABLE);
    return &UartBspInit;

}








