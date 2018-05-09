#ifndef __CANBSP_H__
#define __CANBSP_H__

//#include "BspCfg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "stm32f10x.h"

/*This macro define just for DEBUG*/
#define USE_CAN_MODELE_DEBUG 	1


#define BIT32_MASK_FILTER_NUMBER	14
#define BIT32_LIST_FILTER_NUMBER	28

#define BIT16_MASK_FILTER_NUMBER	28
#define BIT16_LIST_FILTER_NUMBER	56

#define CAN1_GPIO_PORT  	GPIOA
#define CAN1_GPIO_RX_PIN	GPIO_Pin_11
#define CAN1_GPIO_TX_PIN	GPIO_Pin_12

#define CAN2_GPIO_PORT 		GPIOB
#define CAN2_GPIO_RX_PIN	GPIO_Pin_12
#define CAN2_GPIO_TX_PIN  GPIO_Pin_13

#define CAN1_2_STB_GPIO_PORT 		GPIOA
#define CAN1_2_STB_GPIO_PIN			GPIO_Pin_8

#define ON_CAN_STB 		GPIO_ResetBits(CAN1_2_STB_GPIO_PORT,CAN1_2_STB_GPIO_PIN)
#define OFF_CAN_STB		GPIO_SetBits(CAN1_2_STB_GPIO_PORT,CAN1_2_STB_GPIO_PIN)

#define CAN_COM_LED_PORT  	 			GPIOC
#define CAN_COM_LED_PIN    				GPIO_Pin_9
#define CAN_COM_LED_PERIPH_CLK   	RCC_APB2Periph_GPIOC

#define SET_CAN_COM_LED 	GPIO_SetBits(CAN_COM_LED_PORT,CAN_COM_LED_PIN)
#define CLR_CAN_COM_LED 	GPIO_ResetBits(CAN_COM_LED_PORT,CAN_COM_LED_PIN)
#define REV_CAN_COM_LED 	GPIO_Reverse(CAN_COM_LED_PORT,CAN_COM_LED_PIN)

/*status management*/
#define CAN1_HEADER_CNT 1
#define CAN2_HEADER_CNT 2
#define HEADER_TIME     100

#define CONTENT_NOT_CONFIG_CNT 						2
#define CONTENT_DOING_CONFIG_CNT 					4
#define CONTENT_CONFIG_AND_COM_CNT 				6
#define CONTENT_CONFIG_BUT_NOTCOM_CNT 		8
#define CONTENT_TIME    500

typedef enum
{
    CONTENT_NOT_CONFIG = 0,
    CONTENT_DOING_CONFIG,
    CONTENT_CONFIG_AND_COM,
    CONTENT_CONFIG_BUT_NOTCOM,
}
STATUS_CONTENT_ENUM;

#define REGION_CNT 			1
#define REGION_TIME  		100


#define CAN_NO_MAX 2
typedef enum
{
    CAN_NO_1 = 0,
    CAN_NO_2,
} CAN_NO_ENUM;

typedef enum
{
    MASK_FILTER_MODE = 0,
    LIST_FILTER_MODE,
} FILER_MODE_ENUM;

typedef enum
{
    CAN_BAUD_1M = 0,
    CAN_BAUD_800K,
    CAN_BAUD_500K,
    CAN_BAUD_250K,
    CAN_BAUD_125K,
    CAN_BAUD_100K,
    CAN_BAUD_50K,
    CAN_BAUD_20K,
    CAN_BAUD_10K,
    CAN_BAUD_LAST,
} BAUD_SELECT_ENUM;

typedef enum
{
    STD_ID_FRAME = 0,
    EXD_ID_FRAME
} ID_FRAME_MODE_NUM;


typedef struct
{
    u32 Id;
    u8  Len;
    u8  Data[8];
} FrameBufferTypedef, *pFrameBufferTypedef;


__packed typedef struct
{
    CAN_NO_ENUM  			CAN_NO;
    FILER_MODE_ENUM		FilterMode;
    u32 							*pID;/*MASK MODE:BYTE1[ID],BYTE2[MASK]*/
    u32 							IDn;

    ID_FRAME_MODE_NUM	Mode;
}
FilterArrayTypedef, *pFilterArrayTypedef;

typedef enum
{
    READ_FRAME_FLAG = 0x01,
    WRITE_FRAME_FLAG = 0x02
} FRAME_INF_FLAG_ENUM;

__packed typedef struct
{
    CAN_NO_ENUM  			Fd;
    u8 								*pData;
    u32 							Len;

    u8 								Flag;/*@FRAME_INF_FLAG_ENUM*/
    u8								Index;/*FMI index*/
    u32               ID;
}
FrameInfTypedef, *pFrameInfTypedef;

__packed typedef struct
{
    FilterArrayTypedef *pFilterArrayInit;

    BAUD_SELECT_ENUM	Baud;
    STATUS_CONTENT_ENUM Content;

    u8 isStandby;

} CANPrvDataTypeDef, *pCANPrvDataTypeDef;


typedef enum
{
    SIO_NVIC_CONF = 0,			/*u16errupt configuration*/
    SIO_PORT_CONF,				/*port configuration*/
    SIO_LISTFLITER_CONF,	/*list fliter configuration*/
    SIO_BAUD_CONF,				/*baud configuration*/
    /*
    *please use "FrameInfTypedef" struct to replace "argstruct" argument in CAN_IOCtl function
    *
    */
    PROBE_FRAMEINF_STA,			/*probe frame status*/
} IOCTL_CMD_ENUM;



__packed typedef struct
{
    CANPrvDataTypeDef *pCANPrvDataInit[2];
    /*CAN Setting*/
    u8 (*CAN_SetStatusContent)(CAN_NO_ENUM Header, STATUS_CONTENT_ENUM Content);
    u8  	(*CAN_ModuleInit)(CAN_NO_ENUM CanNo, BAUD_SELECT_ENUM Baud);
    void 	(*CAN_NVIC_Setting)(CAN_NO_ENUM CanNo, FunctionalState NewState);
    u8 		(*CAN_ListFliter_Setting)(FilterArrayTypedef *pFilterArrayInit);
    u8 		(*CAN_Baud_Setting)(CAN_NO_ENUM CanNo);
    u8 		(*Suspend)(void);
    u8 		(*Resume)(void);
    /*Application u16erface*/
    u16 (*open)(const u8 *pathname, u16 flags);
    u16 (*read)(u16 fd, void *buf, u16 count);
    u16 (*write)(u16 fd, void *buf, u16 count);

    u16 (*ioctl)(u16 fd, u16 cmd, u8 *argstruct);
}
CANBspTypeDef;

CANBspTypeDef *GetCANBspData(void);
CANBspTypeDef *CAN_AllBspInit(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif /* __CANBSP_H__ */
