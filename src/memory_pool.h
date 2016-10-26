#pragma once

#include "defines.h"

struct MP_Element
{
    MP_Element *mp_next;
    bool mp_free;
}; // CHECK(daniel): struct size is 12 bytes, but debugger shows it as 16 bytes, is the compiler doing anything to this struct?

struct MemoryPool
{
    uint32 element_size;
    uint32 full_size;
    uint32 element_count;
    MP_Element *first_element;
    MP_Element *free_list;
};

// NOTE(daniel): all memory is uninitialized, both MP_Alloc and MP_Free do not zero the previous data, always initialize the element after you allocated it!!!

bool MP_InitBySize(MemoryPool *mp, uint32 element_size, uint32 full_size);
bool MP_InitByCount(MemoryPool *mp, uint32 element_size, uint32 element_count);
MP_Element *MP_Alloc(MemoryPool *mp);
void MP_Free(MemoryPool *mp, MP_Element *el);
void MP_DestroyMemoryPool(MemoryPool *mp);
