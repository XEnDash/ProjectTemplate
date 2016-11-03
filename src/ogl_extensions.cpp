#include "ogl_extensions.h"

BOOL WINAPI PFNWGLSWAPINTERVALEXTPROC_empty(int interval)
{
    static bool PFNWGLSWAPINTERVALEXTPROC_empty_to_print_message = true;
    if(!PFNWGLSWAPINTERVALEXTPROC_empty_to_print_message)
    {
	LogError("(!!!) Using Empty Function for wglSwapIntervalEXT (!!!)");
	PFNWGLSWAPINTERVALEXTPROC_empty_to_print_message = false;
    }
    return false;
}

int WINAPI PFNWGLGETSWAPINTERVALEXTPROC_empty(void)
{
    static bool PFNWGLGETSWAPINTERVALEXTPROC_empty_to_print_message = true;
    if(!PFNWGLGETSWAPINTERVALEXTPROC_empty_to_print_message)
    {
	LogError("(!!!) Using Empty Function for wglGetSwapIntervalEXT (!!!)");
	PFNWGLGETSWAPINTERVALEXTPROC_empty_to_print_message = false;
    }
    
    return 0;
}

PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = 0;
PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = 0;

bool LoadOpenGLExtensions()
{
    bool result = true;
    
    // TODO(daniel): this code is really terrible, spend the time making this good
    char *ext_str = "wglSwapIntervalEXT";
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress(ext_str);
    if(wglSwapIntervalEXT == 0)
    {
	FLog("failed to load OpenGL extension: %s", ext_str);
	wglSwapIntervalEXT = PFNWGLSWAPINTERVALEXTPROC_empty;
	result = false;
    }

    ext_str = "wglGetSwapIntervalEXT";
    wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress(ext_str);
    if(wglGetSwapIntervalEXT == 0)
    {
	FLog("failed to load OpenGL extension: %s", ext_str);
	wglGetSwapIntervalEXT = PFNWGLGETSWAPINTERVALEXTPROC_empty;
	result = false;
    }

    return result;
}
