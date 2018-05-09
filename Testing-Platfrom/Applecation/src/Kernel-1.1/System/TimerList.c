/*
 *  ../DataStruct/TimerList.c
 *
 *  Declarations for the Double linked list timer.
 *
 *  Copyright (C) 2015 baony CO.,LTD
 */

#include "MALLOC.h"
#include "TimerList.h"
#include "sys.h"
#include "usart.h"


u32 timer_jiffies;
TmrlistTypeDef *pTmrlistInit;

TmrlistTypeDef *InitTmrLinkedList(void)
{
    /*Create a null linked list*/
    TmrlistTypeDef *Header = NULL;

    Header = (TmrlistTypeDef *)mymalloc(SRAMIN, sizeof(TmrlistTypeDef));
    if(Header != NULL)
    {
        Header->data.Enable = 0;
        Header->data.expires = 0;
        Header->data.handler = NULL;
        Header->data.parg = NULL;

        Header->prior = Header;
        Header->next = Header;
        return Header;
    }
    return NULL;
}

TmrlistTypeDef *InserTmrLinkedNode(TmrlistTypeDef *pListNode, struct ElemType *pElemInit)
{
    TmrlistTypeDef *pNew;

    pNew = (TmrlistTypeDef *)mymalloc(SRAMIN, sizeof(TmrlistTypeDef));

    if(pNew)
    {
        /*set the value*/
        memcpy(&pNew->data, pElemInit, sizeof(struct ElemType));

        pNew->prior = pListNode->prior;
        pNew->next = pListNode;

        pListNode->prior->next = pNew;
        pListNode->prior = pNew;

        return pNew;
    }
    return NULL;
}


TmrlistTypeDef *FindTmrLinkedNode(TmrlistTypeDef *pListNode, char *Name)
{
    TmrlistTypeDef  *pnode = pListNode->next;
    /*   printf("%s %s\r\n",pnode->data.Name,Name);
       printf("CmpVal:%d\r\n",strcmp(pnode->data.Name,Name));*/
    while((pnode != pListNode) && (strcmp(pnode->data.Name, Name) != 0))
    {
        pnode = pnode->next;
    }
    if (pnode == pListNode)  return NULL;

    return pnode;
}

u8 DeleleTmrLinkedNode(TmrlistTypeDef *pListNode, char *Name)
{
    TmrlistTypeDef *pnode = FindTmrLinkedNode(pListNode, Name);
    if (NULL == pnode) return NULL;

    pnode->prior->next = pnode->next;
    pnode->next->prior = pnode->prior;
    myfree(SRAMIN, pnode);

    return SUCCESS;
}

u8 DeleteTmrLinkedList(TmrlistTypeDef *pListNode)
{
    TmrlistTypeDef  *pnode = pListNode->next;
    TmrlistTypeDef  *pnode_next;
    if (NULL == pListNode) return NULL;

    while (pnode != pListNode)
    {
        pnode_next = pnode->next;
        myfree(SRAMIN, pnode);
        pnode = pnode_next;
    }
    myfree(SRAMIN, pListNode);
    return SUCCESS;
}

u8 GetLengthTmrLinkedList(TmrlistTypeDef *pListNode)
{
    TmrlistTypeDef *pNode = pListNode->next;
    u8 nCount = 0;
    while(pNode != pListNode)
    {
        pNode = pNode->next;
        nCount++;
    }

    return nCount;
}


TmrlistTypeDef *QueryTmrLinkedList(TmrlistTypeDef *pListNode, u8 addr)
{
    TmrlistTypeDef *pNode = pListNode->next;
    u8 nCount = 0;

    while(pNode != pListNode)
    {
        if(addr == nCount)
            return pNode;

        pNode = pNode->next;
        nCount++;
    }

    return NULL;
}


void Timer_Link_Callback(void)
{
    u8 TmrListLen, i;
    TmrlistTypeDef *pNode;

    timer_jiffies++;

    /*check timer-link*/
    TmrListLen = GetLengthTmrLinkedList( pTmrlistInit);
    for(i = 0; i < TmrListLen; i++)
    {
        pNode = QueryTmrLinkedList( pTmrlistInit, i);
        if(pNode)
        {
            if(pNode->data.Enable == 1)
            {
                if(timer_jiffies >= pNode->data.expires)
                {
                    pNode->data.Enable = 0;
                    if(pNode->data.handler)
                        pNode->data.handler(pNode->data.parg);
                }
            }
        }
    }
}

u8 Init_timer(struct ElemType *pLTmrElem)
{
    memset(pLTmrElem, 0, sizeof(struct ElemType));
    return 0;
}

u8 Add_timer(struct ElemType *pLTmrElem)
{
    u8 res = NULL;
    TmrlistTypeDef *pNode;

    if(pLTmrElem->Name == NULL || pLTmrElem->handler == NULL)
        return res;

    if(FindTmrLinkedNode( pTmrlistInit, pLTmrElem->Name))
        return NULL;

    pNode = InserTmrLinkedNode(pTmrlistInit, pLTmrElem);

    if(pNode)
        res = 1;

    return res;

}

u8 Mod_timer(struct ElemType *pLTmrElem, FunctionalState State)
{
    u8 res = NULL;
    TmrlistTypeDef *pNode;
    if(pLTmrElem->Name == NULL)
        return res;

    pNode = FindTmrLinkedNode( pTmrlistInit, pLTmrElem->Name);
    if(!pNode)
        return res;

    pNode->data.expires = timer_jiffies + pLTmrElem->expires;
    pNode->data.Enable = State;
    /*printf("set:%d\r\n",pNode->data.expires);*/
    return res = 1;
}

u8 Del_timer(struct ElemType *pLTmrElem)
{
    u8 res = NULL;

    if(pLTmrElem->Name == NULL)
        return res;

    res = DeleleTmrLinkedNode( pTmrlistInit, pLTmrElem->Name);

    return res;

}
