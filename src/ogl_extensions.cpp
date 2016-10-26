#include "ogl_extensions.h"

PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = 0;
PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = 0;

void LoadOpenGLExtensions()
{
    // TODO(daniel): this code is really terrible, spend the time making this good
    char *ext_str = "wglSwapIntervalEXT";
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress(ext_str);
    if(wglSwapIntervalEXT == 0)
    {
	FLog("failed to load OpenGL extensions %s", ext_str);
    }

    ext_str = "wglGetSwapIntervalEXT";
    wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress(ext_str);
    if(wglGetSwapIntervalEXT == 0)
    {
	FLog("failed to load OpenGL extensions %s", ext_str);
    }
}
