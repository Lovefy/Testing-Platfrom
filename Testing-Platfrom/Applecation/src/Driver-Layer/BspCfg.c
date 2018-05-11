#include "BspCfg.h"
#include "malloc.h"
#include "AppConfig.h"
AllBsp_Typedef AllBspStruct;    /*定义硬件结构体全局变量*/

AllBsp_Typedef *GetAllBspPrvData(void)
{
    return &AllBspStruct;
}


uint8_t BSP_Init(void)
{

    JTAG_Set(SWD_ENABLE);
    delay_init(72);
    NVIC_Configuration();
    Uart_Debug_init(DEBUG_BAUD);
//    Led_Init(GPIO_Mode_Out_PP);
	MX_GPIO_Init();
	AllTimerInit();

    return SUCCESS;
}


