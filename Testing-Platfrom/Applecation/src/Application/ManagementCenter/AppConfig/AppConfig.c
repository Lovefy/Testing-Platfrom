
#include "UpdateManModule.h"
#include "AppConfig.h"
#include "CRC_Check.h"

UserCfgInfTypeDef UserCfgSaveStruct;

const u8 Hardware[HARDWARE_VRDION_BYTE] = {0x00, 0x00, HARDWARE_VERSION, RESERVE_BYTE}; /*32 STM32 18 V18 00�汾*/
const u8 BootloaderVer[BOOTLOADER_VERSION_BYTE] = {0x00, 0x00, BOOTLOADER_KERNEL_VERSION, RESERVE_BYTE};
//const u8 AppVertion[APP_VERSION_BYTE ]={APP_MAIN_VERSION,APP_SUB_VERSION,APP_PATCH_VERSION,RESERVE_BYTE};
u8 Send_data[] = {0xfc, 0x00, 0x11, 0xFF, 0xEE, 0x00, 0x00, 0x00,  0xA1, 0x04, 0x00, 0x05, 0xFF, 0xFF, 0x9E, 0xFA, 0x2D};

//App��ʼ��
u8 AppCfgInit(void)
{
    //����ϵͳ�����������Ϣ
    UserCfgRestore(&AllBspStruct);
    LOG("\r\nUser CfgInf finish\r\n");
    return SUCCESS;
}

//�ص��û�����
static u8 UserCfgRestore(AllBsp_Typedef *pAllBspInit)
{
    u8 Len[2];
    Len[0] = sizeof(UserCfgSaveStruct) / 2; //���½ṹ��ĵ�ַ
    Len[1] = sizeof(UserCfgSaveStruct.UpdateCfgInf) / 2; //�û����õĵ�ַ
    //�ص����ʼ��������Ϣ
    pAllBspInit->pFlashBspStruct->Flash_Read(EEPROM_FLASH_BASE, (u16 *)&UserCfgSaveStruct, Len[0]);
    LOG("=================USER CONFIGURETION=================\r\n");
    LOG("OriginalUser CfgInf:DefaultCfg LowVolValAdc LowVolValRef  G_SenserEnStatus  IN_G_SenserLevel\r\n");
    LOG("%d %d %d %d %d \r\n",

        UserCfgSaveStruct.AppConfigInf.DefaultCfg,
        UserCfgSaveStruct.AppConfigInf.LowVolValAdc,
        UserCfgSaveStruct.AppConfigInf.LowVolValRef,
        UserCfgSaveStruct.AppConfigInf.G_SenserEnStatus,
        UserCfgSaveStruct.AppConfigInf.IN_G_SenserLevel

       );
    LOG("\r\n");

#if 0
    if(UserCfgSaveStruct.AppConfigInf.DefaultCfg != DEFAULT_SET)
    {
        //ϵͳ��������

        UserCfgSaveStruct.AppConfigInf.DefaultCfg = DEFAULT_SET;
        pAllBspInit->pFlashBspStruct->Flash_Write(EEPROM_FLASH_BASE, (u16 *)&UserCfgSaveStruct, Len[0]);
        pAllBspInit->pFlashBspStruct->Flash_Read(EEPROM_FLASH_BASE, (u16 *)&UserCfgSaveStruct, Len[0]);

        LOG("Reload Default Configuretion\r\n");
        LOG("NEW Default Configuretion\r\n");
        LOG("NEW User:%d %d %d %d %d %d %d\r\n",
            UserCfgSaveStruct.UpdateCfgInf.NeedUpdateFlag,
            UserCfgSaveStruct.UpdateCfgInf.PackTotalSize,

            UserCfgSaveStruct.UpdateCfgInf.CRCVal,
            UserCfgSaveStruct.AppConfigInf.LowVolValAdc,
            UserCfgSaveStruct.AppConfigInf.LowVolValRef,
            UserCfgSaveStruct.AppConfigInf.G_SenserEnStatus,
            UserCfgSaveStruct.AppConfigInf.IN_G_SenserLevel

           );

    }
#endif //������Ϣ�ı���ͻָ�����APP����ʵ��
    return SUCCESS;
}
u16 TempBuf[1024 / 2];
//��������
u8 UpdateProcess(AllBsp_Typedef *pAllBspInit, UserCfgInfTypeDef *pUserCfgSaveInit)
{
    u8 m_PerIndexAddr = 0;
    u32 PerIndexAddrTemp;
    u16 m_NowPackSize;
    u8 TotalPerIndex;
    u8 TimOutCnt = 0;
    u8 Sendtime;
    u16 crc_vel;
    // u8 KeyDownCnt=0;

    u8 Len[2];
    Len[0] = sizeof(UserCfgSaveStruct) / 2; //���½ṹ��ĵ�ַ
    Len[1] = sizeof(UserCfgSaveStruct.UpdateCfgInf) / 2; //�û����õĵ�ַ
    //3S����Ƿ���96�ż�����
    while(1)
    {
#ifdef USE_RECORVE
        if(pAllBspInit->pIRBspStruct->IRKeyProcess())//��ⰴ���Ƿ���
        {

            pAllBspInit->pIRBspStruct->pIRPrvDataInit->KeyUpdateFlag = DISABLE;

            LOG("Key:%d r\n", pAllBspInit->pIRBspStruct->pIRPrvDataInit->KeyLastValue);

            if(pAllBspInit->pIRBspStruct->pIRPrvDataInit->KeyLastValue == 96)
            {

                SET_STATUS_LED;
recover:
                //�ָ���������
                while(1)
                {
                    m_NowPackSize = 1024;
                    PerIndexAddrTemp = RECOVER_FLASH_BASE + m_PerIndexAddr * 1024;
                    //�����������ݵ�������
                    pAllBspInit->pFlashBspStruct->Flash_Read(PerIndexAddrTemp, (u16 *)&TempBuf[0], m_NowPackSize / 2); //��ǰ���Ĵ�С
                    //д���������ݵ�APP��
                    PerIndexAddrTemp = APP_FLASH_BASE + m_PerIndexAddr * 1024;
                    pAllBspInit->pWDGBspStruct->IWDG_Feed();
                    __disable_irq();  //�ر������ж�
                    pAllBspInit->pFlashBspStruct->Flash_Write(PerIndexAddrTemp, (u16 *)&TempBuf[0], m_NowPackSize / 2);
                    __enable_irq();   //���´������ж�
                    pAllBspInit->pWDGBspStruct->IWDG_Feed();

                    LOG("CpRecover sector:%d size:%d Addr:%d\r\n", m_PerIndexAddr, m_NowPackSize, PerIndexAddrTemp);


                    if(m_PerIndexAddr >= (RECOVER_FLASH_SIZE - 1))
                    {
                        m_PerIndexAddr = 0;
                        //ϵͳ��������
                        UserCfgSaveStruct.UpdateCfgInf.NeedUpdateFlag = ALREADY_UPDATE_FLAG;
                        UserCfgSaveStruct.UpdateCfgInf.PackTotalSize = 0x00;
                        UserCfgSaveStruct.UpdateCfgInf.CRCVal = 0;
                        UserCfgSaveStruct.UpdateCfgInf.BootloaderVerion[0] = BootloaderVer[0];
                        UserCfgSaveStruct.UpdateCfgInf.BootloaderVerion[1] = BootloaderVer[1];
                        UserCfgSaveStruct.UpdateCfgInf.BootloaderVerion[2] = BootloaderVer[2];
                        UserCfgSaveStruct.UpdateCfgInf.BootloaderVerion[3] = BootloaderVer[3];

                        UserCfgSaveStruct.UpdateCfgInf.HardwareVersion[0]  = Hardware[0];
                        UserCfgSaveStruct.UpdateCfgInf.HardwareVersion[1]  = Hardware[1];
                        UserCfgSaveStruct.UpdateCfgInf.HardwareVersion[2]  = Hardware[2];
                        UserCfgSaveStruct.UpdateCfgInf.HardwareVersion[3]  = Hardware[3];


                        //���ø�����ɱ�־λ
                        pAllBspInit->pFlashBspStruct->Flash_Write(EEPROM_FLASH_BASE, (u16 *)&UserCfgSaveStruct, Len[0]);
                        pAllBspInit->pFlashBspStruct->Flash_Read(EEPROM_FLASH_BASE, (u16 *)&UserCfgSaveStruct, Len[0]);

                        /*���EEPROM������ַ����*/

                        LOG("Copy Data finish\r\n");

                        break;
                    }
                    m_PerIndexAddr++;
                    pAllBspInit->pWDGBspStruct->IWDG_Feed();
                }
                //�ر���ʾ
                CLR_STATUS_LED;
                LOG("Restart 1��\r\n");

                NVIC_SystemReset();
                while(1);//ֹͣι�����Ա����¸�λ*/
            }
        }
#endif


        if((TIM_GetFlagStatus(TIM7, TIM_FLAG_Update)) != RESET)
        {
            TIM_ClearFlag(TIM7, TIM_FLAG_Update);
            TIM_SetAutoreload(TIM7, TIM7_ARR_VAL);
            TimOutCnt++;
            if(TimOutCnt % 2 == 0)
            {
                REV_STATUS_LED;
                LOG("EV_STATUS_LED��%d\r\n", TimOutCnt / 2);
            }
            if(TimOutCnt >= 20)
            {
                TimOutCnt = 0;
                break;
            }

        }

        pAllBspInit->pWDGBspStruct->IWDG_Feed();
    }
    //�������
    if(pUserCfgSaveInit->UpdateCfgInf.NeedUpdateFlag == ALREADY_UPDATE_FLAG || (pUserCfgSaveInit->UpdateCfgInf.NeedUpdateFlag == DEFAULT_CFG))
    {

        LOG("ALREADY_UPDATE \r\n");
        if (((*(__IO uint32_t *)(APP_FLASH_BASE + 4)) & 0xFF000000 ) == 0x8000000)
        {

            LOG("APP:%x \r\n", (*(__IO uint32_t *)(APP_FLASH_BASE + 4)));
            LOG("Jump to app \r\n");
            delay_ms(10);
            pAllBspInit->pWDGBspStruct->IWDG_Feed();
            iap_load_app(APP_FLASH_BASE);
        }
#ifdef USE_RECORVE
        else //���ƻָ��������ݵ�APP��
        {
            SET_STATUS_LED;
            delay_ms(1000);
            CLR_STATUS_LED;
            delay_ms(1000);
            LOG("Jump to restore\r\n");

            goto recover;
        }
#endif
        while(1);//ֹͣι�����Ա����¸�λ
    }//��Ҫ����
    else if(pUserCfgSaveInit->UpdateCfgInf.NeedUpdateFlag == NEED_UPDATE_FLAG)
    {

        LOG("Start update!\r\n");
        TotalPerIndex = pUserCfgSaveInit->UpdateCfgInf.PackTotalSize / 1024;
        if(pUserCfgSaveInit->UpdateCfgInf.PackTotalSize % 1024 != 0)
        {
            TotalPerIndex += 1;
        }

        LOG("Pack num:%d Pack size:%d\r\n", TotalPerIndex, 1024);

        while(1)
        {

            if(m_PerIndexAddr <= TotalPerIndex - 2) //
            {
                m_NowPackSize = 1024;
            }
            else if (m_PerIndexAddr == TotalPerIndex - 1)
            {
                m_NowPackSize = pUserCfgSaveInit->UpdateCfgInf.PackTotalSize % 1024;
            }

            PerIndexAddrTemp = RESTORE_FLASH_BASE + m_PerIndexAddr * 1024;
            //�����������ݵ�������
            pAllBspInit->pFlashBspStruct->Flash_Read(PerIndexAddrTemp, (u16 *)&TempBuf[0], m_NowPackSize / 2);

            //д���������ݵ�APP��
            PerIndexAddrTemp = APP_FLASH_BASE + m_PerIndexAddr * 1024;
            pAllBspInit->pWDGBspStruct->IWDG_Feed();
            pAllBspInit->pFlashBspStruct->Flash_Write(PerIndexAddrTemp, (u16 *)&TempBuf[0], m_NowPackSize / 2);
            pAllBspInit->pWDGBspStruct->IWDG_Feed();

            LOG("CpRestore sector:%d size:%d Addr:%d\r\n", m_PerIndexAddr, m_NowPackSize, PerIndexAddrTemp);


            if(m_PerIndexAddr >= (TotalPerIndex - 1))
            {
                m_PerIndexAddr = 0;
                pUserCfgSaveInit->UpdateCfgInf.NeedUpdateFlag = ALREADY_UPDATE_FLAG; //��־����ɳ������
                //���ø�����ɱ�־λ
                pAllBspInit->pFlashBspStruct->Flash_Write(EEPROM_FLASH_BASE, (u16 *)&UserCfgSaveStruct, Len[0]);
                pAllBspInit->pFlashBspStruct->Flash_Read(EEPROM_FLASH_BASE, (u16 *)&UserCfgSaveStruct, Len[0]);

                LOG("Copy Data finish��\r\n");

                break;
            }
            m_PerIndexAddr++;
            pAllBspInit->pWDGBspStruct->IWDG_Feed();
        }
        //�ر���ʾ
        CLR_STATUS_LED;

        pAllBspInit->pUartBspStruct->Uart4_init(ENABLE);
        crc_vel = CRC16Default(&Send_data[0], sizeof(Send_data) - 3);
        Send_data[sizeof(Send_data) - 3] = crc_vel >> 8;
        Send_data[sizeof(Send_data) - 2] = crc_vel;
        Sendtime = 3;
        delay_ms(500);
        while(Sendtime--)
        {
            pAllBspInit->pUartBspStruct->Uart4_send(&Send_data[0], sizeof(Send_data));
            PrintfStream(&Send_data[0], sizeof(Send_data));
            delay_ms(200);
            pAllBspInit->pWDGBspStruct->IWDG_Feed();

        }
        pAllBspInit->pUartBspStruct->Uart4_init(DISABLE);
        LOG("Restart 2��\r\n");
        delay_ms(800);
#if 1
        __set_PRIMASK(1);
        NVIC_SystemReset();
        while(1);//ֹͣι�����Ա����¸�λ*/
#endif
        //iap_load_app(APP_FLASH_BASE);

    }

    return 1;
}




