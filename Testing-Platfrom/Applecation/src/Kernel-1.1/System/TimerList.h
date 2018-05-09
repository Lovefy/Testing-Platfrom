#ifndef __TIMER_LIST_H
#define __TIMER_LIST_H


#include "stdlib.h"

/* gpio eint call back function */
typedef u32 (*peint_handle)(void *para);


__packed struct ElemType
{
    u8  Enable;     			/*enable flag*/

    char Name[20];				/*Node Serial number£¬it's a char string*/
    u32 expires;		   		/*Time counter*/

    peint_handle handler;	/*eint call back function*/
    void *parg;						/*Private data*/
};

typedef struct timerlist
{
    struct ElemType data;
    struct timerlist *prior, *next;
} TmrlistTypeDef;


extern TmrlistTypeDef *pTmrlistInit;

extern TmrlistTypeDef *InitTmrLinkedList(void);
/*TmrlistTypeDef* InserTmrLinkedNode(TmrlistTypeDef *pListNode,struct ElemType ElemInit);*/
extern TmrlistTypeDef *InserTmrLinkedNode(TmrlistTypeDef *pListNode, struct ElemType *pElemInit);
extern TmrlistTypeDef *FindTmrLinkedNode(TmrlistTypeDef *pListNode, char *Name);
extern u8 DeleleTmrLinkedNode(TmrlistTypeDef *pListNode, char *Name);
extern u8 GetLengthTmrLinkedList(TmrlistTypeDef *pListNode);
extern u8 DeleteTmrLinkedList(TmrlistTypeDef *pListNode);
extern TmrlistTypeDef *QueryTmrLinkedList(TmrlistTypeDef *pListNode, u8 addr);
extern void Timer_Link_Callback(void);



u8 Init_timer(struct ElemType *pLTmrElem);
u8 Add_timer(struct ElemType *pLTmrElem);
u8 Mod_timer(struct ElemType *pLTmrElem, FunctionalState State);
u8 Del_timer(struct ElemType *pLTmrElem);


#endif

