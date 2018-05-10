#include "BspCfg.h"
#include "LedManModule.h"
#include "AppConfig.h"
#include "UpdateManModule.h"
/////////////////////////////////////////////////////////
//                      主函数                         //
/////////////////////////////////////////////////////////

u32 test;

int main(void)
{
    //初始化硬件
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0);
    BSP_Init();
    //App初始化
    //AppCfgInit();
    //跳转APP
//    UpdateProcess(&AllBspStruct, &UserCfgSaveStruct);
    while(1)
    {
			test = get_key();
			delay_ms(1000);
			printf("key = 0x%x\r\n",test);
        AllBspStruct.pWDGBspStruct->IWDG_Feed();
    }
}


#ifdef  USE_FULL_ASSERT
/***************************************************************************//**
  * @brief  报告在检查参数发生错误时的源文件名和错误行数
  * @param  file: 指向错误文件的源文件名
  * @param  line: 错误的源代码所在行数
  * @retval 无
*******************************************************************************/
void assert_failed(uint8_t *file, uint32_t line)
{
    while (1)
    {
        LOG("Error value: file name %s in line%d\r\n", file, line);
    }
}


#endif


/******************* (C) COPYRIGHT Baony***************文件结束***********/


