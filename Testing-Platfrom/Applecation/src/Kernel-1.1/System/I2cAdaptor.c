#include "I2cBus.h"
#include "I2cAdaptor.h"


static I2CAdaptorTypedef	I2CAdaptorInit;

/*
*This fllowing section is description for IIC Bus creator model
*/
static void IIC_Start(BusInterfaceTypedef *pBase)
{
    pBase->SDAOutCfg();
    pBase->SetSDA();
    pBase->SetSCL();
    delay_us(2);
    pBase->ClrSDA();
    delay_us(2);
    pBase->ClrSCL();
}

static void IIC_Stop(BusInterfaceTypedef *pBase)
{
    pBase->SDAOutCfg();
    pBase->ClrSCL();
    pBase->ClrSDA();
    delay_us(2);
    pBase->SetSCL();
    delay_us(2);
    pBase->SetSDA();
    delay_us(2);
}

static u8 IIC_Wait_Ack(BusInterfaceTypedef *pBase)
{
    u8 ucErrTime = 0;
    pBase->SDAInCfg();
    /*pBase->SetSDA();delay_us(2);*/
    pBase->SetSCL();
    delay_us(1);
    while(pBase->ReadSDA())
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            IIC_Stop(pBase);

            return 1;
        }
    }
    pBase->ClrSCL();

    return 0;
}

static void IIC_Ack(BusInterfaceTypedef *pBase)
{
    pBase->ClrSCL();
    pBase->SDAOutCfg();
    pBase->ClrSDA();
    delay_us(1);
    pBase->SetSCL();
    delay_us(1);
    pBase->ClrSCL();
}

static void IIC_NAck(BusInterfaceTypedef *pBase)
{
    pBase->ClrSCL();
    pBase->SDAOutCfg();
    pBase->SetSDA();
    delay_us(1);
    pBase->SetSCL();
    delay_us(1);
    pBase->ClrSCL();
}

static void IIC_Send_Byte(BusInterfaceTypedef *pBase, u8 txd)
{
    u8 t;
    pBase->SDAOutCfg();
    pBase->ClrSCL();
    for(t = 0; t < 8; t++)
    {
        if(txd & 0x80)
            pBase->SetSDA();
        else
            pBase->ClrSDA();

        txd <<= 1;
        delay_us(1);
        pBase->SetSCL();
        delay_us(2);
        pBase->ClrSCL();
        /*delay_us(2);*/
    }
}

static u8 IIC_Read_Byte(BusInterfaceTypedef *pBase, unsigned char ack)
{
    unsigned char i, receive = 0;
    pBase->SDAInCfg();

    for(i = 0; i < 8; i++ )
    {
        pBase->ClrSCL();
        delay_us(2);
        pBase->SetSCL();
        receive <<= 1;
        if(pBase->ReadSDA())receive++;
        delay_us(1);
    }
    if (!ack)
        IIC_NAck(pBase);
    else
        IIC_Ack(pBase);

    return receive;
}

u8 I2CAdaptor_Init(void)
{
    u8 i;
    /*configure II2Bus*/
    Mutil_I2CBus_Init();

    /*configure I2CDevice*/

    /*configure I2CAdaptor*/

}

static I2CBusTypedef *I2CAdaptor_Get_Bus(I2CDeviceTypedef *pI2CDevice)
{
    u8 i = 0;
    for(i = 0; i < IICBUS_NUMBER_MAX; i++)
    {
        if(pI2CDevice->DeviceArray.Index == I2CBusInit[i].Index)
        {
            return &I2CBusInit[i];
        }
    }

    return NULL;
}


u8 I2CAdaptor_TransferData(I2CDeviceTypedef *pI2CDevice, u8 *pBuf, u16 count)
{
    u16 i;
    u8 Data = 0;
    /*1.根据设备使用的IICbus信息选择总线*/
    I2CAdaptorInit.pBus = I2CAdaptor_Get_Bus(pI2CDevice);

    if(!I2CAdaptorInit.pBus)
    {
        return ERROR;
    }
    I2CAdaptorInit.pBus->Base.IIC_Init();


    /*2.写数据*/
    I2CAdaptorInit.pData = pBuf;
    I2CAdaptorInit.Len = count;

    for(i = 0; i < I2CAdaptorInit.Len / 2; i++)
    {
        /*Write data*/
        IIC_Start(&I2CAdaptorInit.pBus->Base);
        IIC_Send_Byte(&I2CAdaptorInit.pBus->Base, pI2CDevice->DeviceArray.PhyAddr);
        IIC_Wait_Ack(&I2CAdaptorInit.pBus->Base);
        IIC_Send_Byte(&I2CAdaptorInit.pBus->Base, *(I2CAdaptorInit.pData + i * 2));	 /*WRITE ADDR*/
        IIC_Wait_Ack(&I2CAdaptorInit.pBus->Base);
        IIC_Send_Byte(&I2CAdaptorInit.pBus->Base, *(I2CAdaptorInit.pData + i * 2 + 1)); /*WRITE REGISTER*/
        IIC_Wait_Ack(&I2CAdaptorInit.pBus->Base);
        IIC_Stop(&I2CAdaptorInit.pBus->Base);
    }

    I2CAdaptorInit.pBus->Base.IIC_DeInit();

    return SUCCESS;
}


u8 I2CAdaptor_TransferAndVerifyData(I2CDeviceTypedef *pI2CDevice, u8 *pBuf, u16 count)
{
    u16 i;
    u8 Data = 0;
    /*1.根据设备使用的IICbus信息选择总线*/
    I2CAdaptorInit.pBus = I2CAdaptor_Get_Bus(pI2CDevice);

    if(!I2CAdaptorInit.pBus)
    {
        return ERROR;
    }

    I2CAdaptorInit.pBus->Base.IIC_Init();


    /*2.写数据*/
    I2CAdaptorInit.pData = pBuf;
    I2CAdaptorInit.Len = count;

    for(i = 0; i < I2CAdaptorInit.Len / 2; i++)
    {
        /*Write data*/
        IIC_Start(&I2CAdaptorInit.pBus->Base);
        IIC_Send_Byte(&I2CAdaptorInit.pBus->Base, pI2CDevice->DeviceArray.PhyAddr);
        IIC_Wait_Ack(&I2CAdaptorInit.pBus->Base);
        IIC_Send_Byte(&I2CAdaptorInit.pBus->Base, *(I2CAdaptorInit.pData + i * 2));	 /*WRITE ADDR*/
        IIC_Wait_Ack(&I2CAdaptorInit.pBus->Base);
        IIC_Send_Byte(&I2CAdaptorInit.pBus->Base, *(I2CAdaptorInit.pData + i * 2 + 1)); /*WRITE REGISTER*/
        IIC_Wait_Ack(&I2CAdaptorInit.pBus->Base);
        IIC_Stop(&I2CAdaptorInit.pBus->Base);

        /*printf("Write Array:%d reg:%x val:%x\r\n",i,*(I2CAdaptorInit.pData+i*2),*(I2CAdaptorInit.pData+i*2+1));*/
        /*Read and verify data*/
        IIC_Start(&I2CAdaptorInit.pBus->Base);
        IIC_Send_Byte(&I2CAdaptorInit.pBus->Base, pI2CDevice->DeviceArray.PhyAddr);
        Data = IIC_Wait_Ack(&I2CAdaptorInit.pBus->Base);
        IIC_Send_Byte(&I2CAdaptorInit.pBus->Base, *(I2CAdaptorInit.pData + i * 2)); /*Read addr*/
        Data = IIC_Wait_Ack(&I2CAdaptorInit.pBus->Base);

        IIC_Start(&I2CAdaptorInit.pBus->Base);
        IIC_Send_Byte(&I2CAdaptorInit.pBus->Base, pI2CDevice->DeviceArray.PhyAddr + 1);
        Data = IIC_Wait_Ack(&I2CAdaptorInit.pBus->Base);
        Data = IIC_Read_Byte(&I2CAdaptorInit.pBus->Base, 0);
        IIC_Stop(&I2CAdaptorInit.pBus->Base);

#if USE_I2C_ADAPTOR==1
        printf("Verify Array:%d reg:%x val:%x Data:%x\r\n", i, *(I2CAdaptorInit.pData + i * 2), *(I2CAdaptorInit.pData + i * 2 + 1), Data);
#endif
    }

    I2CAdaptorInit.pBus->Base.IIC_DeInit();

    return SUCCESS;
}

u8 I2CAdaptor_RequestData(I2CDeviceTypedef *pI2CDevice, u8 *pOldBuf, u8 *pNewBuf, u16 count)
{
    u8 Data = 0;
    u16 i = 0;
    /*1.根据设备使用的IICbus信息选择总线*/
    I2CAdaptorInit.pBus = I2CAdaptor_Get_Bus(pI2CDevice);
    if(!I2CAdaptorInit.pBus)
    {
        return ERROR;
    }

    I2CAdaptorInit.pBus->Base.IIC_Init();

    /*2.读数据*/
    I2CAdaptorInit.pData = pOldBuf;
    I2CAdaptorInit.Len = count;

    for(i = 0; i < I2CAdaptorInit.Len / 2; i++)
    {

        IIC_Start(&I2CAdaptorInit.pBus->Base);
        IIC_Send_Byte(&I2CAdaptorInit.pBus->Base, pI2CDevice->DeviceArray.PhyAddr);
        Data = IIC_Wait_Ack(&I2CAdaptorInit.pBus->Base);
        IIC_Send_Byte(&I2CAdaptorInit.pBus->Base, *(I2CAdaptorInit.pData + i * 2)); /*Read addr*/
        Data = IIC_Wait_Ack(&I2CAdaptorInit.pBus->Base);

        IIC_Start(&I2CAdaptorInit.pBus->Base);
        IIC_Send_Byte(&I2CAdaptorInit.pBus->Base, pI2CDevice->DeviceArray.PhyAddr + 1);
        Data = IIC_Wait_Ack(&I2CAdaptorInit.pBus->Base);
        *(pNewBuf + 2 * i) = *(I2CAdaptorInit.pData + i * 2);
        Data = IIC_Read_Byte(&I2CAdaptorInit.pBus->Base, 0);
        *(pNewBuf + 2 * i + 1) = Data;
        IIC_Stop(&I2CAdaptorInit.pBus->Base);
#if USE_I2C_ADAPTOR==1
        printf("Read Array:%d reg:%x %x val:%x\r\n", i, *(I2CAdaptorInit.pData + 2 * i), *(I2CAdaptorInit.pData + 2 * i + 1), Data);
#endif
    }

    I2CAdaptorInit.pBus->Base.IIC_DeInit();

    return Data;
}





