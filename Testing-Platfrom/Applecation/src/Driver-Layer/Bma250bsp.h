#ifndef __BMA250_H
#define __BMA250_H
#include "iicbsp.h"


#define USE_BMA250_DEBUG  1

/* Range selection definitions*/
#define ACC_RANGE_2G                0x03    /*  3.91 mg/LSB*/
#define ACC_RANGE_4G                0x05    /*  7.81 mg/LSB*/
#define ACC_RANGE_8G                0x08    /* 15.62 mg/LSB*/
#define ACC_RANGE_16G               0x0C    /* 31.25 mg/LSB*/

/* Filtered bandwidth selection (delta_t = time between successive acc samples)*/
#define ACC_BW_7_81HZ               0x08    /* 7.81Hz bandwidth (delta_t = 64 ms)*/
#define ACC_BW_15_63HZ              0x09    /* delta_t = 32   ms*/
#define ACC_BW_31_25HZ              0x0A    /* delta_t = 16   ms*/
#define ACC_BW_62_5HZ               0x0B    /* delta_t =  8   ms*/
#define ACC_BW_125HZ                0x0C    /* delta_t =  4   ms*/
#define ACC_BW_250HZ                0x0D    /* delta_t =  2   ms*/
#define ACC_BW_500HZ                0x0E    /* delta_t =  1   ms*/
#define ACC_BW_1000HZ               0x0F    /* delta_t =  0.5 ms*/

#define ACC_PM_SUSP                 0x80    /* Power mode register (0x11), bit 7*/
#define ACC_PM_LP                   0x40    /* Low power mode*/
#define ACC_PM_SLEEP_10MS           0x14
#define ACC_PM_SLEEP_25MS           0x16
#define ACC_PM_SLEEP_50MS           0x18

/* Interrupt enable bitmasks (for use with registers ACC_INT_ENABLEx [x=0,1] )*/
#define ACC_INT_FLAT_EN             0x80    /* Bit in register 0x16*/
#define ACC_INT_ORIENT_EN           0x40    /*          "*/
#define ACC_INT_S_TAP_EN            0x20    /*          "*/
#define ACC_INT_D_TAP_EN            0x10    /*          "*/
#define ACC_INT_SLOPE_Z_EN          0x04    /*          "*/
#define ACC_INT_SLOPE_Y_EN          0x02    /*          "*/
#define ACC_INT_SLOPE_X_EN          0x01    /*          "*/
#define ACC_INT_DATA_EN             0x10    /* Bit in register 0x17*/
#define ACC_INT_LOW_EN              0x08    /*          "*/
#define ACC_INT_HIGH_Z_EN           0x04    /*          "*/
#define ACC_INT_HIGH_Y_EN           0x02    /*          "*/
#define ACC_INT_HIGH_X_EN           0x01    /*          "*/

/* Interrupt mapping bitmasks (for use with registers ACC_INT_MAPPINGx [x=0,1,2] )*/
#define ACC_INT_MAP_FLAT            0x80    /* For pin INT1 (INT2), bit in register 0x19 (0x1B)*/
#define ACC_INT_MAP_ORIENT          0x40    /*                   "*/
#define ACC_INT_MAP_S_TAP           0x20    /*                   "*/
#define ACC_INT_MAP_D_TAP           0x10    /*                   "*/
#define ACC_INT_MAP_SLOPE           0x04    /*                   "*/
#define ACC_INT_MAP_HIGH            0x02    /*                   "*/
#define ACC_INT_MAP_LOW             0x01    /*                   "*/
#define ACC_INT1_MAP_DATA           0x01    /* New data IRQ to pin INT1, bit in register 0x1A*/
#define ACC_INT2_MAP_DATA           0x80    /* New data IRQ to pin INT2, bit in register 0x1A*/

/* Interrupt source bitmasks (for use with register ACC_INT_SOURCE)*/
#define ACC_INT_SRC_DATA_FILT       0x20
#define ACC_INT_SRC_TAP_FILT        0x01
#define ACC_INT_SRC_SLOPE_FILT      0x04
#define ACC_INT_SRC_HIGH_FILT       0x02
#define ACC_INT_SRC_LOW_FILT        0x01

/* Interrupt pin behavior bitmasks (for use with register (Open drive/push-pull and active level 0/1)*/
#define ACC_INT2_OD                 0x08
#define ACC_INT2_LVL                0x04
#define ACC_INT1_OD                 0x02
#define ACC_INT1_LVL                0x01


/*Register define*/
#define BGW_CHIPID		0X00 	/*BMA250ID,0XF9*/

#define ACCD_X_LSB    0X02
#define ACCD_X_MSB    0X03
#define ACCD_Y_LSB    0X04
#define ACCD_Y_MSB    0X05
#define ACCD_Z_LSB    0X06
#define ACCD_Z_MSB    0X07
#define ACCD_TEMP     0X08
#define INT_STATUS_0  0X09
#define INT_STATUS_1  0X0A
#define INT_STATUS_2  0X0B
#define INT_STATUS_3  0X0C

#define FIFO_STATUS   0X0E
#define PMU_RANGE     0X0F
#define PMU_BW        0X10
#define PMU_LPW       0X11
#define PMU_LOW_POWER 0X12
#define ACCD_HBW      0X13
#define BGW_SOFTRESET 0X14  /*Controls user triggered reset of the sensor.*/

#define INT_EN_0      0X16  /*Controls which interrupt engines in group 0 are enabled.*/
#define INT_EN_1      0X17  /*Controls which interrupt engines in group 1 are enabled.*/
#define INT_EN_2      0X18  /*Controls which interrupt engines in group 2 are enabled.*/
#define INT_MAP_0     0X19  /*Controls which interrupt signals are mapped to the INT1 pin.*/
#define INT_MAP_1     0X1A  /*Controls which interrupt signals are mapped to the INT1 and INT2 pins.*/
#define INT_MAP_2     0X1B  /*Controls which interrupt signals are mapped to the INT2 pin.*/

#define INT_SRC       0X1E  /*Contains the data source definition for interrupts with selectable data source.*/

#define INT_OUT_CTRL  0X20  /*Contains the behavioural configuration (electrical behaviour) of the interrupt pins.*/
#define INT_RST_LATCH 0X21
#define INT_0         0X22
#define INT_1         0X23
#define INT_2         0X24
#define INT_3         0X25
#define INT_4         0X26
#define INT_5         0X27
#define INT_6         0X28
#define INT_7         0X29
#define INT_8         0X2A
#define INT_9         0X2B
#define INT_A         0X2C
#define INT_B         0X2D
#define INT_C         0X2E
#define INT_D         0X2F

#define FIFO_CONFIG_0 0X30
#define PMU_SELF_TEST 0X32
#define TRIM_NVM_CTRL 0X33
#define BGW_SPI3_WDT  0X34

#define OFC_CTRL      0X36
#define OFC_SETTING   0X37
#define OFC_OFFSET_X  0X38
#define OFC_OFFSET_Y  0X39
#define OFC_OFFSET_Z  0X3A
#define TRIM_GP0      0X3B
#define TRIM_GP1      0X3C
#define FIFO_CONFIG_1 0X3E
#define FIFO_DATA     0X3F

#define BMA250_CHIPID       0XF9
#define BMA250_READ_ADDR    0X31
#define BMA250_WRITE_ADDR   0X30

__packed typedef struct
{
    u8 (*BMA250_SetTapLevel)(u8 SecLevel);
    u8 (*BMA250_Init)(void);
    u8 (*BMA250_SetMode)(DeviceModeEnum Mode);
    void (*Bma250_Suspend)(void);
    void (*Bma250_Resume)(void);
}
BMA250BspTypeDef;

BMA250BspTypeDef *GetBma250BspData(void);
BMA250BspTypeDef *Bma250_AllBspInit(void);


#endif

