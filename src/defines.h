#pragma once

#include "import_std.h"

typedef int8_t int8;
typedef uint8_t uint8;

typedef int16_t int16;
typedef uint16_t uint16;

typedef int32_t int32;
typedef uint32_t uint32;

typedef int64_t int64;
typedef uint64_t uint64;

typedef int32 bool32;

#define bool bool32
//#define int int32

#define KILOBYTE(x) ((uint64)(x) * (uint64)(1024))
#define MEGABYTE(x) KILOBYTE((uint64)(x) * (uint64)(1024))
#define GIGABYTE(x) MEGABYTE((uint64)(x) * (uint64)(1024))

#define STRING_COUNTING_LIMIT 65536 // in case the input is bad we prevent infinite loops after this much counts

struct String
{
    char *c_str;
    uint32 length;
    bool32 allocated;

    String();
    ~String();
    String(const String &s);
    String(char *str);
    String(int64 i);

    bool32 Allocate(uint32 size);
    bool32 Reallocate(uint32 size);
    bool32 Deallocate();

    operator char*() { return this->c_str; }

    void operator =(char *str);
    void operator =(int64 i);

    void operator +=(char *str);
    void operator +=(int64 i);

    String operator +(char *str);
    String operator +(int64 i);

    bool32 Assign(char *str); // NOTE(daniel): assignment implicitly allocates! adds one byte for zero termindated
    bool32 Append(char *str);
    uint32 Length();

    static uint32 CalcLength(char *str);
    static bool Copy(char *dest, char *src);
    static bool CopyLim(char *dest, char *src, uint32 size);
    static String ParseInt(int64 i);
    static void ParseIntIntoCStr(char *c_str, int64 i);
};

struct Color
{
public:
    int r, g, b;

    static const Color white;
    static const Color black;
    static const Color red;
    static const Color green;
    static const Color blue;
};

#define RGBtob32(r, g, b) ((r<<16) | (g<<8) | (b))

#define Shiftb32tob8(p) ((p[0]) | (p[1]) << 8 | (p[2] << 8) | (p[3] << 24))

#define min3(a, b, c) (fmin(fmin(a, b), c))
#define max3(a, b, c) (fmax(fmax(a, b), c))

#define Clamp(c, min, max) { if(c < min) c = min; if(c > max) c = max; }

uint32 GetNumberOfCharactersInNumber(int64 i);

#define M_PI 3.14159265359
#define M_PI_F 3.14159265359f

#define DEBUG_ASSERT

#ifdef DEBUG_ASSERT
#include "app.h" // for App_MessageBox
#include "log.h" // for all the log stuff
#include <stdio.h> // for sprintf

//#define DAssert(b) { if(b == 0) { App_MessageBox("Assertion", "Assertion"); (*(uint8 *)(0)) = 1; } }

#define DAssert(b) { if(b == 0) {		\
	    char buffer[1024]; \
	    sprintf(buffer, "file: %s | line: %i", __FILE__, __LINE__); \
	    FLog("Assertion: %s", buffer);		\
	    UpdateLogDisplay(); \
	    DumpLogToFile(); \
	    App_MessageBox(buffer, "Assertion"); \
	    (*(uint8 *)(0)) = 1; \
	} \
    }
#else
#define DAssert(b)
#endif

#define lnew(s) App_Allocate(s, __FILE__, __LINE__)
#define lrealloc(p, s) App_Reallocate(p, s, __FILE__, __LINE__)
#define ldelete(ptr) App_Free(ptr, __FILE__, __LINE__)

#define DISALLOW_NEW_AND_DELETE
#ifdef DISALLOW_NEW_AND_DELETE
#define new 0;DAssert(0);//, "new is disallowed..");
#define delete(p) DAssert(0);//, "delete is disallowed..");
#endif
