#pragma once

#include "defines.h"
#include "memory_pool.h"

// NOTE(daniel): THIS IMPLEMENTATION IS REALLY BAD!!!!
// i recommend using double_likned_list.h instead, takes slightly more memory and works better
// and is also more clean
// the reason is LinkedList_Free uses a linear search from the beginning in order to set the
// last element, in a doubly linked list we just use the pointer to the previous elemenet for
// that

struct LL_Element
{
    MP_Element mp_element;
    LL_Element *ll_next;
};

struct LinkedList
{
    LL_Element *first_element;
    LL_Element *last_element;
    uint32 count;

    MemoryPool mp;
};

bool LinkedList_Init(LinkedList *ll, uint32 element_size, uint32 max_elements);
void *LinkedList_GetNew(LinkedList *ll);
void LinkedList_Free(LinkedList *ll, LL_Element *el);
void LinkedList_DestroyList(LinkedList *ll);
LL_Element *LinkedList_GetNext(LL_Element *el);
