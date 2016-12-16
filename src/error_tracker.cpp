#include "error_tracker.h"

Error Error_CreateNotAnError()
{
    Error output = { 0 };
    output.id = ERROR_ID_NOT_AN_ERROR;
    
    return output;
}

Error Error_Create(uint32 id, char *file, uint32 line, char *description, bool32 critical)
{
    Error output = { 0 };
    output.id = id;
    output.file = file;
    output.line = line;
    output.description = description;
    output.critical = critical;

    return output;
}

Error Error_CreateWithoutDescription(uint32 id, char *file, uint32 line)
{
    return Error_Create(id, file, line, 0, false);
}

Error Error_CreateCritical(uint32 id, char *file, uint32 line, char *description)
{
    return Error_Create(id, file, line, description, true);
}
