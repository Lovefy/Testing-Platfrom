#include "sys.h"

static u8  fac_us = 0; /*us��ʱ������*/
static u16 fac_ms = 0; /*ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��*/

static SystemPropertiesTypedef SysProperties;
SystemTimeTypedef SysTime;

void delay_init(u8 SYSCLK)
{
    SysTick->CTRL &= ~(1 << 2);	/*SYSTICKʹ���ⲿʱ��Դ	 */
    fac_us = SYSCLK / 8;				/*�����Ƿ�ʹ��ucos,fac_us����Ҫʹ�� 8ָ8��Ƶ*/

    fac_ms = (u16)fac_us * 1000; /*��ucos��,����ÿ��ms��Ҫ��systickʱ����*/
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

/*JTAGģʽ����,��������JTAG��ģʽ
*mode:jtag,swdģʽ����;00,ȫʹ��;01,ʹ��SWD;10,ȫ�ر�;
*#define JTAG_SWD_DISABLE   0X02
*#define SWD_ENABLE         0X01
*#define JTAG_SWD_ENABLE    0X00
*/
void JTAG_Set(u8 mode)
{
    u32 temp;
    temp = mode;
    temp <<= 25;
    RCC->APB2ENR |= 1 << 0; //��������ʱ��
    AFIO->MAPR &= 0XF8FFFFFF; //���MAPR��[26:24]
    AFIO->MAPR |= temp;     //����jtagģʽ
}

void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

__asm void WFI_SET(void)
{
    WFI;
}
/*�ر������ж�*/
__asm void INTX_DISABLE(void)
{
    CPSID I;
}
/*���������ж�*/
__asm void INTX_ENABLE(void)
{
    CPSIE I;
}
/*����ջ����ַ
*addr:ջ����ַ
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




