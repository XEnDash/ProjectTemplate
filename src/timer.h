#pragma once

#include "defines.h"

struct Timer
{
    uint64 begin_time;
};

void Timer_Start(Timer *t);
double Timer_GetTimeSinceStart(Timer *t);
void Timer_Restart(Timer *t);
