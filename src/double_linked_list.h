#pragma once

#include "defines.h"
#include "memory_pool.h"

struct DLL_Element
{
    MP_Element mp_element;
    DLL_Element *dll_prev, *dll_next;
};

struct DoubleLinkedList
{
    DLL_Element *first_element;
    DLL_Element *last_element;
    uint32 count;

    MemoryPool mp;
};

bool DoubleLinkedList_Init(DoubleLinkedList *ll, uint32 element_size, uint32 max_elements);
void *DoubleLinkedList_GetNew(DoubleLinkedList *ll);
void DoubleLinkedList_Free(DoubleLinkedList *ll, DLL_Element *el);
void DoubleLinkedList_DestroyList(DoubleLinkedList *ll);
DLL_Element *DoubleLinkedList_GetNext(DLL_Element *el);
DLL_Element *DoubleLinkedList_GetPrev(DLL_Element *el);
