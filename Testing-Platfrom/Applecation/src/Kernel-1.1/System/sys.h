#ifndef __SYS_H
#define __SYS_H
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "system_stm32f10x.h"
#include "string.h"
#include "usart.h"
#include "TimerList.h"



#define USE_DEBUG_PRINTF_DEBUG 2


typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */


typedef enum {FALSE = 0, TRUE = !FALSE} bool;
typedef enum {STANDBY_MODE = 0, NORMAL_MODE = !FALSE} DeviceModeEnum;

#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00


typedef enum
{
    START_UP_MODE 			= 0X00,
    RUN_MODE							= 0X01,
    PRE_STANDY_MODE 		= 0X02,
    STANDY_MODE 					= 0X03,
} SYS_RUN_MODE_ENUM;

typedef enum
{
    NXP_HARD_ID = 1,
    V18_HARD_ID,
    V40_HARD_ID,
} SOC_ID_ENUM;


typedef struct
{
    u32 Year;
    u8 Month;
    u8 Day;

    u8 Hour;
    u8 Minute;
    u8 Second;

}
SystemTimeTypedef;


typedef struct
{

    SYS_RUN_MODE_ENUM  	Mode;     			/*system runing mode*/
    SOC_ID_ENUM  				SocID;

}
SystemPropertiesTypedef;

extern SystemTimeTypedef SysTime;

#if USE_DEBUG_PRINTF_DEBUG==1
#define LOG(format,...)     fprintf(stdout,format,##__VA_ARGS__)
#define BSP_PrintfStream    PrintfStream
#elif USE_DEBUG_PRINTF_DEBUG==2
#define LOG(format,...)     printf(format,##__VA_ARGS__)
#define BSP_PrintfStream    PrintfStream
#elif USE_DEBUG_PRINTF_DEBUG==3
#define LOG 	            	printf
#define BSP_PrintfStream    PrintfStream
#else
#define LOG(format,...)
#define BSP_PrintfStream(...)
#endif


/*以下为汇编函数*/
void WFI_SET(void);		/*执行WFI指令*/
void INTX_DISABLE(void);/*关闭所有中断*/
void INTX_ENABLE(void);	/*开启所有中断*/
void MSR_MSP(uint32_t addr);	/*设置堆栈地址*/

void delay_init(u8 SYSCLK);
void delay_us(u32 nus);
void delay_ms(u16 nms);
void JTAG_Set(u8 mode);
void NVIC_Configuration(void);

extern void SetSysProperties_Status(SYS_RUN_MODE_ENUM Value);
extern SYS_RUN_MODE_ENUM GetSysProperties_Status(void);
extern void SetSysProperties_SocID(SOC_ID_ENUM Value);
extern SOC_ID_ENUM GetSysProperties_SocID(void);
extern SystemPropertiesTypedef *GetSysPropertiesData(void);
extern void SetSystemTime(SystemTimeTypedef *pSystemTime);
extern SystemTimeTypedef *GetSystemTime(void);

#endif
