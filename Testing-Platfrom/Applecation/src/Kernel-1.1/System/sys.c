#include "sys.h"

static u8  fac_us = 0; /*us延时倍乘数*/
static u16 fac_ms = 0; /*ms延时倍乘数,在ucos下,代表每个节拍的ms数*/

static SystemPropertiesTypedef SysProperties;
SystemTimeTypedef SysTime;

void delay_init(u8 SYSCLK)
{
    SysTick->CTRL &= ~(1 << 2);	/*SYSTICK使用外部时钟源	 */
    fac_us = SYSCLK / 8;				/*不论是否使用ucos,fac_us都需要使用 8指8分频*/

    fac_ms = (u16)fac_us * 1000; /*非ucos下,代表每个ms需要的systick时钟数*/
}

void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD = nus * fac_us;
    SysTick->VAL = 0x00;
    SysTick->CTRL = 0x01 ;
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16)));
    SysTick->CTRL = 0x00;
    SysTick->VAL = 0X00;
}
/*nms:1-1864ms*/
void delay_ms(u16 nms)
{
    u32 temp;
    SysTick->LOAD = (u32)nms * fac_ms;
    SysTick->VAL = 0x00;
    SysTick->CTRL = 0x01 ;
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16)));
    SysTick->CTRL = 0x00;
    SysTick->VAL = 0X00;
}

/*JTAG模式设置,用于设置JTAG的模式
*mode:jtag,swd模式设置;00,全使能;01,使能SWD;10,全关闭;
*#define JTAG_SWD_DISABLE   0X02
*#define SWD_ENABLE         0X01
*#define JTAG_SWD_ENABLE    0X00
*/
void JTAG_Set(u8 mode)
{
    u32 temp;
    temp = mode;
    temp <<= 25;
    RCC->APB2ENR |= 1 << 0; //开启辅助时钟
    AFIO->MAPR &= 0XF8FFFFFF; //清除MAPR的[26:24]
    AFIO->MAPR |= temp;     //设置jtag模式
}

void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

__asm void WFI_SET(void)
{
    WFI;
}
/*关闭所有中断*/
__asm void INTX_DISABLE(void)
{
    CPSID I;
}
/*开启所有中断*/
__asm void INTX_ENABLE(void)
{
    CPSIE I;
}
/*设置栈顶地址
*addr:栈顶地址
*/
__asm void MSR_MSP(uint32_t addr)
{
    MSR MSP, r0
    BX r14
}

void SetSysProperties_Status(SYS_RUN_MODE_ENUM Value)
{
    SysProperties.Mode = Value;
}

SYS_RUN_MODE_ENUM GetSysProperties_Status(void)
{
    return SysProperties.Mode;
}

void SetSysProperties_SocID(SOC_ID_ENUM Value)
{
    SysProperties.SocID = Value;
}

SOC_ID_ENUM GetSysProperties_SocID(void)
{
    return SysProperties.SocID;
}

SystemPropertiesTypedef *GetSysPropertiesData(void)
{
    return &SysProperties;
}


void SetSystemTime(SystemTimeTypedef *pSystemTime)
{
    if(pSystemTime)
    {
        memcpy(&SysTime, pSystemTime, sizeof(SystemTimeTypedef));
    }
}

SystemTimeTypedef *GetSystemTime(void)
{
    return &SysTime;
}




