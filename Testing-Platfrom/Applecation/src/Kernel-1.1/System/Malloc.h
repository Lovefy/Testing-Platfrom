#ifndef __MALLOC_H
#define __MALLOC_H
#include "sys.h"

#ifndef NULL
#define NULL 0
#endif

#define SRAMIN	0	//�ڲ��ڴ��
#define SRAMEX  1	//�ⲿ�ڴ��


/*Internal memory*/
#define MEM1_BLOCK_SIZE			32
#define MEM1_MAX_SIZE			15*1024
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE

/*SRAM memory*/
#define MEM2_BLOCK_SIZE			32
#define MEM2_MAX_SIZE			1*1024
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE


struct _m_mallco_dev
{
    void (*init)(u8);
    u8 (*perused)(u8);
    u8 	*membase[2];
    u16 *memmap[2];
    u8  memrdy[2];
};
extern struct _m_mallco_dev mallco_dev;

void mymemset(void *s, u8 c, u32 count);
void mymemcpy(void *des, void *src, u32 n);

void mem_init(u8 memx);
u32 mem_malloc(u8 memx, u32 size);
u8 mem_free(u8 memx, u32 offset);
u8 mem_perused(u8 memx);

void myfree(u8 memx, void *ptr);
void *mymalloc(u8 memx, u32 size);
void *myrealloc(u8 memx, void *ptr, u32 size);
#endif













