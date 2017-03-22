#pragma once

#include "defines.h"
#include "vector2.h"
#include "double_linked_list.h"

struct Sprite
{
    DLL_Element dll_element;
    V2 pos;
    V2 scl;
    float rot;
    bool visible;
};

struct SpriteList
{
    DoubleLinkedList dll;
};

Sprite *Sprite_Allocate(SpriteList *list);
Sprite *Sprite_Allocate(SpriteList *list, V2 pos);
Sprite *Sprite_Allocate(SpriteList *list, V2 pos, V2 scl);
Sprite *Sprite_Allocate(SpriteList *list, V2 pos, V2 scl, float rot);
Sprite *Sprite_Allocate(SpriteList *list, V2 pos, V2 scl, float rot, bool visible);
void Sprite_Free(SpriteList *list, Sprite *s);

bool32 Sprite_AllocateList(SpriteList *list, uint32 max_objects);
void Sprite_DeallocateList(SpriteList *list);

Sprite *Sprite_GetFirst(SpriteList *list);
Sprite *Sprite_GetLast(SpriteList *list);
Sprite *Sprite_GetNext(Sprite *s);
Sprite *Sprite_GetPrev(Sprite *s);

