#pragma once

#include "defines.h"

struct MachineData
{
    uint32 oem_id;
    uint16 processor_architecture;
    uint32 page_size;
    void *minimum_application_address_ptr;
    void *maximum_application_address_ptr;
    uint32 *active_processor_mask_ptr;
    uint32 num_of_processors;
    uint32 processors_type;
    uint32 alloc_granularity;
    uint16 processor_level;
    uint16 processor_revision;
};

struct Gamepad
{
    bool up, down, left, right;
    bool w, a, s, d;
    bool space;
    bool r, v;
};

struct AppData
{
    MachineData md;
    
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
