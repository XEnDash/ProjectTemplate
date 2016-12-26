#pragma once

#include "app.h"
#include "defines.h"
#include "renderer.h"
#include "sprite.h"
#include "texture.h"
#include "linked_list.h"
#include "double_linked_list.h"
#include "sstring.h"

struct GameData
{
    DoubleLinkedList texture_list;
    Texture *tex;
    char *tex_filename;

    //MemoryPool mp;
    DoubleLinkedList sprite_list;
    Sprite *test;

    String str;
};

bool InitGameData(AppData *ad, GameData *gd);
void UpdateGameData(AppData *ad, GameData *gd);
void DeleteGameData(AppData *ad, GameData *gd);
