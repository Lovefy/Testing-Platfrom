#include "BspCfg.h"

static FlashBspTypeDef 				FlashBspInit;
static FlashPrvDataTypeDef		FlashPrvDataInit;

static void InFlash_Read(u32 ReadAddr, u16 *pBuffer, u16 NumToRead);

//static u8 Flash_ReadByte(u32 faddr)
//{
//	return *(__IO u8 *)faddr;
//}

static u16 Flash_ReadHalfWord(u32 faddr)
{
    return *(__IO u16 *)faddr;
}


#if STM32_FLASH_WREN
static u16 STMFLASH_BUF[STM_SECTOR_SIZE / 2];

static void Flash_Write_NoCheck(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite)
{
    u16 i, res = 0;
#ifdef USE_ST_FLASH_DEBUG
    LOG("Write start\r\n");
#endif
    for(i = 0; i < NumToWrite; i++)
    {
        res = FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
#ifdef USE_ST_FLASH_DEBUG
        if(res != 4)
            LOG("EA:%d\r\n", WriteAddr);
#else
        res = res; /*防止编译器警告*/
#endif
        WriteAddr += 2;

    }
#ifdef USE_ST_FLASH_DEBUG
    LOG("Write end\r\n");
#endif
}

static void InFlash_Write(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite)
{
    u32 secpos;
    u16 secoff;
    u16 secremain;
    u16 i;
    u32 offaddr;
    if(WriteAddr < STM32_FLASH_BASE || (WriteAddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))return;
    FLASH_ClearFlag(FLASH_FLAG_OPTERR | FLASH_FLAG_WRPRTERR | FLASH_FLAG_PGERR | FLASH_FLAG_EOP | FLASH_FLAG_BSY);
    FLASH_Unlock();
    offaddr = WriteAddr - STM32_FLASH_BASE;
    secpos = offaddr / STM_SECTOR_SIZE;
    secoff = (offaddr % STM_SECTOR_SIZE) / 2;
    secremain = STM_SECTOR_SIZE / 2 - secoff;
    if(NumToWrite <= secremain)secremain = NumToWrite;
#ifdef USE_ST_FLASH_DEBUG
    LOG("Data1:%d %d %d %d %d %d\r\n", secpos, secoff, *pBuffer, WriteAddr, NumToWrite, secremain);
#endif
    while(1)
    {
        InFlash_Read(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE / 2);
        for(i = 0; i < secremain; i++)
        {
            if(STMFLASH_BUF[secoff + i] != 0XFFFF)break;
        }
#ifdef USE_ST_FLASH_DEBUG
        LOG("i:%d \r\n", i);
#endif
        if(i < secremain)
        {
            FLASH_ErasePage(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE);
            for(i = 0; i < secremain; i++)
            {
                STMFLASH_BUF[i + secoff] = pBuffer[i];
            }
            Flash_Write_NoCheck(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE / 2);
#ifdef USE_ST_FLASH_DEBUG
            LOG("擦除部分扇区完成\r\n");
#endif
        }
        else
        {
            Flash_Write_NoCheck(WriteAddr, pBuffer, secremain);
#ifdef USE_ST_FLASH_DEBUG
            LOG("擦除整个扇区完成\r\n");
#endif
        }
        if(NumToWrite == secremain)break;
        else
        {
            secpos++;
            secoff = 0;
            pBuffer += secremain;
            WriteAddr += secremain * 2;
            NumToWrite -= secremain;
            if(NumToWrite > (STM_SECTOR_SIZE / 2))secremain = STM_SECTOR_SIZE / 2;
            else secremain = NumToWrite;
#ifdef USE_ST_FLASH_DEBUG
            LOG("Data2:%d %d %d %d %d %d\r\n", secpos, secoff, *pBuffer, WriteAddr, NumToWrite, secremain);
#endif
        }
    };
    FLASH_Lock();
}

#endif

static void InFlash_Read(u32 ReadAddr, u16 *pBuffer, u16 NumToRead)
{
    u16 i;

    for(i = 0; i < NumToRead; i++)
    {
        pBuffer[i] = Flash_ReadHalfWord(ReadAddr);
        ReadAddr += 2;
    }

}

FlashBspTypeDef *GetFlashBspData(void)
{
    return &FlashBspInit;
}


FlashBspTypeDef *Flash_AllBspInit(void)
{
    FlashBspInit.pFlashPrvDataInit = &FlashPrvDataInit;
    FlashBspInit.Flash_Read = InFlash_Read;
    FlashBspInit.Flash_Write = InFlash_Write;


    /*48-72MHZ*/
    /*FLASH_SetLatency(FLASH_Latency_2);*/

    return &FlashBspInit;
}














