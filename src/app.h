#pragma once

#include "defines.h"

struct Gamepad
{
    bool up, down, left, right;
    bool w, a, s, d;
    bool space;
    bool r, v;
};

struct AppData
{
    float dt;
    uint16 win_w, win_h;
    uint16 scr_w, scr_h;
    int32 program_state;

    Gamepad gamepad;
    Gamepad gamepad_prev;
};

void *App_Allocate(uint64 size, char *file, uint32 line);
void *App_Reallocate(void *ptr, uint64 size, char *file, uint32 line);
void App_Free(void *ptr, char *file, uint32 line);

void App_MessageBox(char *message, char *title);
bool App_GetLastErrorMessage(char *msg, uint32 size);

uint32 App_Random();
uint64 App_GetTicks();
double App_GetTimeDifference(uint64 start, uint64 end);
double App_GetTime();
void App_Sleep(uint64 ms);
