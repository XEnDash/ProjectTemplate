#include "double_linked_list.h"

bool DoubleLinkedList_Init(DoubleLinkedList *dll, uint32 element_size, uint32 max_elements)
{
    DAssert(dll);
    DAssert(max_elements > 0);

    dll->first_element = 0;
    dll->count = 0;

    if(!MP_InitByCount(&dll->mp, element_size, max_elements))
        return false;
    
    return true;
}

void *DoubleLinkedList_GetNew(DoubleLinkedList *dll)
{
    DAssert(dll);
    
    DLL_Element *output = (DLL_Element *)MP_Alloc(&dll->mp);
    if(output == 0)
	return 0;

    if(dll->count == 0)
    {
	dll->first_element = output;
	dll->last_element = output;
	output->dll_next = 0;
	output->dll_prev = 0;

	dll->count++;

	return output;
    }

    dll->last_element->dll_next = output;
    output->dll_prev = dll->last_element;
    output->dll_next = 0;
    dll->last_element = output;

    dll->count++;
    
    return output;
}

void DoubleLinkedList_Free(DoubleLinkedList *dll, DLL_Element *el)
{
    DAssert(dll);
    DAssert(el);

    if(el->dll_prev == 0)
    {
	dll->first_element = el->dll_next;
    }

    if(el->dll_next == 0)
    {
	dll->last_element = el->dll_prev;
    }

    if(el->dll_prev != 0)
	el->dll_prev->dll_next = el->dll_next;

    if(el->dll_next != 0)
	el->dll_next->dll_prev = el->dll_prev;
    
    MP_Free(&dll->mp, (MP_Element *)el);

    dll->count--;
}

void DoubleLinkedList_DestroyList(DoubleLinkedList *dll)
{
    DAssert(dll);

    MP_DestroyMemoryPool(&dll->mp);
    dll->first_element = 0;
    dll->last_element = 0;
    dll->count = 0;
}

DLL_Element *DoubleLinkedList_GetNext(DLL_Element *el)
{
    return el->dll_next;
}

DLL_Element *DoubleLinkedList_GetPrev(DLL_Element *el)
{
    return el->dll_prev;
}
