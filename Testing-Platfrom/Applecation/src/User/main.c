#include "BspCfg.h"
#include "LedManModule.h"
#include "AppConfig.h"
#include "UpdateManModule.h"
/////////////////////////////////////////////////////////
//                      ������                         //
/////////////////////////////////////////////////////////

u32 test;

int main(void)
{
    //��ʼ��Ӳ��
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0);
    BSP_Init();
    //App��ʼ��
    //AppCfgInit();
    //��תAPP
//    UpdateProcess(&AllBspStruct, &UserCfgSaveStruct);
    while(1)
    {
			test = get_key();
//        AllBspStruct.pWDGBspStruct->IWDG_Feed();
    }
}


#ifdef  USE_FULL_ASSERT
/***************************************************************************//**
  * @brief  �����ڼ�������������ʱ��Դ�ļ����ʹ�������
  * @param  file: ָ������ļ���Դ�ļ���
  * @param  line: �����Դ������������
  * @retval ��
*******************************************************************************/
void assert_failed(uint8_t *file, uint32_t line)
{
    while (1)
    {
        LOG("Error value: file name %s in line%d\r\n", file, line);
    }
}


#endif


/******************* (C) COPYRIGHT Baony***************�ļ�����***********/


