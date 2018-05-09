#ifndef __APPCONFIG_H
#define __APPCONFIG_H

#include "BSPCFG.h"

//�ں˰汾
#define HARDWARE_VERSION           0x01
#define BOOTLOADER_KERNEL_VERSION  0X03


//add-su
#define HARDWARE_VRDION_BYTE              0x04
#define BOOTLOADER_VERSION_BYTE           0x04                                /*Ԥ��1���ֽ�*/
//#define APP_VERSION_BYTE                  0x04
#define RESERVE_BYTE            					0xff


#define HARDWARE_VRDION_ADDR              (0X08000000+1024)
#define BOOTLOADER_VERSION_ADDR           (HARDWARE_VRDION_ADDR+HARDWARE_VRDION_BYTE)
//#define APP_VERSION_ADDR                  (BOOTLOADER_VERSION_ADDR+BOOTLOADER_VERSION_BYTE)


extern const u8 Hardware[HARDWARE_VRDION_BYTE] __attribute__((at(HARDWARE_VRDION_ADDR)));
extern const u8 BootloaderVer[BOOTLOADER_VERSION_BYTE] __attribute__((at(BOOTLOADER_VERSION_ADDR)));
//extern const u8 AppVertion[APP_VERSION_BYTE  ]  __attribute__((at(APP_VERSION_ADDR)));



#define RECOVER_FLASH_SIZE 40
#define DEFAULT_SET  0XBB

/* Bootloader section | Recover section | App section | Restore setcion |Eeprom section*/
/*256K- Bootloader section | Recover section | App section | Restore setcion |Eeprom section*/
#define BOOTLOADER_FLASH_BASE		  0X08000000														/*Size-10K-0X2800 */
#define RECOVER_FLASH_BASE 				BOOTLOADER_FLASH_BASE+0X2800					/*Size-40K-0XA000 */
#define APP_FLASH_BASE 						RECOVER_FLASH_BASE+0XA000							/*Size-85K-0X15400 */
#define RESTORE_FLASH_BASE 		 		APP_FLASH_BASE+0X15400										/*Size-85K-0X15400 */

#define EEPROM_FLASH_BASE 				RESTORE_FLASH_BASE+0X15400							/*Size-36K -0X9000*/
/*VIDEO(30K)+USERCFG(6K)*/
#define EEP_USERCFG_FLASH_BASE 	EEPROM_FLASH_BASE											/*Size-2K -0X800*/

#define EEP_CAN_FLASH_BASE 			EEP_USERCFG_FLASH_BASE+0X800					/*Size-10K-0X2800*/
#define EEP_VIDEO_FLASH_BASE 		EEP_CAN_FLASH_BASE+0X2800							/*Size-24K-0X7800*/
/*
*IRAM1:  0x20000000+0XC0   			192 Byte -Bootloader
*IRAM2: 0x200000C4+0x0EF40 	0x0FF40 Byte-App
*/

#define DEFAULT_SET  0XBB

typedef enum
{
    NEED_UPDATE_FLAG = 0X55,
    ALREADY_UPDATE_FLAG = 0XAA,
    DEFAULT_CFG = 0XFF
} UPDATE_FLAG_ENUM;

__packed typedef struct
{
    u8  DefaultCfg;          		/*Ĭ������ */
    u8  NeedUpdateFlag;		      	/*���±�־ 0XAA��ʾ�Ѿ����� 0X55����Ҫ���� ������������*/
    u32 PackTotalSize; 		      	/*�ܰ���С*/
    u16	CRCVal;						/*���/CRC16���ļ���У�� */
    u8 HardwareVersion[4];
    u8 BootloaderVerion[4];         /*Bootloader�汾�� */
    u8 APPVerion[4];

    u8 Reserved[8];

} UpdateCfgTypeDef;       		/*��������*/





__packed typedef struct
{
    /*ע��-����ֵ����֤�ṹΪ˫��*/
    u8 Reversed[10];


}
SocSpecialConfigTypeDef;



__packed typedef struct
{
    /*ע��-����ֵ����֤�ṹΪ˫��*/
    u8  DefaultCfg;          	/*Ĭ������ */
    u16 LowVolValAdc;	       	/*��ѹֵADC��Ϣ*/
    u8  LowVolValRef;       	/*��ѹֵ�ο�*/

    u8  G_SenserEnStatus;    	/*G-SENSERʹ��״̬  BIT0���ڲ��� BIT1:�ⲿ1 BIT2:�ⲿ2*/
    u8  IN_G_SenserLevel;    	/*����G-SENSER�ȼ�*/

    u8  Restart_Cnt;
    u8  Reserved[3];					/*???*/
    SocSpecialConfigTypeDef Config;

} AppConfigTypeDef;   /*ϵͳ����*/

__packed typedef struct
{
    UpdateCfgTypeDef 	UpdateCfgInf;
    AppConfigTypeDef 	AppConfigInf;
} UserCfgInfTypeDef;     /*�û��������ñ���*/



u8 AppCfgInit(void);
static u8 UserCfgRestore(AllBsp_Typedef *pAllBspInit);
u8 UpdateProcess(AllBsp_Typedef *pAllBspInit, UserCfgInfTypeDef *pUserCfgSaveInit);
extern  UserCfgInfTypeDef UserCfgSaveStruct;


#endif
