#include "sstring.h"

String::String(char *str)
{
    DAssert(str);

    this->length = CalcLength(str);

    this->c_str = (char *)lnew((sizeof(char) * this->length) + 1);
    
    if(!CopyLim(this->c_str, str, this->length))
    {
	LogError("String::CopyLim failed");
    }
}

uint32 String::CalcLength(char *str)
{
    DAssert(str);
    
    uint32 count = 0;

    char *p = str;
    while(*p != 0)
    {
	p++;
	count++;

	if(count >= STRING_COUNTING_LIMIT)
	{
	    count = 0;
	    break;
	}
    }

    return count;
}

bool String::Copy(char *dest, char *src)
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
	if(count >= STRING_COUNTING_LIMIT)
	    break;
    }

    // NOTE(daniel): make sure it is zero terminated
    *dest = 0;

    return true;
}

bool String::CopyLim(char *dest, char *src, uint32 size)
{
    DAssert(dest);
    DAssert(src);

    int count = 0;

    for(uint32 i = 0; i < size; i++)
    {
	*dest = *src;
	dest++;
	src++;

	count++;
	if(count >= STRING_COUNTING_LIMIT)
	    break;
    }

    // NOTE(daniel): make sure it is zero terminated
    *dest = 0;
    
    return true;
}
