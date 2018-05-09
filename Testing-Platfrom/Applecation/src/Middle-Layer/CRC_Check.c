#include "CRC_Check.h"
#include "StmflashBsp.h"
#if 0
u8 BCCVerify(u8 *buf, u32 len)
{
    u8 s = 0;
    u32 i;
    for (i = 0; i < len; i++)
    {
        s = s ^ (*(buf + i));
    }

    return s;
}

u8 SUMVerify(u8 *buf, u32 len)
{
    u8 s = 0;
    u32 i;
    for (i = 0; i < len; i++)
    {
        s = s + (*(buf + i));
    }

    return s;
}

static u8 FLASH_ReadByte(u32 faddr)
{
    return *(__IO u8 *)faddr;
}

u8 FileVerify(u32 FirAddr, u32 EndAddr)
{
    u8 s = 0;
    u32 i, Len;
    Len = EndAddr - FirAddr;
    for (i = 0; i < Len; i++)
    {
        s = s ^ (FLASH_ReadByte(FirAddr + i));
    }
    //LOG("i:%d Addr:0x%x\r\n",i,FirAddr+i);
    return s;
}

#endif
u16 CRC16Default(u8 *pszBuf, u32 unLength)
{
    u32 i, j;
    u16 CRCReg = 0xFFFF;
    u16 CurVal;

    for (i = 0; i < unLength; i++)
    {
        CurVal = pszBuf[i] << 8;

        for (j = 0; j < 8; j++)
        {
            if ((short)(CRCReg ^ CurVal) < 0)
                CRCReg = (CRCReg << 1) ^ 0x8005;
            else
                CRCReg <<= 1;
            CurVal <<= 1;
        }
    }

    return CRCReg;
}
