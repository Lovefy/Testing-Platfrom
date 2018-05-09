#include "sys.h"
#include "stm32f10x_can.h"

#include "misc.h"
#include <stdio.h>
#include <string.h>
#include "CanBsp.h"

//#include "BspCfg.h"

typedef struct
{
    u8  SJW;
    u8  BS1;
    u8  BS2;
    u16 Scale;
    u16 Interval;
} CanBaudTypeDef;

__packed typedef struct
{
    CanRxMsg 				*pCanPhyInfArray;
    CanBaudTypeDef 	*pCanBaudInit;

    u8 isStandby;
} CANProDataTypeDef, *pCANProDataTypeDef;

typedef enum
{
    STATUS_HEADER = 0X00,
    STATUS_CONTENT,
    STATUS_REGION,

}
STATUS_STEP_ENUM;

__packed typedef struct
{
    u8 Step;
    CAN_NO_ENUM Header;
    u8 ContentVal[2];
    u8 Cnt;
    u8 Cntcompare;

    u8 ComBreakCnt[2];
}
StatusMangement_Typedef;


/*protected class*/
static CANProDataTypeDef  CANProDataInit[2];
static CanRxMsg  					CanPhyInfArray[2];
static FrameBufferTypedef FrameBufferInit[2][56];/*[0][56]-CAN1 [1][56]-CAN2*/
static FrameInfTypedef		FrameInfInit[2];
static StatusMangement_Typedef StatusMangement;
static struct ElemType CANBspElem[2];


/*privated class*/
static CANBspTypeDef 			CANBspInit;
static CANPrvDataTypeDef  CANPrvDataInit[2];
static FilterArrayTypedef FilterArrayInit[2];

/* CAN	BaudRate = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler */
/*@enum BAUD_SELECT_ENUM*/
const CanBaudTypeDef CanBaudInit[9] =
{
    { CAN_SJW_1tq, CAN_BS1_8tq,  CAN_BS2_3tq,  3,    0x100 },  /*1M*/
    { CAN_SJW_1tq, CAN_BS1_11tq, CAN_BS2_3tq,  3,    0x120 },  /*800K*/
    { CAN_SJW_1tq, CAN_BS1_8tq, CAN_BS2_3tq,   6,    0x120 },  /*500K*/
    { CAN_SJW_1tq, CAN_BS1_13tq, CAN_BS2_2tq,  9,    0x200 },  /*250k*/
    { CAN_SJW_1tq, CAN_BS1_13tq, CAN_BS2_2tq,  18,   0x350 },  /*125k*/
    { CAN_SJW_1tq, CAN_BS1_14tq, CAN_BS2_3tq,  20,   0x400 },  /*100K*/
    { CAN_SJW_1tq, CAN_BS1_13tq, CAN_BS2_2tq,  45,   0x800 },  /*50k*/
    { CAN_SJW_1tq, CAN_BS1_14tq, CAN_BS2_3tq,  100,  0x1400},  /*20k*/
    { CAN_SJW_1tq, CAN_BS1_13tq, CAN_BS2_2tq,  225,  0x3000}   /*10k*/
};


static void CAN_Port_Setting(CAN_NO_ENUM CanNo)
{
    GPIO_InitTypeDef  			GPIO_InitStructure;
    CAN_TypeDef *CANx;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    if(CanNo == CAN_NO_1)
    {
        CANx = CAN1;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

        GPIO_InitStructure.GPIO_Pin = CAN1_GPIO_RX_PIN;

        if(CANPrvDataInit[CAN_NO_1].isStandby == ENABLE)
        {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        }
        else
        {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        }

        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(CAN1_GPIO_PORT, &GPIO_InitStructure);

        if(CANPrvDataInit[CAN_NO_1].isStandby == ENABLE)
        {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        }
        else
        {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        }

        GPIO_InitStructure.GPIO_Pin = CAN1_GPIO_TX_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(CAN1_GPIO_PORT, &GPIO_InitStructure);

    }
    else 	if(CanNo == CAN_NO_2)
    {
        CANx = CAN2;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

        GPIO_InitStructure.GPIO_Pin = CAN2_GPIO_RX_PIN;

        if(CANPrvDataInit[CAN_NO_2].isStandby == ENABLE)
        {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        }
        else
        {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        }

        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(CAN2_GPIO_PORT, &GPIO_InitStructure);
        /* CAN2 TX PB13 */
        GPIO_InitStructure.GPIO_Pin = CAN2_GPIO_TX_PIN;

        if(CANPrvDataInit[CAN_NO_2].isStandby == ENABLE)
        {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        }
        else
        {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        }

        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(CAN2_GPIO_PORT, &GPIO_InitStructure);
    }

}


static void CAN_STB_Setting(u8 StandbyMode)
{
    GPIO_InitTypeDef  			GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = CAN1_2_STB_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(CAN1_2_STB_GPIO_PORT, &GPIO_InitStructure);

    if(StandbyMode)
    {
        OFF_CAN_STB;
    }
    else
    {
        ON_CAN_STB;
    }
}

static u8 CAN_Baud_Setting(CAN_NO_ENUM CanNo)
{
    CAN_InitTypeDef CAN_InitStructure;
    CAN_TypeDef *CANx;


    CAN_StructInit(&CAN_InitStructure);

    if (CanNo == CAN_NO_1)
    {
        CANx = CAN1;
    }
    else
    {
        CANx = CAN2;
    }
    CAN_DeInit(CANx);

    CANProDataInit[CanNo].pCanBaudInit = (CanBaudTypeDef *)&CanBaudInit[CANPrvDataInit[CanNo].Baud];

    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = ENABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = ENABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;

    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    /*CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;*/

    CAN_InitStructure.CAN_SJW = CANProDataInit[CanNo].pCanBaudInit->SJW;
    CAN_InitStructure.CAN_BS1 = CANProDataInit[CanNo].pCanBaudInit->BS1;
    CAN_InitStructure.CAN_BS2 = CANProDataInit[CanNo].pCanBaudInit->BS2;
    CAN_InitStructure.CAN_Prescaler = CANProDataInit[CanNo].pCanBaudInit->Scale;

    return CAN_Init(CANx, &CAN_InitStructure);
}


static u8 FrameId2RegValue32 ( u32 Id, u8 Mode, u16 *pValH, u16 *pValL )
{

    u8 bRtn = FALSE;

    if ( pValH && pValL )
    {
        if (Mode)
        {
            Id = ( Id << 3 ) | 0x04;
        }
        else
        {
            Id = Id << 21;
        }

        *pValH = ( u16 ) ( Id >> 16 );
        *pValL = ( u16 ) Id;
        bRtn = TRUE;
    }

    return ( bRtn );

}

static u8 FrameId2RegValue16 ( u32 Id1, u32 Id2, u16 *pVal1, u16 *pVal2 )
{

    u8 bRtn = FALSE;

    if ( pVal1 && pVal2 )
    {
        Id1 = Id1 << 21;
        Id2 = Id2 << 21;

        *pVal1 = ( u16 ) ( Id1 >> 16 );
        *pVal2 = ( u16 ) ( Id2 >> 16 );

        bRtn = TRUE;
    }

    return ( bRtn );

}


static u8 CAN_ListFliter_Setting(FilterArrayTypedef *pFilterArrayInit)
{
    CAN_FilterInitTypeDef  	CAN_FilterStruct;
    u8 i = 0;
    u8 MaxFilter = 0;

    if(pFilterArrayInit->Mode == STD_ID_FRAME)
        MaxFilter = BIT16_LIST_FILTER_NUMBER;
    else if(pFilterArrayInit->Mode == EXD_ID_FRAME)
        MaxFilter = BIT32_LIST_FILTER_NUMBER;

    if(pFilterArrayInit->IDn > MaxFilter)
    {

        printf("IDn:%d\r\n", pFilterArrayInit->IDn);
        return FALSE;
    }


    if(pFilterArrayInit->CAN_NO == CAN_NO_1) /*filter SN:0-13*/
    {
        CAN_FilterStruct.CAN_FilterNumber = 0;
        CAN_FilterStruct.CAN_FilterFIFOAssignment = CAN_FIFO0;

        /*copy ID to FrameBufferInit.ID*/
        memset(&FrameBufferInit[0][0], 0, sizeof(FrameBufferTypedef)*MaxFilter);
        for(i = 0; i < pFilterArrayInit->IDn; i++)
            FrameBufferInit[0][i].Id = *(pFilterArrayInit->pID + i);
    }
    else if(pFilterArrayInit->CAN_NO == CAN_NO_2)
    {
        CAN_FilterStruct.CAN_FilterNumber = 14;	/*filter SN:14-27*/
        CAN_FilterStruct.CAN_FilterFIFOAssignment = CAN_FIFO1;

        memset(&FrameBufferInit[1][0], 0, sizeof(FrameBufferTypedef)*MaxFilter);
        for(i = 0; i < pFilterArrayInit->IDn; i++)
            FrameBufferInit[1][i].Id = *(pFilterArrayInit->pID + i);
    }

    CAN_FilterStruct.CAN_FilterMode = CAN_FilterMode_IdList;

    i = 0;
    while(i < pFilterArrayInit->IDn)
    {
        if(pFilterArrayInit->Mode == STD_ID_FRAME)
        {
            CAN_FilterStruct.CAN_FilterScale = CAN_FilterScale_16bit;

            FrameId2RegValue16(*(pFilterArrayInit->pID + i + 3), *(pFilterArrayInit->pID + i + 2), &CAN_FilterStruct.CAN_FilterMaskIdHigh, &CAN_FilterStruct.CAN_FilterIdHigh);
            FrameId2RegValue16(*(pFilterArrayInit->pID + i + 1), *(pFilterArrayInit->pID + i), &CAN_FilterStruct.CAN_FilterMaskIdLow, &CAN_FilterStruct.CAN_FilterIdLow  );
#if USE_CAN_MODELE_DEBUG
            printf("CAN:%d FilterN:%d-%x %x %x %x\r\n",
                   pFilterArrayInit->CAN_NO,
                   CAN_FilterStruct.CAN_FilterNumber,
                   *(pFilterArrayInit->pID + i),
                   *(pFilterArrayInit->pID + i + 1),
                   *(pFilterArrayInit->pID + i + 2),
                   *(pFilterArrayInit->pID + i + 3));
#endif
            i += 4;
        }
        else if(pFilterArrayInit->Mode == EXD_ID_FRAME)
        {
            CAN_FilterStruct.CAN_FilterScale = CAN_FilterScale_32bit;
            /*LIST 32bit模式下设置的滤波ID组为2，但是滤波器组的编号还是i*/

            FrameId2RegValue32(*(pFilterArrayInit->pID + i), pFilterArrayInit->Mode, &CAN_FilterStruct.CAN_FilterIdHigh, &CAN_FilterStruct.CAN_FilterIdLow);
            FrameId2RegValue32(*(pFilterArrayInit->pID + i + 1), pFilterArrayInit->Mode, &CAN_FilterStruct.CAN_FilterMaskIdHigh, &CAN_FilterStruct.CAN_FilterMaskIdLow);
#if USE_CAN_MODELE_DEBUG
            printf("CAN:%d FilterN:%d-%x %x\r\n",
                   pFilterArrayInit->CAN_NO,
                   CAN_FilterStruct.CAN_FilterNumber,
                   *(pFilterArrayInit->pID + i),
                   *(pFilterArrayInit->pID + i + 1));
#endif

            i += 2;
        }

        CAN_FilterStruct.CAN_FilterActivation = ENABLE;
        CAN_FilterInit(&CAN_FilterStruct);

        CAN_FilterStruct.CAN_FilterNumber++;
    }


    return 0;
}


static void CAN_NVIC_Setting(CAN_NO_ENUM CanNo, FunctionalState NewState)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    uint32_t IT_FmpNum;
    CAN_TypeDef *CANx;

    if(CanNo == CAN_NO_1)
    {
        CANx = CAN1;
        NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
        IT_FmpNum = CAN_IT_FMP0 | CAN_IT_BOF;
    }
    else if(CanNo == CAN_NO_2)
    {
        CANx = CAN2;
        NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
        IT_FmpNum = CAN_IT_FMP1 | CAN_IT_BOF;
    }
    else
        return ;

    NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;
    NVIC_Init(&NVIC_InitStructure);

    CAN_ITConfig(CANx, IT_FmpNum, NewState);
}




static u8 SetStatusContent(CAN_NO_ENUM Header, STATUS_CONTENT_ENUM Content)
{

    switch(Content)
    {
    case CONTENT_NOT_CONFIG:
        StatusMangement.ContentVal[Header] = CONTENT_NOT_CONFIG_CNT;
        break;
    case CONTENT_DOING_CONFIG:
        StatusMangement.ContentVal[Header] = CONTENT_DOING_CONFIG_CNT;
        break;
    case CONTENT_CONFIG_AND_COM:
        StatusMangement.ContentVal[Header] = CONTENT_CONFIG_AND_COM_CNT;
        break;
    case CONTENT_CONFIG_BUT_NOTCOM:
        StatusMangement.ContentVal[Header] = CONTENT_CONFIG_BUT_NOTCOM_CNT;
        break;
    default:
        StatusMangement.ContentVal[Header] = CONTENT_NOT_CONFIG_CNT;
        break;

    }
}


static u32 CAN_StatusManHandler(void *para)
{

    switch(StatusMangement.Step)
    {
    case STATUS_HEADER:
        if(CAN_NO_1 == StatusMangement.Header)
        {
            if(StatusMangement.Cnt >= StatusMangement.Cntcompare)
            {
                StatusMangement.Cnt = 0;
                StatusMangement.Cntcompare = StatusMangement.ContentVal[CAN_NO_1];
                StatusMangement.Step = STATUS_CONTENT;
                CANBspElem[0].expires = CONTENT_TIME;
                /*printf("skip1 to STATUS_CONTENT\r\n");*/
            }
            else
            {
                CANBspElem[0].expires = HEADER_TIME;
            }
        }
        else if(CAN_NO_2 == StatusMangement.Header)
        {
            if(StatusMangement.Cnt >= StatusMangement.Cntcompare)
            {
                StatusMangement.Cnt = 0;
                StatusMangement.Cntcompare = StatusMangement.ContentVal[CAN_NO_2];
                StatusMangement.Step = STATUS_CONTENT;
                CANBspElem[0].expires = CONTENT_TIME;
                /*printf("skip2 to STATUS_CONTENT\r\n");*/
            }
            else
            {
                CANBspElem[0].expires = HEADER_TIME;
            }

        }
        REV_CAN_COM_LED;
        StatusMangement.Cnt++;
        break;
    case STATUS_CONTENT:
        /*printf("Content1:%d Content2:%d Cnt:%d\r\n",StatusMangement.ContentVal[0],StatusMangement.ContentVal[1],StatusMangement.Cnt);*/
        if(CAN_NO_1 == StatusMangement.Header)
        {
            if(StatusMangement.Cnt >= StatusMangement.Cntcompare)
            {
                StatusMangement.Cnt = 0;
                StatusMangement.Cntcompare = REGION_CNT;
                StatusMangement.Step = STATUS_REGION;
                CANBspElem[0].expires = CONTENT_TIME;
                /*printf("skip1 to STATUS_REGION\r\n");*/
            }
            else
            {
                CANBspElem[0].expires = CONTENT_TIME;
                /*printf("STATUS1_CONTENT\r\n");*/
            }
        }
        else if(CAN_NO_2 == StatusMangement.Header)
        {
            if(StatusMangement.Cnt >= StatusMangement.Cntcompare)
            {
                StatusMangement.Cnt = 0;
                StatusMangement.Cntcompare = REGION_CNT;
                StatusMangement.Step = STATUS_REGION;
                CANBspElem[0].expires = CONTENT_TIME;
                /*printf("skip2 to STATUS_REGION\r\n");*/
            }
            else
            {
                CANBspElem[0].expires = CONTENT_TIME;
                /*printf("STATUS2_CONTENT\r\n");*/
            }
        }
        REV_CAN_COM_LED;
        StatusMangement.Cnt++;
        break;
    case STATUS_REGION:
        if(CAN_NO_1 == StatusMangement.Header)
        {
            StatusMangement.Cnt = 0;
            StatusMangement.Cntcompare = CAN2_HEADER_CNT;
            StatusMangement.Step = STATUS_HEADER;
            CANBspElem[0].expires = 2500;
            StatusMangement.Header = CAN_NO_2;
            /*printf("skip1 to STATUS_HEADER\r\n");*/
        }
        else if(CAN_NO_2 == StatusMangement.Header)
        {
            StatusMangement.Cnt = 0;
            StatusMangement.Cntcompare = CAN1_HEADER_CNT;
            StatusMangement.Step = STATUS_HEADER;
            CANBspElem[0].expires = 2500;
            StatusMangement.Header = CAN_NO_1;
            /*printf("skip2 to STATUS_HEADER\r\n");*/
        }
        CLR_CAN_COM_LED;

        break;
    default:
        CLR_CAN_COM_LED;
        CANBspElem[0].expires = 100;
        break;
    }

    Mod_timer(&CANBspElem[0], ENABLE);

    return 0;
}

#define COM_BREAK_TIME  5
static u32 CAN_Communication_Check_Handler(void *para)
{
    if(CONTENT_CONFIG_AND_COM == CANBspInit.pCANPrvDataInit[CAN_NO_1]->Content)
    {

        if(++StatusMangement.ComBreakCnt[CAN_NO_1] > COM_BREAK_TIME)
        {
            CANBspInit.pCANPrvDataInit[CAN_NO_1]->Content = CONTENT_CONFIG_BUT_NOTCOM;
            CANBspInit.CAN_SetStatusContent(CAN_NO_1, CANBspInit.pCANPrvDataInit[CAN_NO_1]->Content);

            StatusMangement.ComBreakCnt[CAN_NO_1] = 0;
        }
    }

    if(CONTENT_CONFIG_AND_COM == CANBspInit.pCANPrvDataInit[CAN_NO_2]->Content)
    {

        if(++StatusMangement.ComBreakCnt[CAN_NO_2] > COM_BREAK_TIME)
        {
            CANBspInit.pCANPrvDataInit[CAN_NO_2]->Content = CONTENT_CONFIG_BUT_NOTCOM;
            CANBspInit.CAN_SetStatusContent(CAN_NO_2, CANBspInit.pCANPrvDataInit[CAN_NO_2]->Content);

            StatusMangement.ComBreakCnt[CAN_NO_2] = 0;

        }
    }


    CANBspElem[1].expires = 1000; /*1s*/
    Mod_timer(&CANBspElem[1], ENABLE);

    return 0;
}

static void StatusMangementTmrInit(void)
{
    Init_timer(&CANBspElem[0]);
    strcpy((char *)&CANBspElem[0].Name, "CANStatusTmr");
    CANBspElem[0].expires = 100;
    CANBspElem[0].handler = CAN_StatusManHandler;
    Add_timer(&CANBspElem[0]);

    CANBspElem[0].expires = 100;
    Mod_timer(&CANBspElem[0], ENABLE);

    Init_timer(&CANBspElem[1]);
    strcpy((char *)&CANBspElem[1].Name, "CANComCheckTmr");
    CANBspElem[1].expires = 100;
    CANBspElem[1].handler = CAN_Communication_Check_Handler;
    Add_timer(&CANBspElem[1]);

    CANBspElem[1].expires = 100;
    Mod_timer(&CANBspElem[1], ENABLE);


}


static void StatusMangementPortInit(GPIOMode_TypeDef GPIOMode)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(CAN_COM_LED_PERIPH_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = CAN_COM_LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIOMode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CAN_COM_LED_PORT, &GPIO_InitStructure);

    CLR_CAN_COM_LED;
}


static u8 CAN_Suspend(void)
{
    CANPrvDataInit[CAN_NO_1].isStandby = ENABLE;
    CANPrvDataInit[CAN_NO_2].isStandby = ENABLE;

    CAN_STB_Setting(ENABLE);
    CAN_Port_Setting(CAN_NO_1);
    CAN_Port_Setting(CAN_NO_2);
    StatusMangementPortInit(GPIO_Mode_AIN);
#if (USE_CAN_MODELE_DEBUG)
    printf("CAN_Suspend\r\n");
#endif
    return 0;
}

static u8 CAN_Resume(void)
{
    CANPrvDataInit[CAN_NO_1].isStandby = DISABLE;
    CANPrvDataInit[CAN_NO_2].isStandby = DISABLE;

    CAN_STB_Setting(DISABLE);
    CAN_Port_Setting(CAN_NO_1);
    CAN_Port_Setting(CAN_NO_2);
    StatusMangementPortInit(GPIO_Mode_Out_PP);

#if (USE_CAN_MODELE_DEBUG)
    printf("CAN_Resume\r\n");
#endif

    return 0;
}



static void CAN1_DataHandler_Func(void)
{
    FrameBufferTypedef Frame;
    /*Receive and save data*/
    CAN_Receive(CAN1, CAN_FIFO0, &CanPhyInfArray[CAN_NO_1]);

    if(CanPhyInfArray[CAN_NO_1].RTR == CAN_RTR_Remote)
        return;

    if(CanPhyInfArray[CAN_NO_1].IDE == CAN_Id_Standard)
        Frame.Id = CanPhyInfArray[CAN_NO_1].StdId;
    else if(CanPhyInfArray[CAN_NO_1].IDE == CAN_Id_Extended)
        Frame.Id = CanPhyInfArray[CAN_NO_1].ExtId;

    Frame.Len = CanPhyInfArray[CAN_NO_1].DLC;
    memcpy(Frame.Data, CanPhyInfArray[CAN_NO_1].Data, Frame.Len);

    if(Frame.Id == FrameBufferInit[CAN_NO_1][CanPhyInfArray[CAN_NO_1].FMI].Id)
    {
        if(memcmp(Frame.Data, FrameBufferInit[CAN_NO_1][CanPhyInfArray[CAN_NO_1].FMI].Data, Frame.Len))
        {
            memcpy(&FrameBufferInit[CAN_NO_1][CanPhyInfArray[CAN_NO_1].FMI], &Frame, sizeof(FrameBufferTypedef));
#if (USE_CAN_MODELE_DEBUG)
            printf("CAN1 Data is changed!\r\n");
#endif
            /*Notify application can read the changed frame*/

            FrameInfInit[CAN_NO_1].pData = &FrameBufferInit[CAN_NO_1][CanPhyInfArray[CAN_NO_1].FMI].Data[0];
            FrameInfInit[CAN_NO_1].Len = FrameBufferInit[CAN_NO_1][CanPhyInfArray[CAN_NO_1].FMI].Len;
            FrameInfInit[CAN_NO_1].Flag |= READ_FRAME_FLAG;
            FrameInfInit[CAN_NO_1].Index = CanPhyInfArray[CAN_NO_1].FMI;
            FrameInfInit[CAN_NO_1].ID = Frame.Id;
            FrameInfInit[CAN_NO_2].Fd = CAN_NO_1;

        }

    }

#if (USE_CAN_MODELE_DEBUG)
    printf("CAN1 FMI:%x\r\n", CanPhyInfArray[CAN_NO_1].FMI);
    /* PrintfStream(&CanPhyInfArray[CAN_NO_1].Data[0],CanPhyInfArray[CAN_NO_1].DLC);*/
#endif
}

void CAN1_RX0_IRQHandler(void)
{
    if(CAN_GetITStatus(CAN1, CAN_IT_BOF))
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_BOF);

        CANBspInit.pCANPrvDataInit[CAN_NO_1]->Content = CONTENT_CONFIG_BUT_NOTCOM;
        CANBspInit.CAN_SetStatusContent(CAN_NO_1, CANBspInit.pCANPrvDataInit[CAN_NO_1]->Content);
        printf("CAN1_IT_BOF\r\n");
    }
    else
    {
        CAN1_DataHandler_Func();

        if(CONTENT_CONFIG_BUT_NOTCOM == CANBspInit.pCANPrvDataInit[CAN_NO_1]->Content)
        {
            CANBspInit.pCANPrvDataInit[CAN_NO_1]->Content = CONTENT_CONFIG_AND_COM;
            CANBspInit.CAN_SetStatusContent(CAN_NO_1, CANBspInit.pCANPrvDataInit[CAN_NO_1]->Content);
        }

        StatusMangement.ComBreakCnt[CAN_NO_1] = 0;
    }
}


static void CAN2_DataHandler_Func(void)
{
    FrameBufferTypedef Frame;
    /*Receive and save data*/
    CAN_Receive(CAN2, CAN_FIFO1, &CanPhyInfArray[CAN_NO_2]);

    if(CanPhyInfArray[CAN_NO_2].RTR == CAN_RTR_Remote)
        return;

    if(CanPhyInfArray[CAN_NO_2].IDE == CAN_Id_Standard)
        Frame.Id = CanPhyInfArray[CAN_NO_2].StdId;
    else if(CanPhyInfArray[CAN_NO_2].IDE == CAN_Id_Extended)
        Frame.Id = CanPhyInfArray[CAN_NO_2].ExtId;

    Frame.Len = CanPhyInfArray[CAN_NO_2].DLC;
    memcpy(Frame.Data, CanPhyInfArray[CAN_NO_2].Data, Frame.Len);

    if(Frame.Id == FrameBufferInit[CAN_NO_2][CanPhyInfArray[CAN_NO_2].FMI].Id)
    {
        if(memcmp(Frame.Data, FrameBufferInit[CAN_NO_2][CanPhyInfArray[CAN_NO_2].FMI].Data, Frame.Len))
        {
            memcpy(&FrameBufferInit[CAN_NO_2][CanPhyInfArray[CAN_NO_2].FMI], &Frame, sizeof(FrameBufferTypedef));

            /*Notify application can read the changed frame*/

            FrameInfInit[CAN_NO_2].pData = &FrameBufferInit[CAN_NO_2][CanPhyInfArray[CAN_NO_2].FMI].Data[0];
            FrameInfInit[CAN_NO_2].Len = FrameBufferInit[CAN_NO_2][CanPhyInfArray[CAN_NO_2].FMI].Len;
            FrameInfInit[CAN_NO_2].Flag |= READ_FRAME_FLAG;
            FrameInfInit[CAN_NO_2].Index = CanPhyInfArray[CAN_NO_2].FMI;
            FrameInfInit[CAN_NO_2].ID = Frame.Id;
            FrameInfInit[CAN_NO_2].Fd = CAN_NO_2;
#if (USE_CAN_MODELE_DEBUG)
            printf("CAN2 Data is changed:%d %d!\r\n", FrameInfInit[CAN_NO_2].Flag, FrameInfInit[CAN_NO_2].Len);
#endif
        }

    }
#if (USE_CAN_MODELE_DEBUG)
    printf("CAN2 FMI:%x\r\n", CanPhyInfArray[CAN_NO_2].FMI);
#endif
}


void CAN2_RX1_IRQHandler(void)
{
    if(CAN_GetITStatus(CAN2, CAN_IT_BOF))
    {
        CAN_ClearITPendingBit(CAN2, CAN_IT_BOF);
        CANBspInit.pCANPrvDataInit[CAN_NO_2]->Content = CONTENT_CONFIG_BUT_NOTCOM;
        CANBspInit.CAN_SetStatusContent(CAN_NO_2, CANBspInit.pCANPrvDataInit[CAN_NO_2]->Content);
        printf("CAN2_IT_BOF\r\n");
    }
    else
    {
        CAN2_DataHandler_Func();

        if(CONTENT_CONFIG_BUT_NOTCOM == CANBspInit.pCANPrvDataInit[CAN_NO_2]->Content)
        {
            CANBspInit.pCANPrvDataInit[CAN_NO_2]->Content = CONTENT_CONFIG_AND_COM;
            CANBspInit.CAN_SetStatusContent(CAN_NO_2, CANBspInit.pCANPrvDataInit[CAN_NO_2]->Content);
        }

        StatusMangement.ComBreakCnt[CAN_NO_2] = 0;

    }

}



/*cmd:@IOCTL_CMD_ENUM*/
static u16 CAN_IOCtl(u16 fd, u16 cmd, u8 *argstruct)
{
    u8 Res = 1;
    if(fd >= CAN_NO_MAX)
        return 1;
    if(!argstruct)
        return 1;

    switch(cmd)
    {
    case SIO_NVIC_CONF:


        break;
    case SIO_PORT_CONF:


        break;
    case SIO_LISTFLITER_CONF:


        break;
    case SIO_BAUD_CONF:


        break;
    case PROBE_FRAMEINF_STA:
        if(FrameInfInit[fd].Flag & READ_FRAME_FLAG)
        {
            memcpy(argstruct, &FrameInfInit[fd], sizeof(FrameInfTypedef));
            Res = 0;
        }
        else
        {
            Res = 1;
        }
        break;

    default:
        break;
    }

    return Res;
}

static u16 CAN_ReadOneFrame(u16 fd, void *buf, u16 count)
{
    u16 Res = 1;

    if(buf == NULL)
        return Res;

    if(FrameInfInit[fd].Flag == READ_FRAME_FLAG)
    {
        memcpy(buf, FrameInfInit[fd].pData, FrameInfInit[fd].Len);
        /*Retern the count of bytes in the last*/
        FrameInfInit[fd].Flag &= ~READ_FRAME_FLAG;
        Res = FrameInfInit[fd].Len;
    }
    else
    {
        Res = 1;
    }

    return Res;
}

static u16 CAN_WriteOneFrame(u16 fd, void *buf, u16 count)
{
    /*if(CanNo==CAN_NO_1)
    	CAN_Transmit(CAN1,TxMessage);
    else
    	CAN_Transmit(CAN2,TxMessage);*/
    return 0;
}


static u8 CAN_ModuleInit(CAN_NO_ENUM CanNo, BAUD_SELECT_ENUM Baud)
{
    CANPrvDataInit[CanNo].Baud = Baud;
    CAN_Port_Setting(CanNo);
    CAN_Baud_Setting(CanNo);
    CAN_NVIC_Setting(CanNo, ENABLE);
    CAN_STB_Setting(DISABLE);
    StatusMangementPortInit(GPIO_Mode_Out_PP);
    StatusMangementTmrInit();

    return 0;
}

CANBspTypeDef *GetCANBspData(void)
{
    return &CANBspInit;
}

CANBspTypeDef *CAN_AllBspInit(void)
{
    CANPrvDataInit[CAN_NO_1].pFilterArrayInit = &FilterArrayInit[CAN_NO_1];
    CANPrvDataInit[CAN_NO_2].pFilterArrayInit = &FilterArrayInit[CAN_NO_1];

    CANBspInit.pCANPrvDataInit[CAN_NO_1] = &CANPrvDataInit[CAN_NO_1];
    CANBspInit.pCANPrvDataInit[CAN_NO_2] = &CANPrvDataInit[CAN_NO_2];

    CANBspInit.CAN_SetStatusContent = SetStatusContent;
    CANBspInit.CAN_ModuleInit = CAN_ModuleInit;
    CANBspInit.CAN_Baud_Setting = CAN_Baud_Setting;
    CANBspInit.CAN_ListFliter_Setting = CAN_ListFliter_Setting;
    CANBspInit.CAN_NVIC_Setting = CAN_NVIC_Setting;
    CANBspInit.Resume = CAN_Resume;
    CANBspInit.Suspend = CAN_Suspend;


    CANBspInit.write = CAN_WriteOneFrame;
    CANBspInit.read = CAN_ReadOneFrame;
    CANBspInit.ioctl = CAN_IOCtl;

    return &CANBspInit;
}


