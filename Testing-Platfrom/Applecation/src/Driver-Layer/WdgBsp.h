#ifndef __WDGBSP_H
#define __WDGBSP_H

#include "sys.h"

__packed typedef struct
{
    void (*IWDG_Init)(u8 prer, u16 rlr);
    void (*IWDG_Feed)(void);
} WDGBspTypedef;


WDGBspTypedef *WDG_AllBspInit(void);


#endif




























