////////////////////////////////////////////////////
//			�������鶯�������޹�˾				  //
//---------------V40 APP����----------------------//
////////////////////////////////////////////////////

������Դ��
//STM32F103RCT6-CPU��Դʹ�ñ�
��ʱ����Դ��
TIM1:Lת�ź��߼��
TIM2:�����ʱ���͹̶�100ms��ʱ
TIM3:D�������
TIM4���������岶��
TIM5:������PWM
TIM6:R������
TIM7:ACC���
TIM8:


�ⲿ�ж���Դ��
EXIT0:EX-G-SENSER0 	PC0
EXIT2:EX-G-SENSER2	PC2
EXIT3:RIGHT 		PB3
EXIT4:LEFT			PB4
EXIT5:ACC			PA5
EXIT15:IN-G-SERNER	PA15


�����̶��������ţ�
PA0                 3V3_EN      �װ�3V3��Դ        ����Ч
PA1                 UART_C      ��չ���ڵ�Դ����   ����Ч
PA2				    USBC_C      ���USB��Դ����    ����Ч
PA3					PWM         PWM����������      TIM5
PA4                 PWR_ADC     ACC�ĵ�ѹ���ADC 
PA5  				ACC_INT     ACC�ⲿ�жϼ��    ACC����Ϊ��
PA6                 D_INT       �����������       ����Ч(�򷽲�)
PA7                 UC15_PWRKEY 3G�����ź�         ������
PA8 				STB          CAN TJA1040ʹ��   ����Ч
PA9					RST#_A20     V18���߻��Ѹ�λ�ź� �ߵ�ƽ��Ч
PA10				WAKEUP       V18 ���߻����ź�  �͵�ƽ��Ч�����ش���
PA11				CAN1_RX
PA12				CAN1_TX
PA13				SWDIO        SWD���Կ�
PA14 				SWCLK		 SWD���Կ�
PA15 				G-INT        �ڲ�G����SENSOR     ���ж�


PB0                 +3V7_GPS_EN GPS��Դ����        ����Ч
PB1          		L70_RESET   GPS��λ�ź�        �ߵ�ƽ
PB2                 BOOT1
PB3					R_INT       R�źż��          ����Ч(�򷽲�)
PB4                 L_INT       L�źż��          ����Ч(�򷽲�)
PB5                 UC15_RST	3G��λ�ź�		   �����壨>0.2S��	
PB6 				PWR_ON       V40������         �����壨>500ms��	
PB7                 I2C1_SDA 
PB8       			IR_IN        ����������
PB9                 DVD_C        ����Դ����         ����Ч
PB10                TX3
PB11				RX3
PB12				CAN2_RX
PB13				CAN2_TX	
PB14                +5V_MAIN_EN   ���İ�5V��Դ����  ����Ч
PB15 			    CCD12_PWR	  ����ͷ��Դ����	����Ч

PC0					G_INT1        �ⲿG_SenSor1���  ���ж�
PC1					ADC���       Ӳ���װ���
PC2					G_INT2        �ⲿG_SenSor2���	 ���ж�
PC3                 WIFI_3V3_EN   WIFI +3.3V��Դ���� ����Ч
PC4					+3V7_3G_EN	 3G VBAT 3.7V��Դ���� ����Ч
PC5                 +5V_3G_EN    3G USB +5V��Դ����  ����Ч
PC6					LED_Control1 ��Դָʾ���̵�      ����Ч
PC7					MCU_SCL3     3G-sensor���� 		 BMA250 
PC8					MCU_SDA3     3G-sensor���� 		 BMA250 
PC9                 LED_Control2 CANָʾ�� ���      ����Ч	
PC10				UART4_TX     ��Ƭ��TX
PC11             	UART4_RX     ��Ƭ��RX
PC12                UART5_Tx     ��Ƭ�����Դ���
PC13
PC14
PC15

PD2              	UART5_Rx     ��Ƭ�����Դ���



	
�ڴ�ֲ���
FLASH�ֲ�;
0x08000000-0x08002800   10K         BootLoader
0x08002800-0x0800C800   40K         Recover
0x0800C800-0x08021C00	85K 		APP
0x08021C00-0x08037000	85K         UPdateBuf
0x08037000-0x08037800	2K          EEP_USERCFG_CAN_FLASH
0x08037800-0x0803F000	30K			Ԥ��



�жϷֲ���
�жϹ����飺2
						���ȼ�   �����ȼ�
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
�������汾0.0.22
