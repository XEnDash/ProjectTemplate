#pragma once

#include "defines.h"

#define MAX_FILENAME_LENGTH 64

struct FileHandle;

struct File
{
    char filename[MAX_FILENAME_LENGTH];
    uint64 size;
    FileHandle *plat_handle;
};

enum ENUM_FILE_MODE { OPEN_MODE_EXISTING, OPEN_MODE_CREATE, OPEN_MODE_ALWAYS };

bool FILE_InitGlobalFilePool(uint32 max_file_count);
void FILE_DestroyGlobalFilePool();
bool FILE_Open(File *f, char *filename, ENUM_FILE_MODE mode);
bool FILE_OpenAndClear(File *f, char *filename, ENUM_FILE_MODE mode);
bool FILE_Read(File *f, void *buffer);
bool FILE_Write(File *f, void *buffer, uint32 size);
void FILE_Close(File *f);
bool FILE_Clear(File *f);
