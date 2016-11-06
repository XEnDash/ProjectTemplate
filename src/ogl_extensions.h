#pragma once

#include "windows.h"
#include "gl/gl.h"
#include "wglext.h"

#include "defines.h"

extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
extern PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;

bool32 LoadOpenGLExtensions();
bool32 IsOGLExtensionAvilable(const char *str);
