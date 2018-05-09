#ifndef __APPCONFIG_H
#define __APPCONFIG_H

#include "BSPCFG.h"

//内核版本
#define HARDWARE_VERSION           0x01
#define BOOTLOADER_KERNEL_VERSION  0X03


//add-su
#define HARDWARE_VRDION_BYTE              0x04
#define BOOTLOADER_VERSION_BYTE           0x04                                /*预留1个字节*/
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
    u8  DefaultCfg;          		/*默认配置 */
    u8  NeedUpdateFlag;		      	/*更新标志 0XAA表示已经更新 0X55：需要更新 其他：不更新*/
    u32 PackTotalSize; 		      	/*总包大小*/
    u16	CRCVal;						/*异或/CRC16包文件总校验 */
    u8 HardwareVersion[4];
    u8 BootloaderVerion[4];         /*Bootloader版本号 */
    u8 APPVerion[4];

    u8 Reserved[8];

} UpdateCfgTypeDef;       		/*升级配置*/





__packed typedef struct
{
    /*注意-保留值，保证结构为双数*/
    u8 Reversed[10];


}
SocSpecialConfigTypeDef;



__packed typedef struct
{
    /*注意-保留值，保证结构为双数*/
    u8  DefaultCfg;          	/*默认配置 */
    u16 LowVolValAdc;	       	/*低压值ADC信息*/
    u8  LowVolValRef;       	/*低压值参考*/

    u8  G_SenserEnStatus;    	/*G-SENSER使能状态  BIT0：内部的 BIT1:外部1 BIT2:外部2*/
    u8  IN_G_SenserLevel;    	/*内置G-SENSER等级*/

    u8  Restart_Cnt;
    u8  Reserved[3];					/*???*/
    SocSpecialConfigTypeDef Config;

} AppConfigTypeDef;   /*系统设置*/

__packed typedef struct
{
    UpdateCfgTypeDef 	UpdateCfgInf;
    AppConfigTypeDef 	AppConfigInf;
} UserCfgInfTypeDef;     /*用户配置设置保存*/



u8 AppCfgInit(void);
static u8 UserCfgRestore(AllBsp_Typedef *pAllBspInit);
u8 UpdateProcess(AllBsp_Typedef *pAllBspInit, UserCfgInfTypeDef *pUserCfgSaveInit);
extern  UserCfgInfTypeDef UserCfgSaveStruct;


#endif
