#include "defines.h"

#include "vector2.h"
#include "vector3.h"

const Color Color::white = { 255, 255, 255 };
const Color Color::black = { 0, 0, 0 };
const Color Color::red = { 255, 0, 0 };
const Color Color::green = { 0, 255, 0 };
const Color Color::blue = { 0, 0, 255 };

const V2 V2::Zero = { 0, 0 };
const V3 V3::Zero = { 0, 0, 0 };

uint32 GetNumberOfCharactersInNumber(int64 i)
{
    uint32 size;
    if(i > 0)
	size = ceil(log10((float)(i)));
    else
	size = ceil(log10((float)(-i)));

    if(i == 0 || i == 1 || i == -1)
	size = 1;

    if(i == 10 || i == -10)
	size = 2;

    if(i < 0)
	size += 1;

    return size;
}
