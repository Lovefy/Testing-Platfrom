#include "BspCfg.h"
#include "Malloc.h"

static RtcBspTypeDef 		 RtcBspInit;
static RtcPrvDataTypeDef RtcPrvDataInit;
static RTC_AlarmTypeDef  RTC_AlarmInit;

static RTC_TimeTypeDef RTC_TimeStructure;
static RTC_DateTypeDef RTC_DateStructure;


u8 Is_Leap_Year(u16 year)
{
    if(year % 4 == 0)
    {
        if(year % 100 == 0)
        {
            if(year % 400 == 0)return 1;
            else return 0;
        }
        else return 1;
    }
    else return 0;
}


u8 const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5}; /*月修正数据表 */
/*平年的月份日期表*/
const u8 mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static u8 RTC_Get_Week(u16 year, u8 month, u8 day)
{
    u16 temp2;
    u8 yearH, yearL;
    yearH = year / 100;
    yearL = year % 100;
    if (yearH > 19)yearL += 100;
    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + table_week[month - 1];
    if (yearL % 4 == 0 && month < 3)temp2--;
    return(temp2 % 7);
}

static void ReadAllRTCData(RTC_TimeTypeDef *pRTC_TimeInit, RTC_DateTypeDef *pRTC_DateInit)
{
    static u16 daycnt = 0;
    u32 timecount = 0;
    u32 temp = 0;
    u16 temp1 = 0;

    timecount = RTC->CNTH;
    timecount <<= 16;
    timecount += RTC->CNTL;

    temp = timecount / 86400;
    if(daycnt != temp)
    {
        daycnt = temp;
        temp1 = 1970;
        while(temp >= 365)
        {
            if(Is_Leap_Year(temp1))
            {
                if(temp >= 366)temp -= 366;
                else
                {
                    temp1++;
                    break;
                }
            }
            else temp -= 365;
            temp1++;
        }
        pRTC_DateInit->RTC_Year = temp1;

        temp1 = 0;
        while(temp >= 28)
        {
            if(Is_Leap_Year(pRTC_DateInit->RTC_Year) && temp1 == 1)
            {
                if(temp >= 29)temp -= 29;
                else break;
            }
            else
            {
                if(temp >= mon_table[temp1])temp -= mon_table[temp1];
                else break;
            }
            temp1++;
        }
        pRTC_DateInit->RTC_Month = temp1 + 1;
        pRTC_DateInit->RTC_Date = temp + 1;
    }
    temp = timecount % 86400;
    pRTC_TimeInit->RTC_Hours = temp / 3600;
    pRTC_TimeInit->RTC_Minutes = (temp % 3600) / 60;
    pRTC_TimeInit->RTC_Seconds = (temp % 3600) % 60;
    pRTC_DateInit->RTC_WeekDay = RTC_Get_Week(pRTC_DateInit->RTC_Year, pRTC_DateInit->RTC_Month, pRTC_DateInit->RTC_Date);

}


static u8 WriteAllRTCData(RTC_TimeTypeDef *pRTC_TimeInit, RTC_DateTypeDef *pRTC_DateInit)
{
    u16 t;
    u32 seccount = 0;
    u16 tYear = pRTC_DateInit->RTC_Year + 0;
    u8  tmonth = pRTC_DateInit->RTC_Month - 1; /*当前月前面的月份*/

    if(tYear < 1970 || tYear > 2099)
        return 1;

    for(t = 1970; t < tYear; t++) /*把所有年份的秒钟相加*/
    {
        if(Is_Leap_Year(t))seccount += 31622400; /*闰年的秒钟数*/
        else seccount += 31536000; /*平年的秒钟数*/
    }

    for(t = 0; t < tmonth; t++) /*把前面月份的秒钟数相加*/
    {
        seccount += (u32)mon_table[t] * 86400; /*月份秒钟数相加*/
        if(Is_Leap_Year(tYear) && t == 1)seccount += 86400; /*闰年 2 月份增加一天的秒钟数*/
    }
    seccount += (u32)(pRTC_DateInit->RTC_Date - 1) * 86400; /*把前面日期的秒钟数相加*/
    seccount += (u32)pRTC_TimeInit->RTC_Hours * 3600; /*小时秒钟数*/
    seccount += (u32)pRTC_TimeInit->RTC_Minutes * 60; /*分钟秒钟数*/
    seccount += pRTC_TimeInit->RTC_Seconds; /*最后的秒钟加上去*/

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); /*使能 PWR 和 BKP 外设时钟*/
    PWR_BackupAccessCmd(ENABLE); /*使能 RTC 和后备寄存器访问*/
    RTC_SetCounter(seccount); /*设置 RTC 计数器的值*/
    RTC_WaitForLastTask(); /*等待最近一次对 RTC 寄存器的写操作完成*/


    return 0;
}

static void SetAlarmStatus(FunctionalState Status)
{
    RTC_ClearITPendingBit(RTC_IT_ALR);
    /* Enable the RTC Alarm A interrupt */
    RTC_ITConfig(RTC_IT_ALR, Status);
}


static void SetAlarmTime(void)
{
    RTC_SetAlarm(RTC_GetCounter() + 5); /*5S wake up to feed WDG*/
    RTC_WaitForLastTask();
}


static u8  RTC_WakeUp_Config(FunctionalState NewStatus)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    RTC_ITConfig(RTC_FLAG_ALR, NewStatus);
    RTC_ClearITPendingBit(RTC_IT_ALR);
    RTC_ITConfig(RTC_FLAG_SEC, !NewStatus);
    RTC_ClearITPendingBit(RTC_IT_SEC);
    RTC_WaitForLastTask();

    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = NewStatus;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = NewStatus;
    NVIC_Init(&NVIC_InitStructure);

    return 0;
}

static u8  RTC_Timer_Config(void)
{
    u8 temp = 0;
    NVIC_InitTypeDef NVIC_InitStructure;

    if (BKP_ReadBackupRegister(BKP_DR42) != 0x5050)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
        PWR_BackupAccessCmd(ENABLE);
        BKP_DeInit();
        RCC_LSEConfig(RCC_LSE_ON);
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
            temp++;
            delay_ms(10);
            if(temp >= 250)
                return 1;
        }
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        RCC_RTCCLKCmd(ENABLE);
        RTC_WaitForLastTask();
        RTC_WaitForSynchro();

        RTC_ITConfig(RTC_IT_SEC, ENABLE);

        RTC_WaitForLastTask();
        RTC_EnterConfigMode();
        RTC_SetPrescaler(32767);
        RTC_WaitForLastTask();

        RTC_DateStructure.RTC_Year = 2016;
        RTC_DateStructure.RTC_Month = 1;
        RTC_DateStructure.RTC_Date = 1;
        RTC_DateStructure.RTC_WeekDay = RTC_Get_Week(RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date);
        RTC_TimeStructure.RTC_Hours = 0;
        RTC_TimeStructure.RTC_Minutes = 0;
        RTC_TimeStructure.RTC_Seconds = 0;
        WriteAllRTCData(&RTC_TimeStructure, &RTC_DateStructure);
        RTC_ExitConfigMode();

        BKP_WriteBackupRegister(BKP_DR1, 0X5050);
    }
    else
    {
        RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }

    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    ReadAllRTCData(&RTC_TimeStructure, &RTC_DateStructure);


    return 0;
}


void RTC_Suspend(void)
{
    RTC_WakeUp_Config(ENABLE);
    SetAlarmTime();

    RtcBspInit.pRtcPrvDataInit->isStandby = ENABLE;

#if USE_RTC_BSP_DEBUG==1
    printf("RTC_Suspend\r\n");
#endif
}

void RTC_Resume(void)
{
    RTC_WakeUp_Config(DISABLE);
    RtcBspInit.pRtcPrvDataInit->isStandby = DISABLE;

#if USE_RTC_BSP_DEBUG==1
    printf("RTC_Resume\r\n");
#endif
}


void RTC_IRQHandler(void)
{

    if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_SEC);

#if USE_RTC_BSP_DEBUG==1
        printf("Memory:%d\r\n", mem_perused(SRAMIN));
#endif
    }

    if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_ALR);
        EXTI_ClearITPendingBit(EXTI_Line17);

#if USE_RTC_BSP_DEBUG==1
        SystemInit();
#endif

        SetAlarmStatus(DISABLE);
        SetAlarmTime();
        SetAlarmStatus(ENABLE);




#if USE_RTC_BSP_DEBUG==1
        printf("RTC_IRQHandler->RTC_IT_ALR\r\n");
#endif
    }

}

void RTCAlarm_IRQHandler(void)
{
    if(RTC_GetITStatus(EXTI_Line17) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line17);
        RTC_ClearITPendingBit(RTC_IT_ALR);

#if USE_RTC_BSP_DEBUG==1
        SystemInit();
#endif

        SetAlarmStatus(DISABLE);
        SetAlarmTime();
        SetAlarmStatus(ENABLE);

        SetSysProperties_Status(WDT_FEED_STATUS);

    }

}

RtcBspTypeDef *GetRtcBspData(void)
{
    return &RtcBspInit;
}


RtcBspTypeDef *Rtc_AllBsp_Init(void)
{
    RtcBspInit.pRtcPrvDataInit = &RtcPrvDataInit;
    RtcBspInit.pRTC_DateStruct = &RTC_DateStructure;
    RtcBspInit.pRTC_TimeStruct = &RTC_TimeStructure;
    RtcBspInit.RTC_Get_Week = RTC_Get_Week;
    RtcBspInit.ReadAllRTCData = ReadAllRTCData;
    RtcBspInit.WriteAllRTCData = WriteAllRTCData;
    RtcBspInit.RTC_Resume = RTC_Resume;
    RtcBspInit.RTC_Suspend = RTC_Suspend;

    RTC_Timer_Config();
    RTC_WakeUp_Config(DISABLE);

    return &RtcBspInit;
}

