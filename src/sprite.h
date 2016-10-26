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
};
