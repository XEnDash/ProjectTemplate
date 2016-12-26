#pragma once

#include "defines.h"

#define STRING_COUNTING_LIMIT 65536 // in case the input is bad we prevent infinite loops after this much counts

struct String
{
    char *c_str;
    uint32 length;

    String(char *str);

    static uint32 CalcLength(char *str);
    static bool Copy(char *dest, char *src);
    static bool CopyLim(char *dest, char *src, uint32 size);
};
