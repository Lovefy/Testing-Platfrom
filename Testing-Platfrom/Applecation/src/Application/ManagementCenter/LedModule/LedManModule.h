#ifndef __LEDMANMODULE_H_
#define __LEDMANMODULE_H_

#define USE_LED_MANMODULE_DEBUG 1


__packed typedef enum
{
    LED_IS_CLOSE = 0,
    LED_IS_WAIT_CLOSE,
    LED_IS_OPENED,
}
LED_STATUS_STEP_ENUM;


__packed typedef struct
{
    u16 PositivePeriod; /*positive time,max 65.536ms*/
    u16 NegativePeriod;/*positive time,max 65.536ms*/
}
LedDisplayDataTypedef;

__packed typedef struct
{
    struct quefr  *pStatusMsgQ;
    LedDisplayDataTypedef LedDisplayData;
    LED_STATUS_STEP_ENUM LedDisplayStep;
}
LedStutusManTypedef;

void LedManModuleRunningEntry(void);
u8  SetLedStatusToQueue(LedDisplayDataTypedef *pLedDisplayData);
LedStutusManTypedef *InitLedManModule(void);



#endif

