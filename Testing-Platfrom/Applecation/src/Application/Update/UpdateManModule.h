#ifndef __UPDATE_MANMODULE_H__
#define __UPDATE_MANMODULE_H__
#include "sys.h"
#include "BspCfg.h"
typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.
void iap_load_app(u32 appxaddr);			//ִ��flash�����app����
void iap_load_appsram(u32 appxaddr);		//ִ��sram�����app����


#endif







































