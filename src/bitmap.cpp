#include "bitmap.h"

#include "app.h"
#include "file.h"
#include "string_functions.h"

bool BMP_Load(Bitmap *bmp, uint8 *buffer, uint64 size, char filename[64])
{
    DAssert(bmp);
    DAssert(buffer);
    
    BMP_FileHeader *file_header = (BMP_FileHeader *)buffer;
    if(CheckBMPFileHeader(*file_header, size))
    {
	uint16 bitmap_header_size = *(buffer + 14);

	int ver = GetBMPVersion(bitmap_header_size);

	if(ver == -1)
	{
	    return false;
	}
	    
	if(ver == BMP_VER2)
	{
	    // TODO(daniel): support this?
	    return false;
	}

	if(ver == BMP_VER3)
	{
	    BMP_BitmapHeaderV3 *bitmap_header = (BMP_BitmapHeaderV3 *)(buffer + 14);

	    STR_Copy(bmp->filename, filename);
	    bmp->width = bitmap_header->width;
	    bmp->height = bitmap_header->height;
	    bmp->pixels = (uint32 *)lnew(sizeof(uint32) * bmp->width * bmp->height);

	    for(int i = 0; i < bmp->width * bmp->height; i++)
	    {
		uint8 r = buffer[54 + i * 3];
		uint8 g = buffer[54 + i * 3 + 1];
		uint8 b = buffer[54 + i * 3 + 2];

		bmp->pixels[i] = (r<<16)|(g<<8)|(b);
	    }

	    // HACK(daniel): vertical flipping image because OpenGL flips the image internally
#if 0
	    for(int x = 0; x < bmp->width; x++)
	    {
		for(int y = 0; y < bmp->height; y++)
		{
		    uint32 temp = bmp->pixels[x + y * bmp->width];
		    bmp->pixels[x + y * bmp->width] = bmp->pixels[(bmp->width - x) + y * bmp->width];
		    bmp->pixels[(bmp->width - x) + y * bmp->width] = temp;
		}
	    }
#endif
	    return true;
	}
    }
    else
    {
	return false;
    }

    return false;
}

bool CheckBMPFileHeader(BMP_FileHeader header, int size)
{
    if(header.file_type != 0x4D42) // BM
	return false;
    //if(header.file_size != size)
    //  return false;

    return true;
}

int GetBMPVersion(uint16 bitmap_header_size)
{
    switch(bitmap_header_size)
    {
	case 12:
	{
	    return BMP_VER2;
	} break;
	case 40:
	{
	    return BMP_VER3;
	} break;
    }

    return -1;
}

void BMP_Free(Bitmap *bmp)
{
    DAssert(bmp);
    
    ldelete(bmp->pixels);
    bmp->pixels = 0;
}

bool BMP_SaveAsTXD(Bitmap *bmp, char *filename)
{
    File f;
    if(!FILE_OpenAndClear(&f, filename, OPEN_MODE_ALWAYS))
	return false;

    uint32 buffer_size = 8 + bmp->width * bmp->height * 4;
    uint8 *buffer = (uint8 *)lnew(buffer_size);

    *(int32 *)(&buffer[0]) = bmp->width;
    *(int32 *)(&buffer[4]) = bmp->width;

    for(int i = 0; i < bmp->width * bmp->height; i++)
    {
	*(int32 *)(&buffer[8 + i * 4]) = bmp->pixels[i];
    }
    
    if(!FILE_Write(&f, buffer, buffer_size))
	return false;

    FILE_Close(&f);

    return true;
}
