#include "bspcfg.h"
#include "UpdateManModule.h"
#include "Bspcfg.h"

iapfun jump2app;

//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(u32 appxaddr)
{
    if(((*(vu32 *)appxaddr) & 0x2FFE0000) == 0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
    {
        LOG("%x\r\n", (*(vu32 *)appxaddr));
        jump2app = (iapfun) * (vu32 *)(appxaddr + 4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)
        MSR_MSP(*(vu32 *)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
        jump2app();									//��ת��APP.
    }
}










