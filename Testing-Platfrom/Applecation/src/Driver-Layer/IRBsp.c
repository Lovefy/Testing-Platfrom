#include "BspCfg.h"

static u8  RmtSta = 0;
static u16 Dval;		  /*下降沿时计数器的值*/
static u32 RmtRec = 0;	/*红外接收到的数据*/
static u8  RmtCnt = 0;	/*按键按下的次数	*/

static IRCheckTypedef 	IRCheckInitstruct = {0, 0xff, 0};
static IRPrvDataTypedef IRPrvDataInit;
static IRBspTypeDef 		IRBspInit;


//static void IR_LongPressCallback(void);

static void PortInit(GPIOMode_TypeDef GPIO_Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}


/*设置IO以及定时器4的输入捕获*/
static void IRInit(void)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;

    PortInit(GPIO_Mode_IPU);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 10000;
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x03;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);

    TIM_Cmd(TIM4, ENABLE );

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);

}


void TIM4_IRQHandler(void)
{

    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearFlag(TIM4, TIM_IT_Update);
        if(RmtSta & 0x80)
        {
            RmtSta &= ~0X10;
            if((RmtSta & 0X0F) == 0X00)
            {
                RmtSta |= 1 << 6;
                /* LOG("capture:%x \r\n",RmtRec);*/
            }
            if((RmtSta & 0X0F) < 8)RmtSta++;
            else
            {
                RmtSta &= ~(1 << 7);
                RmtSta &= 0XF0;
                /*LOG("Time out:%x \r\n",RmtRec);*/
            }
        }

    }
    if(TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
    {
        TIM_ClearFlag(TIM4, TIM_IT_CC3);
        if(RDATA != Bit_RESET)
        {
            TIM_OC3PolarityConfig(TIM4, TIM_ICPolarity_Falling);
            TIM_SetCounter(TIM4, 0);
            RmtSta |= 0X10;

        }
        else
        {
            Dval = TIM4->CCR3;
            TIM_OC3PolarityConfig(TIM4, TIM_ICPolarity_Rising);
            if((RmtSta & 0X10) == 0x10)
            {
                if((RmtSta & 0X80) == 0x80)
                {
                    if(Dval > 300 && Dval < 800)
                    {
                        RmtRec <<= 1;
                        RmtRec |= 0;
                    }
                    else if(Dval > 1200 && Dval < 1800)
                    {
                        RmtRec <<= 1;
                        RmtRec |= 1;

                    }
                    else if(Dval > 2000 && Dval < 2600)
                    {
                        RmtCnt++;
                        if(RmtCnt >= 255)
                            RmtCnt = 255;
                        RmtSta &= 0XF0;

                    }

                }
                else if(Dval > 4200 && Dval < 4700)
                {
                    RmtSta |= 1 << 7;
                    RmtCnt = 0;
                    TIM_ITConfig( TIM4, TIM_IT_Update, ENABLE);
                    TIM_ClearFlag(TIM4, TIM_IT_Update);
                    RmtRec = 0;
                }
                RmtSta &= ~(1 << 4);
            }

        }
    }

}

static u16 IR_Scan(void)
{
    pIRCheckTypedef p;
    u8 sta = IR_ERR_VAL;
    u8 t1, t2;
    u8 i;
    u16 Temp = 0;
    u16 Buf = 0;

    p = &IRCheckInitstruct;

    if((RmtSta & (0x40)) == 0x40)
    {
        t1 = RmtRec >> 24;
        t2 = (RmtRec >> 16) & 0xff;

        if((t1 == (u8)~t2) && t1 == IR_ID)
        {
            t1 = RmtRec >> 8;
            t2 = RmtRec;
            if(t1 == (u8)~t2)
                sta = t1;
            switch(sta)
            {
            case 176:
                p->KeyVal = 0x30;
                break;		/*0*/
            case 8:
                p->KeyVal = 0x31;
                break;
            case 136:
                p->KeyVal = 0x32;
                break;
            case 72:
                p->KeyVal = 0x33;
                break;		//3
            case 40:
                p->KeyVal = 0x34;
                break;		//4
            case 168:
                p->KeyVal = 0x35;
                break;
            case 104:
                p->KeyVal = 0x36;
                break;
            case 24:
                p->KeyVal = 0x37;
                break;
            case 152:
                p->KeyVal = 0x38;
                break;
            case 88:
                p->KeyVal = 0x39;
                break;		/*9*/

            case 16:
                p->KeyVal = 0x60;
                break;		/* +/-	    */
            case 48:
                p->KeyVal = 0x2B;
                break;	 	/* increase */
            case 112:
                p->KeyVal = 0x2D;
                break;		/* decrease */
            case 32:
                p->KeyVal = 0x41;
                break;		/* left			*/
            case 96:
                p->KeyVal = 0x64;
                break;		/* right		*/
            case 128:
                p->KeyVal = 0x57;
                break;	  /* up*/
            case 144:
                p->KeyVal = 0x58;
                break;		/* down		 	*/
            case 160:
                p->KeyVal = 0x53;
                break;		/* OK  			*/
            case 80:
                p->KeyVal = 0x4C;
                break;		/* save  		*/
            case 64:
                p->KeyVal = 0x72;
                break;		/* return 	*/
            case 0:
                p->KeyVal = 0x88;
                break;		/* pwr			*/
            default:
                p->KeyVal = 0;
                break;		/* Error-0X00		*/
            }
            IRPrvDataInit.KeyInputSource = KEY_TYPE_IR;
            IRPrvDataInit.KeyStatus = KEY_STATUS_IS_DOWN;

        }
        else if(t1 == 0x55 && t2 == 0xaa)
        {
            Buf = RmtRec & 0xffff;
            for(i = 0; i < 16; i++)
            {
                Temp <<= 1;
                if(Buf & 0x0001)
                {
                    Temp |= 0x0001;
                }
                else
                {
                    Temp &= ~0x0001;
                }
                Buf = Buf >> 1;
            }

            p->KeyVal = Temp & 0xffff;
            IRPrvDataInit.KeyInputSource = KEY_TYPE_TOUCH;
            IRPrvDataInit.KeyStatus = KEY_STATUS_IS_DOWN;

            /*LOG("KEY_TYPE_TOUCH\r\n");*/
        }
        else
        {
            p->KeyVal = 0;
        }
        /*通过下面两个条件防止误动作产生*/
        if(((RmtSta & 0X80) == 0)) /*按键数据错误/遥控已经没有按下了*/
        {
            RmtSta &= ~(1 << 6);		 /*清除接收到有效按键标识*/
            p->OverFlag = 0x01;
            p->RmtCnt = RmtCnt;

            switch(sta)
            {
            case 176:
                p->KeyVal = 0x30;
                break;
            case 8:
                p->KeyVal = 0x31;
                break;
            case 136:
                p->KeyVal = 0x32;
                break;
            case 72:
                p->KeyVal = 0x33;
                break;		//3
            case 40:
                p->KeyVal = 0x34;
                break;		//4
            case 168:
                p->KeyVal = 0x35;
                break;
            case 104:
                p->KeyVal = 0x36;
                break;
            case 24:
                p->KeyVal = 0x37;
                break;
            case 152:
                p->KeyVal = 0x38;
                break;
            case 88:
                p->KeyVal = 0x39;
                break;

            case 16:
                p->KeyVal = 0x60;
                break;
            case 48:
                p->KeyVal = 0x2B;
                break;
            case 112:
                p->KeyVal = 0x2D;
                break;
            case 32:
                p->KeyVal = 0x41;
                break;
            case 96:
                p->KeyVal = 0x64;
                break;
            case 128:
                p->KeyVal = 0x57;
                break;
            case 144:
                p->KeyVal = 0x58;
                break;
            case 160:
                p->KeyVal = 0x53;
                break;
            case 80:
                p->KeyVal = 0x4C;
                break;
            case 64:
                p->KeyVal = 0x72;
                break;
            case 0:
                p->KeyVal = 0x88;
                break;
            default:
                p->KeyVal = 0;
                break;		/* Error-0X00		*/
            }

            RmtCnt = 0;
            RmtRec = 0;

            TIM_ITConfig( TIM4, TIM_IT_Update, DISABLE);
            IRPrvDataInit.KeyStatus = KEY_STATUS_IS_UP;

            if(IRPrvDataInit.KeyInputSource == KEY_TYPE_TOUCH)
                IRCheckInitstruct.ExitOnce = ENABLE;
        }

    }
    else
        p->KeyVal = 0;

    return p->KeyVal;
}


static u8 IRKeyProcess(void)
{
    IRPrvDataInit.KeyLastValue = IR_Scan();
    if(IRPrvDataInit.KeyLastValue != 0)
    {
        if(IRPrvDataInit.KeyType == KeyIsNotPressed)
        {
            IRPrvDataInit.KeyType = KeyIsShortPressed;
            IRPrvDataInit.KeyPreValue = IRPrvDataInit.KeyLastValue;

            IRPrvDataInit.LongPressedEnable = ENABLE;
            IRPrvDataInit.KeyUpdateFlag = ENABLE;

            IRPrvDataInit.KeyType = KEY_STATUS_IS_DOWN;
#if USE_IR_BSP_DEBUG==1
            LOG("短按:%x\r\n", IRPrvDataInit.KeyLastValue);
#endif
        }
        else if(IRPrvDataInit.KeyType == KeyIsLongPressed)
        {
            if(IRPrvDataInit.LongPressedOverFlag == 1)
            {
                IRPrvDataInit.LongPressedOverFlag = 0;
                IRPrvDataInit.KeyUpdateFlag = ENABLE;
#if USE_IR_BSP_DEBUG==1
                LOG("长按:%x\r\n", IRPrvDataInit.KeyLastValue);
#endif
            }
        }

    }
    else
    {
        /*LOG("ir exit\r\n");*/
        IRPrvDataInit.LongPressedEnable = DISABLE;

        IRPrvDataInit.KeyCntTime = 0;
        IRPrvDataInit.KeyType = KeyIsNotPressed;
        if(IRCheckInitstruct.ExitOnce == ENABLE)
        {
            IRPrvDataInit.KeyUpdateFlag = ENABLE;
            IRCheckInitstruct.ExitOnce = DISABLE;
        }
        else
            IRPrvDataInit.KeyUpdateFlag = DISABLE;

        IRPrvDataInit.KeyType = KEY_STATUS_IS_UP;
    }
    return IRPrvDataInit.KeyUpdateFlag;
}


IRBspTypeDef *GetIRBspData(void)
{
    return &IRBspInit;
}

IRBspTypeDef  *IR_AllBspInit(void)
{

    IRBspInit.IRBsp_Init = IRInit;
    IRBspInit.IRKeyProcess = IRKeyProcess;
    IRBspInit.pIRPrvDataInit = &IRPrvDataInit;
    IRInit();
    return &IRBspInit;
}

