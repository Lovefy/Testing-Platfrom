#ifndef __RTCBSP_H
#define __RTCBSP_H

#include "sys.h"



#define USE_RTC_BSP_DEBUG  1


/**
  * @brief  RTC Time structure definition
  */
__packed typedef struct
{
    uint8_t RTC_Hours;    /*!< Specifies the RTC Time Hour.
                        This parameter must be set to a value in the 0-12 range
                        if the RTC_HourFormat_12 is selected or 0-23 range if
                        the RTC_HourFormat_24 is selected. */

    uint8_t RTC_Minutes;  /*!< Specifies the RTC Time Minutes.
                        This parameter must be set to a value in the 0-59 range. */

    uint8_t RTC_Seconds;  /*!< Specifies the RTC Time Seconds.
                        This parameter must be set to a value in the 0-59 range. */

    /* uint8_t RTC_H12;    */  /*!< Specifies the RTC AM/PM Time.
                           This parameter can be a value of @ref RTC_AM_PM_Definitions */
} RTC_TimeTypeDef;

/**
  * @brief  RTC Date structure definition
  */
__packed typedef struct
{
    uint32_t RTC_Year;     /*!< Specifies the RTC Date Year.
                        This parameter must be set to a value in the 0-99 range. */

    uint8_t RTC_Month;	 /*!< Specifies the RTC Date Month.
								This parameter can be a value of @ref RTC_Month_Date_Definitions */

    uint8_t RTC_Date;     /*!< Specifies the RTC Date.
                        This parameter must be set to a value in the 1-31 range. */
    uint8_t RTC_WeekDay; /*!< Specifies the RTC Date WeekDay.
								This parameter can be a value of @ref RTC_WeekDay_Definitions */
} RTC_DateTypeDef;

/**
  * @brief  RTC Alarm structure definition
  */
typedef struct
{
    RTC_TimeTypeDef RTC_AlarmTime;     /*!< Specifies the RTC Alarm Time members. */

    uint32_t RTC_AlarmMask;            /*!< Specifies the RTC Alarm Masks.
                                     This parameter can be a value of @ref RTC_AlarmMask_Definitions */

    uint32_t RTC_AlarmDateWeekDaySel;  /*!< Specifies the RTC Alarm is on Date or WeekDay.
                                     This parameter can be a value of @ref RTC_AlarmDateWeekDay_Definitions */

    uint8_t RTC_AlarmDateWeekDay;      /*!< Specifies the RTC Alarm Date/WeekDay.
                                     This parameter must be set to a value in the 1-31 range
                                     if the Alarm Date is selected.
                                     This parameter can be a value of @ref RTC_WeekDay_Definitions
                                     if the Alarm WeekDay is selected. */
} RTC_AlarmTypeDef;

__packed typedef struct
{
    u8 isStandby;
} RtcPrvDataTypeDef, *pRtcPrvDataTypeDef;


__packed typedef struct
{
    pRtcPrvDataTypeDef pRtcPrvDataInit;

    void (*ReadAllRTCData)(RTC_TimeTypeDef *pRTC_TimeInit, RTC_DateTypeDef *pRTC_DateInit);
    u8 (*WriteAllRTCData)(RTC_TimeTypeDef *pRTC_TimeInit, RTC_DateTypeDef *pRTC_DateInit);
    u8 (*RTC_Get_Week)(u16 year, u8 month, u8 day);
    void (*RTC_Suspend)(void);
    void (*RTC_Resume)(void);

    RTC_TimeTypeDef *pRTC_TimeStruct;
    RTC_DateTypeDef *pRTC_DateStruct;
}
RtcBspTypeDef;

RtcBspTypeDef *GetRtcBspData(void);
RtcBspTypeDef *Rtc_AllBsp_Init(void);



#endif




