#include "string_functions.h"

bool STR_Copy(char *dest, char *src)
{
    DAssert(dest);
    DAssert(src);

    int count = 0;

    while(*src != 0)
    {
	*dest = *src;
	dest++;
	src++;

	count++;
	if(count >= 65536)
	    break;
    }

    // HACK(daniel): to make the string zero terminated, probably wrong doing this here like that
    *dest = 0;

    return true;
}

uint32 STR_Length(char *str)
{
    DAssert(str);
    
    uint32 output = 0;

    while(*str)
    {
	output++;
	str++;

	if(output >= 4096)
	    break;
    }

    return output;
}
