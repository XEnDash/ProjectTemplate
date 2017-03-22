#include "sprite.h"

Sprite *Sprite_Allocate(SpriteList *list)
{
    return Sprite_Allocate(list, V2::Zero, V2(1, 1), 0, true);
}

Sprite *Sprite_Allocate(SpriteList *list, V2 pos)
{
    return Sprite_Allocate(list, pos, V2(1, 1), 0, true);
}

Sprite *Sprite_Allocate(SpriteList *list, V2 pos, V2 scl)
{
    return Sprite_Allocate(list, pos, scl, 0, true);
}

Sprite *Sprite_Allocate(SpriteList *list, V2 pos, V2 scl, float rot)
{
    return Sprite_Allocate(list, pos, scl, rot, true);
}

Sprite *Sprite_Allocate(SpriteList *list, V2 pos, V2 scl, float rot, bool visible)
{
    DAssert(list);
    Sprite *s = (Sprite *)DoubleLinkedList_GetNew(&list->dll);
    
    if(s == 0)
	return 0;
    
    s->pos = pos;
    s->scl = scl;
    s->rot = rot;
    s->visible = visible;
    
    return s;
}

void Sprite_Free(SpriteList *list, Sprite *s)
{
    DAssert(&list->dll);
    DAssert(s);
    DoubleLinkedList_Free(&list->dll, (DLL_Element *)s);
}

bool32 Sprite_AllocateList(SpriteList *list, uint32 max_objects)
{
    DAssert(list != 0);
    DAssert(max_objects > 0);
    
    if(!DoubleLinkedList_Init(&list->dll, sizeof(Sprite), max_objects))
	return false;

    return true;
}

void Sprite_DeallocateList(SpriteList *list)
{
    DoubleLinkedList_DestroyList(&list->dll);
}

Sprite *Sprite_GetFirst(SpriteList *list)
{
    return (Sprite *)list->dll.first_element; 
}

Sprite *Sprite_GetLast(SpriteList *list)
{
    return (Sprite *)list->dll.last_element; 
}

Sprite *Sprite_GetNext(Sprite *s)
{
    return (Sprite *)DoubleLinkedList_GetNext((DLL_Element *)s);
}

Sprite *Sprite_GetPrev(Sprite *s)
{
    return (Sprite *)DoubleLinkedList_GetPrev((DLL_Element *)s);
}
