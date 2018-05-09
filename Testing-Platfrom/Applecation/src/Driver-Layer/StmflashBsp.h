#ifndef __STMFLASHBSP_H__
#define __STMFLASHBSP_H__

#include "sys.h"
#include "stm32f10x_flash.h"


#define STM32_FLASH_SIZE 256 	 				/*Flash size :n*1K*/

#define STM32_FLASH_WREN 1

#if STM32_FLASH_SIZE<128
#define STM_SECTOR_SIZE 1024      		/*Page of size*/
#else
#define STM_SECTOR_SIZE	2048
#endif

#define STM32_FLASH_BASE  0x08000000 	/*STM32 FLASH first of address*/

__packed typedef struct
{
    u8 isStandby;
} FlashPrvDataTypeDef, *pFlashPrvDataTypeDef;

__packed typedef struct
{
    void (*Flash_Write)(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite);
    void (*Flash_Read)(u32 ReadAddr, u16 *pBuffer, u16 NumToRead);
    pFlashPrvDataTypeDef pFlashPrvDataInit;
}
FlashBspTypeDef, *pFlashBspTypeDef;


FlashBspTypeDef *GetFlashBspData(void);
FlashBspTypeDef *Flash_AllBspInit(void);


#endif

















