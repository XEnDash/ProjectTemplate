#pragma once

#include "defines.h"
#include "app.h"
#include "vector2.h"
#include "bitmap.h"

struct RendererInitData;
struct Texture;
struct Sprite;

bool R_Init(RendererInitData *rid);

void R_Begin();
void R_End();

void R_Sprite(Sprite *s, Color c);

void R_Resize(uint16 win_w, uint16 win_h, uint16 scr_w, uint16 scr_h);

// NOTE(daniel): pixel format is expected to be RGBA, support different formats later?
bool R_LoadTexture(Texture **t, uint32 w, uint32 h, void *pixels);

void R_UnloadTexture(Texture *t);
void R_SetTexture(Texture *t);

void R_EnableVSync();
void R_DisableVSync();
bool32 R_IsVSyncExtensionsAvilable();
bool32 R_IsVSyncEnabled();
