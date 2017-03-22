#include "game.h"

#include "log.h"
#include "file.h"
#include "sprite.h"
#include "bitmap.h"

#define DEBUG_SPRITE_LIST_MAX 32 // 1024*64

bool G_LoadTexture(Texture **tex, char *filename)
{
    DAssert(filename);
    
    File f;
    if(!FILE_Open(&f, filename, OPEN_MODE_EXISTING))
	return false;

    uint8 *buffer = (uint8 *)lnew(f.size);
    
    if(!FILE_Read(&f, buffer))
	return false;

    /*uint32 width = *(uint32 *)(&buffer[0]);
    uint32 height = *(uint32 *)(&buffer[4]);
    void *pixels = &buffer[8];*/

    Bitmap bmp;
    if(!BMP_Load(&bmp, buffer, f.size, filename))
    {
	return false;
    }

    uint32 width = bmp.width;
    uint32 height = bmp.height;
    void *pixels = bmp.pixels;
    
    if(!R_LoadTexture(tex, width, height, pixels))
    {
	LogError("R_LoadTexture failed...");
	return false;
    }

    BMP_Free(&bmp);

    ldelete(buffer);
    buffer = 0;
    
    FILE_Close(&f);

    return true;
}

void G_UnloadTexture(Texture **tex)
{
    if(*tex == 0)
	return;
    
    R_UnloadTexture(*tex);
}

bool InitGameData(AppData *ad, GameData *gd)
{
    DAssert(ad);
    DAssert(gd);

    if(!DoubleLinkedList_Init(&gd->texture_list, GetSizeOfTextureStruct(), 512))
	return false;
    
    gd->tex_filename = "../data/test.bmp";
    gd->tex = (Texture *)DoubleLinkedList_GetNew(&gd->texture_list);
    DAssert(gd->tex);

    if(!G_LoadTexture(&gd->tex, gd->tex_filename))
    {
	FLog("Failed to load texture: \"%s\"", gd->tex_filename);
	//return false;
    }

    if(!Sprite_AllocateList(&gd->sprite_list, DEBUG_SPRITE_LIST_MAX))
	return false;

    for(int i = 0; i < DEBUG_SPRITE_LIST_MAX + 10; i++)
    {
	Sprite *s = Sprite_Allocate(&gd->sprite_list);
	if(s == 0)
	    break;
	
	s->pos = V2(App_Random() % 800, App_Random() % 600);
	s->scl = V2(100, 100);
	s->rot = App_Random() % 360;
    }

    Sprite_Free(&gd->sprite_list, Sprite_GetLast(&gd->sprite_list));
    
    //gd->test = (Sprite *)LinkedList_GetNew(&gd->sprite_list);
    gd->test = Sprite_GetLast(&gd->sprite_list);
    DAssert(gd->test);
    
    gd->test->pos = V2(400, 300);
    gd->test->scl = V2(250, 250);
    gd->test->rot = 0;
    
    return true;
}

void UpdateGameData(AppData *ad, GameData *gd)
{
    DAssert(ad);
    DAssert(gd);

    // NOTE(daniel): testing per frame freeing and allocating all sprites
#if 0
    while(gd->sprite_list.last_element)
    {
	Sprite_Free(&gd->sprite_list, gd->sprite_list.last_element);
    }

    for(int i = 0; i < DEBUG_SPRITE_LIST_MAX + 10; i++)
    {
	Sprite *s = Sprite_Allocate(&gd->sprite_list);
	if(s == 0)
	    break;
	
	s->pos = V2(App_Random() % 800, App_Random() % 600);
	s->scl = V2(100, 100);
	s->rot = App_Random() % 360;
    }
#endif

    gd->test = Sprite_GetLast(&gd->sprite_list);
    
    float dt = ad->dt;

    Gamepad gp = ad->gamepad;
    Gamepad gpp = ad->gamepad_prev;

    Sprite *s = gd->test;

    float speed = 1000.0f * dt;
    if(gp.up)
    {
	s->pos.y += speed;
    }
    if(gp.down)
    {
	s->pos.y -= speed;
    }
    if(gp.left)
    {
	s->pos.x -= speed;
    }
    if(gp.right)
    {
	s->pos.x += speed;
    }

    float scl_speed = 1000.0f * dt;
    if(gp.w)
    {
	s->scl.y += scl_speed;
    }
    if(gp.s)
    {
	s->scl.y -= scl_speed;
    }
    if(gp.a)
    {
	s->scl.x -= scl_speed;
    }
    if(gp.d)
    {
	s->scl.x += scl_speed;
    }

    static bool flag = false;

    float rot_speed = 90.0f * dt;
    if(gp.space && !gpp.space)
    {
	//s->rot -= rot_speed;
	flag = true;
    }

    if(flag)
    {
	s->rot -= rot_speed;
	if(s->rot <= -360)
	{
	    s->rot = 0;
	    flag = false;
	}
    }
    
    if(gp.r && !gpp.r)
    {
	Log_BeginSection("=== Reloading texture ===");
	G_UnloadTexture(&gd->tex);
	if(!G_LoadTexture(&gd->tex, gd->tex_filename))
	{
	    FLog("Failed to load texture: %s", gd->tex_filename);
	    //ad->program_state = 0;
	}
	//Log("=== Done reloading texture ===");
	Log_EndSection();
    }

    if(ad->gamepad.v && !ad->gamepad_prev.v)
    {
	if(!R_IsVSyncExtensionsAvilable())
	{
	    Log("VSync Unavailable!");
	}
	else
	{
	    if(R_IsVSyncEnabled())
	    {
		R_DisableVSync();
	    }
	    else
	    {
		R_EnableVSync();
	    }

	    // NOTE(daniel): this gets the answer from the driver, which is why we call the function twice
	    if(R_IsVSyncEnabled())
	    {
		Log("VSync Enabled!");
	    }
	    else
	    {
		Log("VSync Disabled!");
	    }
	}
    }

    ad->gamepad_prev = ad->gamepad;
}

void DeleteGameData(AppData *ad, GameData *gd)
{
    if(gd)
    {
	Sprite_DeallocateList(&gd->sprite_list);
    
	G_UnloadTexture(&gd->tex);
	DoubleLinkedList_DestroyList(&gd->texture_list);
    }
}
