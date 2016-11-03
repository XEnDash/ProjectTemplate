#include "renderer.h"

#include <windows.h>
#include <gl/gl.h>
#include "ogl_extensions.h"
#include "log.h"
#include "sprite.h"
#include "texture.h"
#include "linked_list.h"

struct Texture //NOTE(daniel): must be the same as in renderer_win32_ogl.cpp
{
    LL_Element ll_element;
    uint32 gltex;
    bool32 loaded;
};

struct RendererInitData //NOTE(daniel): must be the same as in app_win32.cpp
{
    HDC dc;
    uint16 win_w, win_h;
    uint16 scr_w, scr_h;
};

bool R_Init(RendererInitData *rid)
{
    PIXELFORMATDESCRIPTOR pfd = { 
	sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd  
	1,                     // version number  
	PFD_DRAW_TO_WINDOW |   // support window  
	PFD_SUPPORT_OPENGL |   // support OpenGL  
	PFD_DOUBLEBUFFER,      // double buffered  
	PFD_TYPE_RGBA,         // RGBA type  
	32,                    // 24-bit color depth  
	0, 0, 0, 0, 0, 0,      // color bits ignored  
	0,                     // no alpha buffer  
	0,                     // shift bit ignored  
	0,                     // no accumulation buffer  
	0, 0, 0, 0,            // accum bits ignored  
	32,                    // 32-bit z-buffer  
	0,                     // no stencil buffer  
	0,                     // no auxiliary buffer  
	PFD_MAIN_PLANE,        // main layer  
	0,                     // reserved  
	0, 0, 0                // layer masks ignored  
    }; 

    int  pixel_format; 

    pixel_format = ChoosePixelFormat(rid->dc, &pfd);
    DescribePixelFormat(rid->dc, pixel_format, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
    SetPixelFormat(rid->dc, pixel_format, &pfd);

    HGLRC context = wglCreateContext(rid->dc);
    if(!context)
    {
	LogError("wglCreateContext failed.");
	return false;
    }
    
    if(!wglMakeCurrent(rid->dc, context))
    {
	LogError("wglMakeCurrent failed.");
	return false;
    }

    if(!LoadOpenGLExtensions())
    {
	LogError("failed to load OpenGL Extensions.");
	//return false;
    }
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho(0, rid->scr_w, 0, rid->scr_h, 0.0f, 1.0f);
    R_Resize(rid->win_w, rid->win_h, rid->scr_w, rid->scr_h);
    
    glEnable(GL_TEXTURE_2D);

    R_EnableVSync();
    //R_DisableVSync();
    
    return true;
}

void R_Begin()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void R_End()
{
    // TODO(daniel): it looks like SwapBuffers calls glFinish internally, so either glFinish stalls until gpu sync or SwapBuffers stalls until gpu sync
    // which is better? my guess (only a guess) is SwapBuffers
    //glFinish();
}

void R_Sprite(Sprite *s, Color c)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glLoadIdentity();
    glTranslatef(s->pos.x, s->pos.y, 0.0f);
    glRotatef(s->rot, 0.0f, 0.0f, 1.0f);
    glScalef(s->scl.x, s->scl.y, 0.0f);

    glBegin(GL_QUADS);

    glColor3f(c.r, c.g, c.b);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.0f);
    
    glEnd();

    glPopMatrix();
}

void R_Resize(uint16 win_w, uint16 win_h, uint16 scr_w, uint16 scr_h)
{
    glViewport(0, 0, win_w, win_h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, scr_w, 0, scr_h, 0.0f, 1.0f);
}

bool R_LoadTexture(Texture **t, uint32 w, uint32 h, void *pixels)
{
    DAssert(t);
    DAssert(*t);

    DAssert(w > 0);
    DAssert(h > 0);
    DAssert(pixels);

    (*t)->loaded = false;
    
    GLuint gltex = -1;
    
    glGenTextures(1, &gltex);

    if(gltex == -1)
    {
	GLenum  error = glGetError();
	FLog("glGenTextures failed: %u", error);
	ldelete(*t);
	*t = 0;
	return false;
    }

    glBindTexture(GL_TEXTURE_2D, gltex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    (*t)->gltex = gltex;
    (*t)->loaded = true;

    return true;
}

void R_UnloadTexture(Texture *t)
{
    DAssert(t);
    
    glDeleteTextures(1, &t->gltex);
}

void R_SetTexture(Texture *t)
{
    DAssert(t);
    
    glBindTexture(GL_TEXTURE_2D, t->gltex);
}

void R_EnableVSync()
{
    wglSwapIntervalEXT(1);
}

void R_DisableVSync()
{
    wglSwapIntervalEXT(0);
}

bool32 R_IsVSyncEnabled()
{
    uint32 swap_interval = wglGetSwapIntervalEXT();
    
    if(swap_interval == 0)
	return false;

    return true;
}
