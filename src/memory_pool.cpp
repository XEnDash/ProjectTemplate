#include "memory_pool.h"

#include "app.h"

bool MP_InitBySize(MemoryPool *mp, uint32 element_size, uint32 full_size)
{
    DAssert(mp);
    DAssert(element_size);
    DAssert(full_size);

    mp->element_size = element_size;
    mp->full_size = full_size;
    mp->element_count = (full_size / element_size);

    mp->first_element = (MP_Element *)lnew(full_size);

    if(!mp->first_element)
	return false;

    mp->first_element->mp_free = true;
    //mp->first_element->prev = (MP_Element *)(-1);
    mp->first_element->mp_next = (MP_Element *)((uint8 *)(mp->first_element) + element_size);

    MP_Element *el = mp->first_element->mp_next;
    MP_Element *end = (MP_Element *)((uint8 *)(mp->first_element) + full_size);
    MP_Element *prev = mp->first_element;
    
    while((uint8 *)(el) < (uint8 *)(end))
    {
	//el->prev = prev;
	el->mp_next = (MP_Element *)((uint8 *)(el) + element_size);
	el->mp_free = true;
	
	prev = el;
	el = el->mp_next;
    }

    prev->mp_next = (MP_Element *)(0);

    mp->free_list = mp->first_element;
    
    return true;
}

bool MP_InitByCount(MemoryPool *mp, uint32 element_size, uint32 element_count)
{
    return MP_InitBySize(mp, element_size, element_size * element_count);
}

MP_Element *MP_Alloc(MemoryPool *mp)
{
    DAssert(mp);
    
    if(mp->free_list == (MP_Element *)(0))
	return (MP_Element *)(0);
    
    MP_Element *output = mp->free_list;
    mp->free_list = mp->free_list->mp_next;
    output->mp_free = false;

    return output;
}

void MP_Free(MemoryPool *mp, MP_Element *el)
{
    DAssert(mp);
    DAssert(el);

    el->mp_free = true;
    
    // no free elements
    if(mp->free_list == (MP_Element *)(0))
    {
	el->mp_next = (MP_Element *)(0);
	mp->free_list = el;
	//el->prev = mp->mp_next = (MP_Element *)(-1);
	return;
    }

    // more then one free element

    // NOTE(daniel): inserting the free element exactly AFTER THE FIRST FREE ELEMENT
    // because it does not require any searching
    
    el->mp_next = mp->free_list->mp_next;
    //el->prev = mp->free_list;
    
    //mp->free_list-mp_next->prev = el;
    mp->free_list->mp_next = el;
}

void MP_DestroyMemoryPool(MemoryPool *mp)
{
    DAssert(mp);
    
    ldelete(mp->first_element);

    mp->element_size = 0;
    mp->full_size = 0;
    mp->element_count = 0;
    mp->first_element = 0;
    mp->free_list = 0;
}
