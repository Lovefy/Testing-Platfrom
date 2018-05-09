#include "BspCfg.h"

#include "malloc.h"
#include "QueueList.h"

#include "LedManModule.h"

static LedStutusManTypedef LedStutusManClass;
static struct ElemType LedStatusTmr;


static u32 LedStatusHandlerCallback(void *para)
{
    if(LED_IS_OPENED == LedStutusManClass.LedDisplayStep)
    {
        GetLedBspData()->Reset();

        LedStatusTmr.expires = LedStutusManClass.LedDisplayData.NegativePeriod;
        Mod_timer(&LedStatusTmr, ENABLE);

        LedStutusManClass.LedDisplayStep = LED_IS_WAIT_CLOSE;
#if	 USE_LED_MANMODULE_DEBUG==1
        printf("NegativePeriod:%d\r\n", LedStutusManClass.LedDisplayData.NegativePeriod);
#endif
    }
    else if(LED_IS_WAIT_CLOSE == LedStutusManClass.LedDisplayStep)
    {
        LedStutusManClass.LedDisplayStep = LED_IS_CLOSE;
#if	 USE_LED_MANMODULE_DEBUG==1
        printf("Led is closed\r\n");
#endif
    }

    return 0;
}


void LedManModuleRunningEntry(void)
{
    u8 Res = 0;
    LedDisplayDataTypedef *pLedDisplayData;
    u8 Len = sizeof(LedDisplayDataTypedef);
    u8 Size = 0;

    if(LED_IS_CLOSE == LedStutusManClass.LedDisplayStep)
    {
        pLedDisplayData = mymalloc(SRAMIN, Len);

        Res = DeLinkQueue(LedStutusManClass.pStatusMsgQ, (u8 *)pLedDisplayData, &Size);
        if((pLedDisplayData != 0) && (Res == SUCCESS) && (Size > 0))
        {
            memcpy(&LedStutusManClass.LedDisplayData, pLedDisplayData, Len);
            /*Set Led blink timer in positive stauts*/
            GetLedBspData()->Set();
            LedStatusTmr.expires = LedStutusManClass.LedDisplayData.PositivePeriod;
            Mod_timer(&LedStatusTmr, ENABLE);

            LedStutusManClass.LedDisplayStep = LED_IS_OPENED;
#if	 USE_LED_MANMODULE_DEBUG==1
            printf("PositivePeriod:%d\r\n", LedStutusManClass.LedDisplayData.PositivePeriod);
#endif
        }

        if(pLedDisplayData)
            myfree(SRAMIN, pLedDisplayData);
    }
}

u8  SetLedStatusToQueue(LedDisplayDataTypedef *pLedDisplayData)
{
    return EnLinkQueue(LedStutusManClass.pStatusMsgQ, (u8 *)pLedDisplayData, sizeof(LedDisplayDataTypedef));
}


LedStutusManTypedef *InitLedManModule(void)
{
    memset(&LedStutusManClass, 0, sizeof(LedStutusManClass));

    LedStutusManClass.pStatusMsgQ = (struct quefr *)mymalloc(SRAMIN, sizeof(struct quefr));
    CreatLinkQueue(LedStutusManClass.pStatusMsgQ);

    /*Init  timer for the led management module*/
    Init_timer(&LedStatusTmr);
    strcpy((char *)&LedStatusTmr.Name, "LedStatusTmr");
    LedStatusTmr.expires = 100;
    /*	LedStatusTmr.parg*/
    LedStatusTmr.handler = LedStatusHandlerCallback;
    Add_timer(&LedStatusTmr);

    return &LedStutusManClass;
}

