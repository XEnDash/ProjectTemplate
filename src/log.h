#pragma once

#include "defines.h"

#define LOG_FILE_NAME "log.txt"

bool Log_Init();
void SLog(const char *s, char *file, uint32 line, bool32 printmeta);
void FLog(const char *s, ...);
void UpdateLogDisplay();
bool DumpLogToFile();

#define Log(s) SLog(s, __FILE__, __LINE__, false)
#define MetaLog(s) SLog(s, __FILE__, __LINE__, true)

#define DebugOutputLog(s) OutputDebugString(s)
