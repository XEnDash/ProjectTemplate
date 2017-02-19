#include "defines.h"

#include "import_std.h"

String::String()
{
    this->length = 0;
    this->c_str = 0;
    this->allocated = false;
}

String::~String()
{
    if(this->allocated)
    {
	this->Deallocate();
    }

    this->length = 0;
    this->c_str = 0;
    this->allocated = false;
}

String::String(const String &s)
{
    this->length = 0;
    this->c_str = 0;
    this->allocated = false;
    
    if(!this->Assign(s.c_str))
    {
	LogError("String::Assign failed");
    }
}

String::String(char *str)
{
    this->length = 0;
    this->c_str = 0;
    this->allocated = false;
    
    if(!this->Assign(str))
    {
	LogError("String::Assign failed");
    }
}

String::String(int64 i)
{
    this->length = 0;
    this->c_str = 0;
    this->allocated = false;

    uint32 size = GetNumberOfCharactersInNumber(i);
    
    if(!this->Allocate(size))
    {
	LogError("String::Allocate failed");
    }
    
    ParseIntIntoCStr(this->c_str, i);
    this->length = CalcLength(this->c_str);
}

bool32 String::Allocate(uint32 size)
{
    DAssert(size);

    if(this->allocated)
    {
	ldelete(this->c_str);
	this->length = 0;
	
	this->allocated = false;
    }

    this->c_str = (char *)lnew((sizeof(char) * size) + 1);

    if(!this->c_str)
    {
	this->allocated = false;
	return false;
    }

    this->allocated = true;

    return true;
}

bool32 String::Reallocate(uint32 size)
{
    DAssert(size);

    if(!this->allocated)
    {
	this->Allocate(size);
	return true;
    }

    this->c_str = (char *)lrealloc(this->c_str, (sizeof(char) * size + 1));

    if(!this->c_str)
    {
	this->allocated = false;
	return false;
    }

    this->allocated = true;

    return true;
}

bool32 String::Deallocate()
{
    DAssert(this->allocated);
    DAssert(this->c_str);

    ldelete(this->c_str);
    this->c_str = 0;
    
    this->length = 0;
    this->allocated = false;

    return true;
}

void String::operator =(char *str)
{
    this->length = 0;
    this->c_str = 0;
    this->allocated = false;
    
    if(!this->Assign(str))
    {
	LogError("String::Assign failed");
    }
}

void String::operator =(int64 i)
{
    this->length = 0;
    this->c_str = 0;
    this->allocated = false;

    uint32 size = GetNumberOfCharactersInNumber(i);
    
    if(!this->Allocate(size))
    {
	LogError("String::Allocate failed");
    }
    
    ParseIntIntoCStr(this->c_str, i);
    this->length = CalcLength(this->c_str);
}

void String::operator +=(char *str)
{
    if(!this->Append(str))
    {
	LogError("String::Append failed");
    }
}

void String::operator +=(int64 i)
{
    uint32 prev_len = this->length;

    uint32 size = GetNumberOfCharactersInNumber(i);
    
    if(!this->Reallocate(prev_len + size))
    {
	LogError("String::Reallocate failed");
    }
    
    ParseIntIntoCStr(&this->c_str[prev_len], i);
    this->length = CalcLength(this->c_str);
}

String String::operator +(char *str)
{
    if(!this->Append(str))
    {
	LogError("String::Append failed");
    }

    return *this;
}
 
String String::operator +(int64 i)
{
    uint32 prev_len = this->length;

    uint32 size = GetNumberOfCharactersInNumber(i);
    
    if(!this->Reallocate(prev_len + size))
    {
	LogError("String::Reallocate failed");
    }
    
    ParseIntIntoCStr(&this->c_str[prev_len], i);
    this->length = CalcLength(this->c_str);

    return *this;
}

bool32 String::Assign(char *str)
{
    DAssert(str);

    uint32 new_len = CalcLength(str);
    
    if(!this->Allocate(CalcLength(str)))
    {
	LogError("String::Allocate failed");
	return false;
    }
    
    if(!CopyLim(this->c_str, str, new_len))
    {
	LogError("String::CopyLim failed");
	return false;
    }

    this->length = CalcLength(this->c_str);

    return true;
}

bool32 String::Append(char *str)
{
    DAssert(str);

    if(!this->allocated)
    {
	LogError("Appending non allocated string");
	return false;
    }

    uint32 prev_len = this->Length();
    uint32 append_len = CalcLength(str);
    uint32 new_len = prev_len + append_len + 1;

    this->c_str = (char *)lrealloc(this->c_str, new_len);

    if(!this->c_str)
    {
	LogError("Unable to reallocate string");
	return false;
    }
    
    if(!CopyLim(&this->c_str[prev_len], str, append_len))
    {
	LogError("String::CopyLim failed");
	return false;
    }

    this->length = CalcLength(this->c_str);

    return true;
}

uint32 String::Length()
{
    return this->length;
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

String String::ParseInt(int64 i)
{
    bool negative = (i < 0);
    uint32 size = GetNumberOfCharactersInNumber(i);

    String output;
    output.Allocate(size + 1);

    if(!output.allocated)
    {
	LogError("String::Allocate failed");
    }

    char *ptr = output.c_str;
    
    if(negative)
    {
	ptr[0] = '-';
	size -= 1;
	ptr += 1;

	i = -i;
    }

    for(uint32 j = size; j > 0; j--)
    {
	int power = pow((float)(10), (float)(j - 1));
	uint32 n = i / power;
	ptr[size - j] = n + 0x30;

	i -= n * power;
    }

    if(negative)
	size += 1;

    output.length = size;
    output.c_str[output.length] = 0;

    return output;
}

void String::ParseIntIntoCStr(char *c_str, int64 i)
{
    DAssert(c_str);
    
    bool negative = (i < 0);
    uint32 size = GetNumberOfCharactersInNumber(i);

    char *output = c_str;
    char *ptr = c_str;
    
    if(negative)
    {
	ptr[0] = '-';
	size -= 1;
	ptr += 1;

	i = -i;
    }

    for(uint32 j = size; j > 0; j--)
    {
	int power = pow((float)(10), (float)(j - 1));
	uint32 n = i / power;
	ptr[size - j] = n + 0x30;

	i -= n * power;
    }

    if(negative)
	size += 1;

    output[size] = 0;
}
