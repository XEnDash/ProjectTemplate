#pragma once

#include "defines.h"

#define ERROR_ID_NOT_AN_ERROR 0
#define ERROR_ID_SYSTEM 1
#define ERROR_ID_FILE 2

struct Error
{
    uint32 id;
    char *file;
    uint32 line;
    char *description;
    bool32 critical;

    bool32 Result() { if(id == ERROR_ID_NOT_AN_ERROR) return true; else return false; }
};

#define Succeded(s) s.Result()
#define Failed(s) !s.Result()

Error Error_CreateNotAnError();
Error Error_Create(uint32 id, char *file, uint32 line, char *description, bool32 critical);
Error Error_CreateWithoutDescription(uint32 id, char *file, uint32 line);
Error Error_CreateCritical(uint32 id, char *file, uint32 line, char *description);

#define MakeError(id, description) Error_Create(id, __FILE__, __LINE__, description, false)
#define MakeNotAnError() Error_CreateNotAnError()
#define MakeCriticalError(id, description) Error Error_CreateCritical(id, __FILE__, __LINE__, description, true);
