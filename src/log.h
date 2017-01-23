#pragma once

#include "defines.h"

#define LOG_FILE_NAME "log.txt"

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_SIMPLE 1
#define LOG_LEVEL_VERBOSE 2
#define LOG_LEVEL_EVERYTHING 3

//#define LOG_ALLOCATIONS

bool Log_Init(uint32 level);
void Log_SetLevel(uint32 level);
void SLog(const char *s, char *file, uint32 line, bool print_meta);
void FLog(const char *s, ...);
void Log_BeginSection(const char *s);
void Log_EndSection();
void UpdateLogDisplay();
bool DumpLogToFile();

void Log_Disable();
void Log_Enable();

#define Log(s) SLog(s, __FILE__, __LINE__, false)
#define LogError(s) SLog(s, __FILE__, __LINE__, true);

// NOTE(daniel): will make the game loop synchronization wrong
#define OUTPUT_DEBUG_TO_VS_LOG

#ifdef OUTPUT_DEBUG_TO_VS_LOG
// NOTE(daniel): this takes WAAAAY to log
#define DebugOutputLog(s) OutputDebugString(s)
#else
#define DebugOutputLog(s)
#endif
