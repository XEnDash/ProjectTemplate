#pragma once

#include "defines.h"

int32 min32(int32 a, int32 b)
{
    if(a < b)
	return a;
    else
	return b;
}

int32 max32(int32 a, int32 b)
{
    if(a > b)
	return a;
    else
	return b;
}
