#include "sys.h"
#include "usart.h"

/*加入以下代码,支持printf函数,而不需要选择use MicroLIB*/
#if Use_UsartDebug
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;

};

FILE __stdout;

int _sys_exit(int x)
{
    x = x;
    return 0;
}

int fputc(int ch, FILE *f)
{
    u16 i = 0;
	#if 0   //2017 -06-12 
    while(((UART5->SR & 0X40) == 0) || (i >= 65535))
        i++;
	#endif
		while((UART5->SR & 0X80) == 0)
		{
				
        	i++;
			if(i>=65535) break;
		}
      
    UART5->DR = (uint8_t) ch;
    return ch;

}

void PrintfStream(u8 *pData, u16 Size)
{
    u16 i;
    LOG("\r\n");
    for(i = 0; i < Size; i++)
    {
        LOG("%x ", *(pData + i));

    }
    LOG("\r\n");
}


/*bound:波特率*/
void Uart_Debug_init(u32 bound)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);


    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);


    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART5, &USART_InitStructure);
    USART_ClearFlag(USART2, USART_FLAG_TC);
    USART_Cmd(UART5, ENABLE);

}
#endif

