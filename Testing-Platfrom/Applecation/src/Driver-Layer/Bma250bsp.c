#include "Bma250bsp.h"

static BMA250BspTypeDef BMA250_BspInit;

static void BMA250_WR_Reg(u8 addr, u8 val)
{
    IIC_Start();
    IIC_Send_Byte(BMA250_WRITE_ADDR);
    IIC_Wait_Ack();
    IIC_Send_Byte(addr);
    IIC_Wait_Ack();
    IIC_Send_Byte(val);
    IIC_Wait_Ack();
    IIC_Stop();
}

static u8 BMA250_RD_Reg(u8 addr)
{
    u8 temp = 0;
    IIC_Start();
    IIC_Send_Byte(BMA250_WRITE_ADDR);
    temp = IIC_Wait_Ack();
    IIC_Send_Byte(addr);
    temp = IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(BMA250_READ_ADDR);
    temp = IIC_Wait_Ack();
    temp = IIC_Read_Byte(0);
    IIC_Stop();
    return temp;
}


static u8 BMA250_Init(void)
{
    IIC_Init();
    BMA250_WR_Reg(BGW_SOFTRESET, 0xB6);
    delay_ms(10);
    if(BMA250_RD_Reg(BGW_CHIPID) == BMA250_CHIPID)
    {
        /*set bandwidch --(default)0.5us*/
        BMA250_WR_Reg(PMU_BW, 0X0f);
        BMA250_WR_Reg(INT_8, 0X44);
        BMA250_WR_Reg(INT_RST_LATCH, 0X0A); /*temporary-500us latch_time*/
        /*BMA250_WR_Reg(INT_SRC,0X00);*/
        BMA250_WR_Reg(INT_MAP_0, 0X20);

        BMA250_WR_Reg(INT_OUT_CTRL, 0X05);

        printf("BMA250_Init is success!\r\n");
        return SUCCESS;
    }

#if USE_BMA250_DEBUG==1
    printf("BMA250_Init is error!\r\n");
#endif

    return ERROR;
}


static u8 BMA250_SetTapLevel(u8 SecLevel)
{
    u8 ACCRangeVal, GLevelVal;
    BMA250_WR_Reg(INT_EN_0, 0X00);
    if(SecLevel == 0)
    {
        goto CHECK_CHIP;
    }
    else
    {
        switch(SecLevel)
        {
        case 1:
            ACCRangeVal = ACC_RANGE_2G;
            GLevelVal = 16;
            break;
        case 2:
            ACCRangeVal = ACC_RANGE_4G;
            GLevelVal = 16;
            break;
        case 3:
            ACCRangeVal = ACC_RANGE_8G;
            GLevelVal = 16;
            break;
        case 4:
            ACCRangeVal = ACC_RANGE_16G;
            GLevelVal = 16;
            break;
        case 5:
            ACCRangeVal = ACC_RANGE_16G;
            GLevelVal = 31;
            break;
        default:
            break;
        }

        /*set  the accelerometer g-range(2,4,8,16g).  (default)-+2g */
        BMA250_WR_Reg(PMU_RANGE, ACCRangeVal);
        BMA250_WR_Reg(INT_9, 0Xc0 | GLevelVal);
        BMA250_WR_Reg(INT_EN_0, 0X20);
        /*printf("BMA250 INT cfg\r\n");*/
    }

CHECK_CHIP:
    if(BMA250_RD_Reg(BGW_CHIPID) == BMA250_CHIPID)
    {
        return SUCCESS;
    }
    else
        return ERROR;
}

static u8 BMA250_SetMode(DeviceModeEnum Mode)
{
    u8 Val;
    Val = BMA250_RD_Reg(PMU_LPW);
    if(STANDBY_MODE == Mode)
    {
        Val |= 0x80;
    }
    else if(NORMAL_MODE == Mode)
    {
        Val &= ~0x80;
    }
    BMA250_WR_Reg(PMU_LPW, Val);

    Val = BMA250_RD_Reg(PMU_LOW_POWER);
    Val |= 0x40;
    BMA250_WR_Reg(PMU_LOW_POWER, Val);
    return 0;
}

static void Bma250_Suspend(void)
{
    /*BMA250_SetMode(STANDBY_MODE);*/
    IIC_DeInit();
#if USE_BMA250_DEBUG==1
    printf("Bma250_Suspend\r\n");
#endif
}

static void Bma250_Resume(void)
{
    IIC_Init();
    /*BMA250_SetMode(NORMAL_MODE);*/
#if USE_BMA250_DEBUG==1
    printf("Bma250_Resume\r\n");
#endif
}

BMA250BspTypeDef *GetBma250BspData(void)
{
    return &BMA250_BspInit;
}

BMA250BspTypeDef *Bma250_AllBspInit(void)
{
    BMA250_BspInit.BMA250_Init = BMA250_Init;
    BMA250_BspInit.BMA250_SetTapLevel = BMA250_SetTapLevel;
    BMA250_BspInit.BMA250_SetMode = BMA250_SetMode;
    BMA250_BspInit.Bma250_Resume = Bma250_Resume;
    BMA250_BspInit.Bma250_Suspend = Bma250_Suspend;

    BMA250_Init();

    return &BMA250_BspInit;
}



