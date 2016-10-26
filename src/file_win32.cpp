#include "file.h"

#include "windows.h"
#include "app.h"
#include "log.h"
#include "string_functions.h"
#include "memory_pool.h"

struct FileHandle : MP_Element
{
    HANDLE handle;
};

MemoryPool global_filehandle_pool;

bool FILE_InitGlobalFilePool(uint32 max_file_count)
{
    DAssert(max_file_count > 0);

    if(!MP_InitByCount(&global_filehandle_pool, sizeof(FileHandle), max_file_count))
	return false;

    return true;
}

void FILE_DestroyGlobalFilePool()
{
    MP_DestroyMemoryPool(&global_filehandle_pool);
}

bool FILE_Open(File *f, char *filename, ENUM_FILE_MODE mode)
{
    DAssert(f);
    
    f->plat_handle = (FileHandle *)MP_Alloc(&global_filehandle_pool);
    DAssert(f->plat_handle);

    DWORD create_disposition;

    switch(mode)
    {
	case OPEN_MODE_EXISTING:
	{
	    create_disposition = OPEN_EXISTING;
	} break;
	case OPEN_MODE_ALWAYS:
	{
	    create_disposition = OPEN_ALWAYS;
	} break;
	case OPEN_MODE_CREATE:
	{
	    create_disposition = CREATE_NEW;
	} break;

	default:
	{
	    create_disposition = OPEN_EXISTING;
	} break;
    }
    
    f->plat_handle->handle = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, create_disposition, FILE_ATTRIBUTE_NORMAL, 0);
    
    if(f->plat_handle->handle == INVALID_HANDLE_VALUE)
    {
	char message[4096];
	if(App_GetLastErrorMessage(message, 4096))
	{
	    MetaLog("CreateFile Error:");
	    FLog("%s", message);
	}

	ldelete(f->plat_handle);
	
	return false;
    }
    
    LARGE_INTEGER li = { 0 };

    if(!GetFileSizeEx(f->plat_handle->handle, &li))
    {
	char message[4096];
	if(App_GetLastErrorMessage(message, 4096))
	{
	    MetaLog("GetFileSizeEx Error:");
	    FLog("%s", message);
	}
	
	return false;
    }
    
    f->size = li.QuadPart;

    STR_Copy(f->filename, filename);

    return true;
}

bool FILE_OpenAndClear(File *f, char *filename, ENUM_FILE_MODE mode)
{
    bool output = FILE_Open(f, filename, mode);
    FILE_Clear(f);
    return output;
}

bool FILE_Read(File *f, void *buffer)
{
    DAssert(f);
    
    if(f->plat_handle->handle == INVALID_HANDLE_VALUE)
    {
	FLog("FILE_Read error, file handle invalid.");
	return false;
    }
    
    DWORD bytes_read = 0;
    if(!ReadFile(f->plat_handle->handle, buffer, f->size, &bytes_read, 0))
    {
	char message[4096];
	if(App_GetLastErrorMessage(message, 4096))
	{
	    MetaLog("FILE_Read Error:");
	    FLog("%s", message);
	}
	
	return false;
    }

    return true;
}


bool FILE_Write(File *f, void *buffer, uint32 size)
{
    DAssert(f);
    
    if(f->plat_handle->handle == INVALID_HANDLE_VALUE)
    {
	MetaLog("FILE_Write error, file handle invalid.");
	return false;
    }

    DWORD bytes_written = 0;
    if(!WriteFile(f->plat_handle->handle, buffer, size, &bytes_written, 0))
    {
	char message[4096];
	if(App_GetLastErrorMessage(message, 4096))
	{
	    MetaLog("FILE_Write Error:");
	    FLog("%s", message);
	}
	
	return false;
    }

    return true;
}

void FILE_Close(File *f)
{
    DAssert(f);
    
    if(f->plat_handle->handle == INVALID_HANDLE_VALUE)
    {
	char message[4096];
	if(App_GetLastErrorMessage(message, 4096))
	{
	    MetaLog("FILE_Close Error:");
	    FLog("%s", message);
	}

	return;
    }
    
    CloseHandle(f->plat_handle->handle);

    MP_Free(&global_filehandle_pool, f->plat_handle);
    f->plat_handle = 0;
}

bool FILE_Clear(File *f)
{
    DAssert(f);

    if(f->plat_handle->handle == INVALID_HANDLE_VALUE)
    {
	MetaLog("FILE_Clear error, file handle invalid.");
	return false;
    }

    if(SetEndOfFile(f->plat_handle->handle) == 0)
    {
	char message[4096];
	if(App_GetLastErrorMessage(message, 4096))
	{
	    MetaLog("FILE_Clear Error:");
	    FLog("%s", message);
	}

	
	return false;
    }

    return true;
}
