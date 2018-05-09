#include "BspCfg.h"

static AdcBspTypeDef 		 AdcBspInit;
static ADCPrvDataTypedef ADCPrvDataInit =
{
    0,
    {0},
    0
};


static void  ADC1_Init(void)
{
    ADC_InitTypeDef     ADC_InitStruct;
    GPIO_InitTypeDef    GPIO_InitStruct;

    /* Enable  GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    /* Configure PA.01  as analog input */
    GPIO_InitStruct.GPIO_Pin = CAPTURE_VOLTAGE_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CAPTURE_VOLTAGE_GPIO, &GPIO_InitStruct);

    /* Configure PC.01  as analog input */  //_su
    GPIO_InitStruct.GPIO_Pin = HARDWARE_DEVIDE_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HARDWARE_DEVIDE_GPIO, &GPIO_InitStruct);

    ADC_DeInit(ADC1);
    /*Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits */
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStruct);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

}


static u16 Get_Adc(u8 ch)
{
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
    return ADC_GetConversionValue(ADC1);
}


u8 Get_Hardware_Version(void)
{
    u16 Adc_Value[50], max = 0, min = 0x0fff;
    u32 Tal_Value = 0;
    u8 i;
    for(i = 0; i < 50; i++)
    {
        Adc_Value[i] = Get_Adc(HARDWARE_DEVIDE_CHANNEL);

    }

    for(i = 2; i < 48; i++)
    {
        Tal_Value += Adc_Value[i];
        if(Adc_Value[i] > max) max = Adc_Value[i];
        if(Adc_Value[i] < min) min = Adc_Value[i];

    }

    Tal_Value -= max;
    Tal_Value -= min;
    Tal_Value = Tal_Value / 44 ;

    if((Tal_Value > 0) && (Tal_Value < (HARDWARE_ADC_VAL_Interval + HARDWARE_ADC_VAL_Version_1)))
        return 1;

    if((Tal_Value >= HARDWARE_ADC_VAL_Version_2) && (Tal_Value < (HARDWARE_ADC_VAL_Interval + HARDWARE_ADC_VAL_Version_2)))
        return 2;

    if((Tal_Value >= HARDWARE_ADC_VAL_Version_3) && (Tal_Value < (HARDWARE_ADC_VAL_Interval + HARDWARE_ADC_VAL_Version_3)))
        return 3;

    if((Tal_Value >= HARDWARE_ADC_VAL_Version_4) && (Tal_Value < (HARDWARE_ADC_VAL_Interval + HARDWARE_ADC_VAL_Version_4)))
        return 4;

    if((Tal_Value >= HARDWARE_ADC_VAL_Version_5) && (Tal_Value < (HARDWARE_ADC_VAL_Interval + HARDWARE_ADC_VAL_Version_5)))
        return 5;

    if((Tal_Value >= HARDWARE_ADC_VAL_Version_6) && (Tal_Value < (HARDWARE_ADC_VAL_Interval + HARDWARE_ADC_VAL_Version_6)))
        return 6;

    else return 1;

}

static u16 GetADC1Average(u8 ch)
{
    u8 i = 0;
    u8 max_value = 0, min_value = 0, max_index = 1, min_index = 1;
    ADCPrvDataInit.adc_value[ADCPrvDataInit.GatherNum] = Get_Adc(ch);

    ADCPrvDataInit.GatherNum++;

    ADCPrvDataInit.ADCOverFlag = DISABLE;
    if(ADCPrvDataInit.GatherNum >= ADC_DATA_SIZE)
    {
        ADCPrvDataInit.GatherNum = 0;

        ADCPrvDataInit.adc_value_ave = 0;
        for(i = 1; i < ADC_DATA_SIZE; i++)
        {
            ADCPrvDataInit.adc_value_ave += ADCPrvDataInit.adc_value[i];
        }

        ADCPrvDataInit.adc_value_ave /= 7;

        for(i = 1; i < ADC_DATA_SIZE; i++)
        {
            if(ADCPrvDataInit.adc_value[i] > ADCPrvDataInit.adc_value_ave)
            {
                if((ADCPrvDataInit.adc_value[i] - ADCPrvDataInit.adc_value_ave) > max_value)
                {
                    max_value = ADCPrvDataInit.adc_value[i] - ADCPrvDataInit.adc_value_ave;
                    max_index = i;
                }
            }
            else
            {
                if((ADCPrvDataInit.adc_value_ave - ADCPrvDataInit.adc_value[i]) > min_value)
                {
                    min_value = ADCPrvDataInit.adc_value_ave - ADCPrvDataInit.adc_value[i];
                    min_index = i;
                }
            }
        }

        ADCPrvDataInit.adc_value_ave = 0;
        for(i = 1; i < ADC_DATA_SIZE; i++)
        {
            if((i != max_index) && (i != min_index))
            {
                ADCPrvDataInit.adc_value_ave += ADCPrvDataInit.adc_value[i];
            }
        }
        if(max_index != min_index)
        {
            ADCPrvDataInit.adc_value_ave /= 5;
        }
        else
        {
            ADCPrvDataInit.adc_value_ave /= 6;
        }
        //printf("adc_value_ave:%d,%d\r\n",ADCPrvDataInit.adc_value_ave,i);
        ADCPrvDataInit.adc_value_ave += CompasenVal;
#if USE_ADCBSP_DEBUG==1
        printf("ADVal:%d\r\n", ADCPrvDataInit.adc_value_ave);
#endif
        ADCPrvDataInit.ADCOverFlag = ENABLE;
    }

    return ADCPrvDataInit.ADCOverFlag;
}

static u8 ADC_Suspend(void)
{
    u8 Res = 0;
    ADCPrvDataInit.isStandby = ENABLE;
#if USE_ADCBSP_DEBUG==1
    printf("ADC_Suspend\r\n");
#endif
    return Res;
}

static u8 ADC_Resume(void)
{
    u8 Res = 0;

    ADCPrvDataInit.isStandby = DISABLE;


    return Res;
}

AdcBspTypeDef *GetAllADCBspData(void)
{
    return &AdcBspInit;
}

AdcBspTypeDef *ADC_AllBspInit(void)
{
    AdcBspInit.pADCPrvDataInit = &ADCPrvDataInit;
    AdcBspInit.ADC1_Init = ADC1_Init;
    AdcBspInit.GetADC1Average = GetADC1Average;
    AdcBspInit.ADC_Resume = ADC_Resume;
    AdcBspInit.ADC_Suspend = ADC_Suspend;
    ADC1_Init();
    ADCPrvDataInit.adc_value_ave = 0;

    return &AdcBspInit;
}



