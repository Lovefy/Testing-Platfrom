#ifndef __UPDATE_MANMODULE_H__
#define __UPDATE_MANMODULE_H__
#include "sys.h"
#include "BspCfg.h"
typedef  void (*iapfun)(void);				//定义一个函数类型的参数.
void iap_load_app(u32 appxaddr);			//执行flash里面的app程序
void iap_load_appsram(u32 appxaddr);		//执行sram里面的app程序


#endif







































