#pragma once

#include "defines.h"

#define LOG_FILE_NAME "log.txt"

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_SIMPLE 1
#define LOG_LEVEL_VERBOSE 2

bool Log_Init(uint32 level);
void Log_SetLevel(uint32 level);
void SLog(const char *s, char *file, uint32 line);
void FLog(const char *s, ...);
void UpdateLogDisplay();
bool DumpLogToFile();

#define Log(s) SLog(s, __FILE__, __LINE__)

#define DebugOutputLog(s) OutputDebugString(s)
