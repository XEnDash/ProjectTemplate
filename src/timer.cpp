#include "timer.h"

#include "app.h"

void Timer_Start(Timer *t)
{
    DAssert(t);
    
    t->begin_time = App_GetTicks();
}

double Timer_GetTimeSinceStart(Timer *t)
{
    DAssert(t);
    
    return App_GetTimeDifference(t->begin_time, App_GetTicks());
}

void Timer_Restart(Timer *t)
{
    DAssert(t);

    t->begin_time = App_GetTicks();
}
