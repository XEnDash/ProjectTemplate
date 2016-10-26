#pragma once

#include "defines.h"

typedef struct
{
    char filename[64];
    uint16 width;
    uint16 height;
    uint32 *pixels;
} Bitmap;

bool BMP_Load(Bitmap *bmp, uint8 *buffer, uint64 size, char filename[64]);

typedef struct
{
    uint16 file_type;
    uint32 file_size;
    uint16 reserved1;
    uint16 reserved2;
    uint32 bitmap_offset;
} BMP_FileHeader;

bool CheckBMPFileHeader(BMP_FileHeader header, int size);

#define BMP_VER2 0x1
#define BMP_VER3 0x2
int GetBMPVersion(uint16 bitmap_header_size);

typedef struct
{
    uint32 header_size;
    uint16 width;
    uint16 height;
    uint16 planes;
    uint16 bitsperpixel;
} BMP_BitmapHeaderV2;

typedef struct
{
    uint32 header_size;
    uint32 width;
    uint32 height;
    uint16 planes;
    uint16 bitsperpixel;
    uint32 compression;
    uint32 size_of_bitmap;
    uint32 horz_res;
    uint32 vert_res;
    uint32 colors_used;
    uint32 colors_important;
} BMP_BitmapHeaderV3;

void BMP_Free(Bitmap *bmp);

bool BMP_SaveAsTXD(Bitmap *bmp, char *filename);
