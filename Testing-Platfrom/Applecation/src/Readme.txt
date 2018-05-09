////////////////////////////////////////////////////
//			深圳市灵动飞扬有限公司				  //
//---------------V40 APP程序----------------------//
////////////////////////////////////////////////////

所有资源：
//STM32F103RCT6-CPU资源使用表
定时器资源：
TIM1:L转信号线检测
TIM2:软件定时器和固定100ms定时
TIM3:D倒车检测
TIM4：红外脉冲捕获
TIM5:蜂鸣器PWM
TIM6:R方向检测
TIM7:ACC检测
TIM8:


外部中断资源：
EXIT0:EX-G-SENSER0 	PC0
EXIT2:EX-G-SENSER2	PC2
EXIT3:RIGHT 		PB3
EXIT4:LEFT			PB4
EXIT5:ACC			PA5
EXIT15:IN-G-SERNER	PA15


其他固定功能引脚：
PA0                 3V3_EN      底板3V3电源        低有效
PA1                 UART_C      扩展串口电源控制   高有效
PA2				    USBC_C      外界USB电源控制    高有效
PA3					PWM         PWM蜂鸣器驱动      TIM5
PA4                 PWR_ADC     ACC的电压检测ADC 
PA5  				ACC_INT     ACC外部中断检测    ACC存在为低
PA6                 D_INT       倒车检测引脚       低有效(或方波)
PA7                 UC15_PWRKEY 3G开机信号         低脉冲
PA8 				STB          CAN TJA1040使能   低有效
PA9					RST#_A20     V18休眠唤醒复位信号 高电平有效
PA10				WAKEUP       V18 休眠唤醒信号  低电平有效上升沿触发
PA11				CAN1_RX
PA12				CAN1_TX
PA13				SWDIO        SWD调试口
PA14 				SWCLK		 SWD调试口
PA15 				G-INT        内部G——SENSOR     低中断


PB0                 +3V7_GPS_EN GPS电源控制        低有效
PB1          		L70_RESET   GPS复位信号        高电平
PB2                 BOOT1
PB3					R_INT       R信号检测          低有效(或方波)
PB4                 L_INT       L信号检测          低有效(或方波)
PB5                 UC15_RST	3G复位信号		   低脉冲（>0.2S）	
PB6 				PWR_ON       V40开机键         高脉冲（>500ms）	
PB7                 I2C1_SDA 
PB8       			IR_IN        红外检测引脚
PB9                 DVD_C        屏电源控制         高有效
PB10                TX3
PB11				RX3
PB12				CAN2_RX
PB13				CAN2_TX	
PB14                +5V_MAIN_EN   核心板5V电源控制  高有效
PB15 			    CCD12_PWR	  摄像头电源控制	高有效

PC0					G_INT1        外部G_SenSor1检测  低中断
PC1					ADC检测       硬件底板检测
PC2					G_INT2        外部G_SenSor2检测	 低中断
PC3                 WIFI_3V3_EN   WIFI +3.3V电源控制 低有效
PC4					+3V7_3G_EN	 3G VBAT 3.7V电源控制 低有效
PC5                 +5V_3G_EN    3G USB +5V电源控制  高有效
PC6					LED_Control1 电源指示灯绿灯      高有效
PC7					MCU_SCL3     3G-sensor控制 		 BMA250 
PC8					MCU_SDA3     3G-sensor控制 		 BMA250 
PC9                 LED_Control2 CAN指示灯 红灯      高有效	
PC10				UART4_TX     单片机TX
PC11             	UART4_RX     单片机RX
PC12                UART5_Tx     单片机调试串口
PC13
PC14
PC15

PD2              	UART5_Rx     单片机调试串口



	
内存分布：
FLASH分布;
0x08000000-0x08002800   10K         BootLoader
0x08002800-0x0800C800   40K         Recover
0x0800C800-0x08021C00	85K 		APP
0x08021C00-0x08037000	85K         UPdateBuf
0x08037000-0x08037800	2K          EEP_USERCFG_CAN_FLASH
0x08037800-0x0803F000	30K			预留



中断分布表：
中断工作组：2
						优先级   子优先级
USB_LP_CAN1_RX0_IRQn    1           0
CAN2_RX1_IRQn           1           1
TIM4_IRQn               2           1          IR
TIM7_IRQn               1           3          ACC   
EXTI9_5_IRQn            1           3          ACC
EXTI4_IRQn              2           2          LEFT
EXTI3_IRQn      	    2 			2 		   RIGHT
EXTI15_10_IRQn     		2			3		   INTER_GSENSER 
EXTI0_IRQn      	    2 			3 		   EXTER_GSENSER1
EXTI2_IRQn              2 		    3  		   EXTER_GSENSER2
RTCAlarm_IRQn           2   	    3      
RTC_IRQn                2 		    3
TIM1_UP_IRQn            2  		    2  		   LEFT   
TIM2_IRQn               3   	    2     	   
TIM3_IRQn               2     	    1  
TIM6_IRQn               2     	    1
DMA1_Channel3_IRQn		1   	    2   	   Rx
DMA1_Channel2_IRQn      1           3 		   TX
USART3_IRQn             1    	    2    

//Add history==========================================================================
2016-10-18
整理发布版本0.0.22
