#ifndef __I2C_ADAPTOR_H
#define __I2C_ADAPTOR_H

#include "I2cBus.h"

#define USE_I2C_ADAPTOR 0

__packed typedef struct
{
    u8 Reg;
    u8 Val;
}
RegArrayTypedef;


__packed typedef struct
{
    u8 Cnt;


}
DeviceInterfaceTypedef;



__packed typedef struct
{
    u8 				*Name;    /*Device name*/
    I2CBUS_NO_ENUM 	 Index;   /*i2c bus index,@I2CBUS_NO_ENUM*/
    u16 			PhyAddr;
}
DeviceArrayTypedef;

__packed typedef struct
{
    DeviceArrayTypedef	   DeviceArray;

}
I2CDeviceTypedef;


__packed typedef struct
{
    BusInterfaceTypedef *pBase;
    I2CBusTypedef  *pBus;
    u8 *pData;
    u16 Len;
}
I2CAdaptorTypedef;


u8 I2CAdaptor_Init(void);


u8 I2CAdaptor_TransferData(I2CDeviceTypedef *pI2CDevice, u8 *pBuf, u16 count);
u8 I2CAdaptor_TransferAndVerifyData(I2CDeviceTypedef *pI2CDevice, u8 *pBuf, u16 count);
u8 I2CAdaptor_RequestData(I2CDeviceTypedef *pI2CDevice, u8 *pOldBuf, u8 *pNewBuf, u16 count);



#endif






