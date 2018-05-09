#ifndef __RED_H
#define __RED_H

#include "BspCfg.h"

#define USE_IR_BSP_DEBUG   0


/*����״̬*/
#define KeyIsNotPressed         0X00   	/*����̧��*/
#define KeyIsShortPressed       0x01   	/*�����̰�*/
#define KeyIsLongPressed        0x02   	/*��������*/

#define LongPressedTime         30    		/*100*3ms ��������ʱ��*/
#define ShortPressedTime        10   		/*100*20  ȷ���ǳ���ʱ�� */
#define RDATA  									(GPIOB->IDR&GPIO_Pin_8) 	/*�������������*/

#define KEY_TYPE_IR		0
#define KEY_TYPE_TOUCH 	1

#define KEY_STATUS_IS_UP		0
#define KEY_STATUS_IS_DOWN 		1

#define IR_ID 		0X00
#define TOUCH_ID 	0X55

#define IR_ERR_VAL 0Xff

__packed typedef struct
{
    u8 RmtCnt;
    u16 KeyVal;
    u8 OverFlag;
    u8 ExitOnce;
} IRCheckTypedef, *pIRCheckTypedef;

typedef struct
{
    u32 KeyLastValue;
    u32 KeyPreValue;
    u8  KeyType;
    u16 KeyCntTime;
    u8 LongPressedEnable;
    u8 LongPressedOverFlag;
    u8 KeyUpdateFlag;
    u8 KeyInputSource;
    u8 KeyStatus;
} IRPrvDataTypedef;

__packed typedef struct
{
    void (*IRBsp_Init)(void);
    u8 (*IRKeyProcess)(void);
    u8 (*IR_Suspend)(void);
    IRPrvDataTypedef *pIRPrvDataInit;
}
IRBspTypeDef;

IRBspTypeDef *GetIRBspData(void);
IRBspTypeDef  *IR_AllBspInit(void);


#endif


