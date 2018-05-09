#ifndef __BSPCFG_H
#define __BSPCFG_H
#include "sys.h"
#include "ledBSP.h"
#include "UsartBsp.h"
#include "IRBSP.h"
#include "StmflashBsp.h"
#include "WdgBsp.h"
#include "OutInBsp.h"
#include "timerledbsp.h"
#include "IoBsp.h"


/*硬件底层调试信息*/
#define USE_BSP_CFG_DEBUG  0  /*使用硬件配置调试*/
/*G-SENSER setting*/


typedef		u8 (*ExitStandbyCallBack)(void);
typedef		u8 (*EnterStandbyCallBack)(void);

__packed typedef struct
{

    IRBspTypeDef    				*pIRBspStruct;
    TimeLedBpsTypeDef       *pTimerLedBspStruct;
    OutInBspTypeDef 				*pOutInBspStruct;
    FlashBspTypeDef 				*pFlashBspStruct;
    WDGBspTypedef 					*pWDGBspStruct;
    UartBspTypeDef          *pUartBspStruct;

}
AllBsp_Typedef;


u8 BSP_Init(void);
AllBsp_Typedef *GetAllBspPrvData(void);
extern AllBsp_Typedef AllBspStruct;

#endif
