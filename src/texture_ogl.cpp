#include "texture.h"

#include "linked_list.h"

struct Texture //NOTE(daniel): must be the same as in renderer_win32_ogl.cpp
{
    LL_Element ll_element;
    uint32 gltex;
    bool32 loaded;
};

uint32 GetSizeOfTextureStruct()
{
    return sizeof(Texture);
}
