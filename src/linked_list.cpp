#include "linked_list.h"

bool LinkedList_Init(LinkedList *ll, uint32 element_size, uint32 max_elements)
{
    DAssert(ll);
    DAssert(max_elements > 0);

    ll->first_element = 0;
    ll->last_element = 0;
    ll->count = 0;

    if(!MP_InitByCount(&ll->mp, element_size, max_elements))
        return false;
    
    return true;
}

void *LinkedList_GetNew(LinkedList *ll)
{
    DAssert(ll);
    
    LL_Element *output = (LL_Element *)MP_Alloc(&ll->mp);
    if(output == 0)
	return 0;

    if(ll->count == 0)
    {
	ll->first_element = output;
	ll->last_element = output;
	output->ll_next = 0;

	ll->count++;
	
	return output;
    }

    ll->last_element->ll_next = output;
    ll->last_element = output;
    output->ll_next = 0;
    
    ll->count++;
    
    return output;
}

void LinkedList_Free(LinkedList *ll, LL_Element *el)
{
    DAssert(ll);
    DAssert(el);

    if(ll->first_element == el)
    {
	ll->first_element = el->ll_next;
    }
    else if(ll->last_element == el)
    {
	// NOTE(daniel): i can't figure out a way to find the previous last element without searching

	LL_Element *ptr = ll->first_element;
	LL_Element *prev_ptr = ptr;
	while(ptr != el && ptr != 0)
	{
	    prev_ptr = ptr;
	    ptr = ptr->ll_next;
	}

	if(prev_ptr != 0)
	{
	    ll->last_element = prev_ptr;
	    ll->last_element->ll_next = 0;
	}
	else
	{
	    ll->last_element = 0;
	}
    }
    
    MP_Free(&ll->mp, (MP_Element *)el);

    ll->count--;
}

void LinkedList_DestroyList(LinkedList *ll)
{
    DAssert(ll);

    MP_DestroyMemoryPool(&ll->mp);
    ll->first_element = 0;
    ll->last_element = 0;
    ll->count = 0;
}

LL_Element *LinkedList_GetNext(LL_Element *el)
{
    return el->ll_next;
}
