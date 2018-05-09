#include "BspCfg.h"

static WDGBspTypedef WDG_BspInit;

static void IWDG_Init(u8 prer, u16 rlr)
{
    IWDG->KR = 0X5555;
    IWDG->PR = prer;
    IWDG->RLR = rlr;
    IWDG->KR = 0XAAAA;
    IWDG->KR = 0XCCCC;
}
static void IWDG_Feed(void)
{
    IWDG->KR = 0XAAAA;
}

WDGBspTypedef *GetWDGBspData(void)
{
    return &WDG_BspInit;
}


WDGBspTypedef *WDG_AllBspInit(void)
{
    WDG_BspInit.IWDG_Init = IWDG_Init;
    WDG_BspInit.IWDG_Feed = IWDG_Feed;

     IWDG_Init(6, 1600);
    return &WDG_BspInit;
}


