#ifndef __CRC_Check_H
#define __CRC_Check_H

#include "sys.h"

//u8 BCCVerify(u8 *buf,u32 len);
//u8 FileVerify(u32 FirAddr,u32 EndAddr);
//u8 SUMVerify(u8 *buf,u32 len);
u16 CRC16Default(u8 *pszBuf, u32 unLength);
#endif

