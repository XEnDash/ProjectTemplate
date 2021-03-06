#include "app.h"

#include "import_win32.h"
#include "import_std.h"

#include "file.h"
#include "log.h"
#include "timer.h"
#include "renderer.h"
#include "sprite.h"
#include "game.h"

#define CLASS_NAME "GameWindowCN"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

AppData *ad;
GameData *gd;

struct RendererInitData //NOTE(daniel): must be the same as in renderer_win32_ogl.cpp
{
    HDC dc;
    uint16 win_w, win_h;
    uint16 scr_w, scr_h;
};

void *App_Allocate(uint64 size, char *file, uint32 line)
{
    DAssert(size != 0);
    
    void *output = malloc(size);
    
    if(output != 0)
    {
#ifdef LOG_ALLOCATIONS
	FLog("Allocation [ptr=0x%x, size=%llu, file=%s, line=%i]", output, size, file, line);
#endif DONT_LOG_ALLOCATIONS
    }
    else
    {
#ifdef LOG_ALLOCATIONS
	FLog("ALLOCATION FAILLED!!! [ptr=0x%x, size=%llu, file=%s, line=%i]", output, size, file, line);
#endif DONT_LOG_ALLOCATIONS
	
	MessageBox(0, "MEMORY ALLOCATION FAILED!!!", "Error", 0);
	ad->program_state = 0;
    }
    
    return output;
}

void *App_Reallocate(void *ptr, uint64 size, char *file, uint32 line)
{
    DAssert(ptr);
    DAssert(size != 0);
    
    void *output = realloc(ptr, size);
    
    if(output != 0)
    {
#ifdef LOG_ALLOCATIONS
	FLog("Rellocation [ptr=0x%x, size=%llu, file=%s, line=%i]", output, size, file, line);
#endif DONT_LOG_ALLOCATIONS
    }
    else
    {
#ifdef LOG_ALLOCATIONS
	FLog("REALLOCATION FAILED!!! [ptr=0x%x, size=%llu, file=%s, line=%i]", output, size, file, line);
#endif DONT_LOG_ALLOCATIONS
    }
    
    return output;
}

void App_Free(void *ptr, char *file, uint32 line)
{
    DAssert(ptr);

#ifdef LOG_ALLOCATIONS
    FLog("Free [ptr=0x%x, file=%s, line=%i]", ptr, file, line);
#endif DONT_LOG_ALLOCATIONS
    
    free(ptr);
}

void App_MessageBox(char *message, char *title)
{
    DAssert(message);
    DAssert(title);
    
    MessageBox(0, message, title, 0);
}

bool App_GetLastErrorMessage(char *msg, uint32 size)
{
    DAssert(msg);
    
    DWORD last_error = GetLastError();
    if(!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, last_error, 0, msg, size, 0))
	return false;

    // HACK(daniel): the formated message is returned with a /r/n, we remove those
    uint32 len = String::CalcLength(msg);
    msg[len - 3] = 0;

    return true;
}

uint32 App_Random()
{
    return rand();
}

uint64 App_GetTicks()
{
    LARGE_INTEGER counter;
    
    if(!QueryPerformanceCounter(&counter))
    {
	LogError("QueryPerformanceCounter failed.");
	return 0;
    }

    return counter.QuadPart;
}

double App_GetTimeDifference(uint64 start, uint64 end)
{
    DAssert(start <= end);

    LARGE_INTEGER freq;
    
    if(!QueryPerformanceFrequency(&freq))
    {
	LogError("QueryPerformanceFrequency failed.");
	return 0;
    }

    DAssert(freq.QuadPart != 0);

    return (double)(end - start) / (double)(freq.QuadPart);
}

double App_GetTime()
{
    LARGE_INTEGER freq;
    
    if(!QueryPerformanceFrequency(&freq))
    {
	LogError("QueryPerformanceFrequency failed.");
	return 0;
    }

    DAssert(freq.QuadPart != 0);

    LARGE_INTEGER counter;
    
    if(!QueryPerformanceCounter(&counter))
    {
	LogError("QueryPerformanceCounter failed.");
	return 0;
    }

    return double(counter.QuadPart) / (double)(freq.QuadPart);
}

void App_SleepInaccurate(uint64 ms)
{
    // TODO(daniel): what should we do when ms equals 0?
    // sometimes we do Sleep(0) or Sleep(1) if the game is out of focus
    // do we want to support it here?

    // NOTE(daniel): Win32 Sleep function only takes a SUGGESTED MINIMUM, it gurantees nothing
    Sleep(ms);
}

void App_SleepAccurate(uint64 ms)
{
    // TODO(daniel): test this to see how it behaves on different machines and if
    // it's safe to be doing it this way

    uint64 start = App_GetTicks();
    uint64 now = App_GetTicks();

    double wait = (double)(ms) / 1000.0;

    // NOTE(daniel): for debugging
    double diff;
    
    while((diff = App_GetTimeDifference(start, now)) < wait)
    {
	now = App_GetTicks();
    }
}

void ProcessAppEvents()
{
    MSG msg;
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
    { 
	TranslateMessage(&msg); 
	DispatchMessage(&msg); 
    }
}

void QueryHardwareInfo(MachineData *md)
{
    DAssert(md);
    
    SYSTEM_INFO sys_info = { 0 };
    GetSystemInfo(&sys_info);

    md->oem_id = sys_info.dwOemId;
    md->processor_architecture = sys_info.wProcessorArchitecture;
    md->page_size = sys_info.dwPageSize;
    md->minimum_application_address_ptr = sys_info.lpMinimumApplicationAddress;
    md->maximum_application_address_ptr = sys_info.lpMaximumApplicationAddress;
    md->active_processor_mask_ptr = (uint32 *)sys_info.dwActiveProcessorMask;
    md->num_of_processors = sys_info.dwNumberOfProcessors;
    md->processors_type = sys_info.dwProcessorType;
    md->alloc_granularity = sys_info.dwAllocationGranularity;
    md->processor_level = sys_info.wProcessorLevel;
    md->processor_revision = sys_info.wProcessorRevision;

    Log_BeginSection("Machine Information:");
    FLog("OEM ID: %u", md->oem_id);

    char *str = 0;
    switch(md->processor_architecture)
    {
	case PROCESSOR_ARCHITECTURE_AMD64: { str = "AMD64"; } break;
	case PROCESSOR_ARCHITECTURE_ARM: { str = "ARM"; } break;
	case PROCESSOR_ARCHITECTURE_IA64: { str = "IA64"; } break;
	case PROCESSOR_ARCHITECTURE_INTEL: { str = "X86"; } break;
	case PROCESSOR_ARCHITECTURE_UNKNOWN: { str = "unkown"; } break;
    }
    FLog("Processor Architecture: %s", str);

    FLog("Page Size: %u", md->page_size);
    FLog("Minimum Application Address: 0x%x", md->minimum_application_address_ptr);
    FLog("Maximum Application Address: 0x%x", md->maximum_application_address_ptr);
    FLog("Active Processor Mask : 0x%x", md->active_processor_mask_ptr);
    FLog("Number Of Processors: %u", md->num_of_processors);

    str = 0;
    switch(md->processors_type)
    {
	case PROCESSOR_INTEL_386 : { str = "INTEL 386"; } break;
	case PROCESSOR_INTEL_486 : { str = "INTEL 486"; } break;
	case PROCESSOR_INTEL_PENTIUM : { str = "INTEL Pentium"; } break;
	case PROCESSOR_INTEL_IA64 : { str = "Intel Itanium 64"; } break;
	case PROCESSOR_AMD_X8664 : { str = "x86//x64"; } break;
	    //case PROCESSOR_ARM : { str = "ARM"; } break; // TODO(daniel): PROCESSOR_ARM is not defined???
    }
    FLog("Processor Type: %u", str);
    
    FLog("Allocation Granularity: %u", md->alloc_granularity);
    FLog("Processor Level: %u", md->processor_level);
    FLog("Processor Revision: %u", md->processor_revision);

    // TODO(daniel): Log extra info, such as time & date, user name, anything we might want for testing
    
    Log_EndSection();
}

bool InitAppData(AppData *ad)
{
    DAssert(ad);

    QueryHardwareInfo(&ad->md);

    ad->win_w = WINDOW_WIDTH;
    ad->win_h = WINDOW_HEIGHT;
    ad->scr_w = SCREEN_WIDTH;
    ad->scr_h  = SCREEN_HEIGHT;
    ad->program_state = 1;
    
    ad->gamepad = ad->gamepad_prev = { 0 };

    return true;
}

void DeleteAppData(AppData *ad)
{
    if(ad)
    {
	
    }
}

void UpdateAppData(GameData *gd, HDC dc)
{
    DAssert(gd);

    R_Begin();

    if(gd->tex)
	R_SetTexture(gd->tex);

    Sprite *sprite = Sprite_GetFirst(&gd->sprite_list);

    while(sprite)
    {
	R_Sprite(sprite, Color::white);
	sprite = Sprite_GetNext(sprite);
    }
	
    R_End();
}

void Quit()
{
    DeleteGameData(ad, gd);
    DeleteAppData(ad);

    if(gd)
    {
	ldelete(gd);
	gd = 0;
    }

    if(ad)
    {
	ldelete(ad);
	ad = 0;
    }
    
    DumpLogToFile();

    // NOTE(daniel): this will not be logged
    FILE_DestroyGlobalFilePool();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
	case WM_DESTROY:
	{
	    ad->program_state = 0;
	} break;

	case WM_QUIT:
	{
	} break;

	case WM_SIZE:
	{
	    ad->win_w = LOWORD(lParam);
	    ad->win_h = HIWORD(lParam);
	    R_Resize(ad->win_w, ad->win_h, ad->scr_w, ad->scr_h);
	} break;

	case WM_KEYDOWN:
	{
	    switch(wParam)
	    {
		case VK_UP:
		{
		    ad->gamepad.up = true;
		} break;
		case VK_DOWN:
		{
		    ad->gamepad.down = true;
		} break;
		case VK_LEFT:
		{
		    ad->gamepad.left = true;
		} break;
		case VK_RIGHT:
		{
		    ad->gamepad.right = true;
		} break;
		case 'W':
		{
		    ad->gamepad.w = true;
		} break;
		case 'S':
		{
		    ad->gamepad.s = true;
		} break;
		case 'A':
		{
		    ad->gamepad.a = true;
		} break;
		case 'D':
		{
		    ad->gamepad.d = true;
		} break;
		case VK_SPACE:
		{
		    ad->gamepad.space = true;
		} break;
		case 'R':
		{
		    ad->gamepad.r = true;
		} break;
		case 'V':
		{
		    ad->gamepad.v = true;
		} break;
	    }
	} break;

	case WM_KEYUP:
	{
	    switch(wParam)
	    {
		case VK_UP:
		{
		    ad->gamepad.up = false;
		} break;
		case VK_DOWN:
		{
		    ad->gamepad.down = false;
		} break;
		case VK_LEFT:
		{
		    ad->gamepad.left = false;
		} break;
		case VK_RIGHT:
		{
		    ad->gamepad.right = false;
		} break;
		case 'W':
		{
		    ad->gamepad.w = false;
		} break;
		case 'S':
		{
		    ad->gamepad.s = false;
		} break;
		case 'A':
		{
		    ad->gamepad.a = false;
		} break;
		case 'D':
		{
		    ad->gamepad.d = false;
		} break;
		case VK_SPACE:
		{
		    ad->gamepad.space = false;
		} break;
		case 'R':
		{
		    ad->gamepad.r = false;
		} break;
		case 'V':
		{
		    ad->gamepad.v = false;
		} break;
	    }
	} break;
	
	default:
	{
	    return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void TestString()
{
    String s1;
    
    String s2 = String("Hello World!");
    uint32 s2_len = s2.Length();

    String s3 = "This is assigned!";
    uint32 s3_len = s3.Length();

    String s4 = "This is going to get deleted!";
    uint32 s4_len = s4.Length();

    s4 = "A new string";
    s4_len = s4.Length();

    String s5 = "i'm not ";
    s5.Append("finished!");

    String s6 = "operator ";
    s6 += "overloaded";

    String s7 = String::ParseInt(12345);

    String s8 = "The Number is: ";
    s8 += 876235;

    String s9 = "Why the negative attitude?: ";
    s9 += -54321;

    Log((String("This will go to the log: ") + 11111));
#if 0
    Log_BeginSection("string int parsing test");
    for(int i = 0; i < 99999; i++)
    {
	String s = "Mighty number: ";
	s += -i;
	Log(s.c_str);
    }
    Log_EndSection();
#endif
}

uint32 WinMainWrap(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // NOTE(daniel): makes the windows scheduler timer check every 1 ms
    uint32 scheduler_period_ms = 1;
    timeBeginPeriod(scheduler_period_ms);
    
    srand(time(0));
    
    if(!Log_Init(LOG_LEVEL_SIMPLE))
    {
	App_MessageBox("Failed to init logging system!", "Error");
	return -1;
    }

    if(!FILE_InitGlobalFilePool(512))
    {
	App_MessageBox("Failed to init file memory pool!", "Error");
	return -1;
    }

    // NOTE(daniel): From here on out it is possible to dump the log to a file.

    TestString();

    ad = (AppData *)lnew(sizeof(AppData));
    DAssert(ad);
    
    if(!InitAppData(ad))
    {
	LogError("InitAppData failed.");
	Quit();
	return -1;
    }
    
    WNDCLASS wndclass;
    ZeroMemory(&wndclass, sizeof(WNDCLASS));

    wndclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WindowProc; 
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(hInstance, IDC_ARROW);
    wndclass.hbrBackground = GetSysColorBrush(COLOR_ACTIVEBORDER);
    wndclass.lpszMenuName = 0;
    wndclass.lpszClassName = CLASS_NAME;

    if(!RegisterClass(&wndclass))
    {
	LogError("RegisterClass failed.");
	Quit();
	return -1;
    }

    HWND hwnd = CreateWindow(CLASS_NAME, "OpenGL Window", WS_OVERLAPPEDWINDOW,
			     0, 0, ad->win_w, ad->win_h, 0, 0, hInstance, 0);
    
    if(!hwnd)
    {
	LogError("CreateWindow failed.");
	Quit();
	return -1;
    }

    ShowWindow(hwnd, SW_SHOW);

    RendererInitData rid;
    rid.dc = GetDC(hwnd);
    rid.win_w = ad->win_w;
    rid.win_h = ad->win_h;
    rid.scr_w = ad->scr_w;
    rid.scr_h = ad->scr_h;

    if(!R_Init(&rid))
    {
	LogError("R_Init failed.");
	return -1;
    }

    gd = (GameData *)lnew(sizeof(GameData));
    DAssert(gd);
    
    if(!InitGameData(ad, gd))
    {
	Log("InitGameData failed.");
	Quit();
	return -1;
    }

    uint64 game_loop_frames = 0;
    
    // TODO(daniel): query the actual monitor refresh rate and decide the game refresh rate
    // based on it
    uint32 monitor_refresh_rate_hz = 60;
    uint32 game_refresh_rate_hz = monitor_refresh_rate_hz;
    
    double cur_time = App_GetTicks();
    double prev_time = App_GetTicks();
    double time_elapsed = 0.0;
    double lag = 0.0;

    double sleep = 0;
    double desired_fps = 1.0 / (double)game_refresh_rate_hz;

    R_DisableVSync();
    bool to_sleep = !R_IsVSyncEnabled();

    uint64 frame_time_timer_start = App_GetTicks();
    uint64 frame_time_timer_end = App_GetTicks();
    double frame_time_time_elapsed = App_GetTimeDifference(frame_time_timer_start,
							   frame_time_timer_end);
    
    ad->dt = desired_fps;
    
    while(ad->program_state)
    {
	// NOTE(daniel): if vsync is enabled SwapBuffers will sleep for us, if not
	// we sleep ourselves
	to_sleep = !R_IsVSyncEnabled();
	
	cur_time = App_GetTicks();
	time_elapsed = App_GetTimeDifference(prev_time, cur_time);
	prev_time = cur_time;
	lag += time_elapsed;

	frame_time_timer_start = App_GetTicks();
	
	ProcessAppEvents();

	uint64 game_update_timer_start = App_GetTicks();

	while(lag >= desired_fps)
	{
	    UpdateGameData(ad, gd);
	    lag -= desired_fps;
	}

	uint64 game_update_timer_end = App_GetTicks();
	double game_update_time_elapsed = App_GetTimeDifference(game_update_timer_start,
								game_update_timer_end);

	uint64 app_update_timer_start = App_GetTicks();

	UpdateAppData(gd, rid.dc);

	uint64 app_update_timer_end = App_GetTicks();
	double app_update_time_elapsed = App_GetTimeDifference(app_update_timer_start,
							       app_update_timer_end);
	
	uint64 swap_buffers_timer_start = App_GetTicks();

	SwapBuffers(rid.dc);

	uint64 swap_buffers_timer_end = App_GetTicks();
	double swap_buffers_time_elapsed = App_GetTimeDifference(swap_buffers_timer_start,
								 swap_buffers_timer_end);


	frame_time_timer_end = App_GetTicks();
	frame_time_time_elapsed = App_GetTimeDifference(frame_time_timer_start,
							frame_time_timer_end);
	
	uint64 app_sleep_timer_start = App_GetTicks();
	
	if(to_sleep)
	{
	    sleep = desired_fps - frame_time_time_elapsed;
	    if(sleep > 0)
	    {
		App_SleepAccurate(sleep * 1000.0);
	    }
	}

	uint64 app_sleep_timer_end = App_GetTicks();
	double app_sleep_time_elapsed = App_GetTimeDifference(app_sleep_timer_start,
							      app_sleep_timer_end);
	
#if 0
	char sec[2048];
	sprintf(sec, "Frame: %i", game_loop_frames);
	Log_BeginSection(sec);
	
	FLog("Time Elapsed For Previous Frame: %fms", time_elapsed * 1000.0);
	FLog("Frame Time: %fms", frame_time_time_elapsed * 1000.0);
	FLog("Game Update Time: %fms", game_update_time_elapsed * 1000.0);
	FLog("App Update Time: %fms", app_update_time_elapsed * 1000.0);
	FLog("Swap Buffers Time: %fms", swap_buffers_time_elapsed * 1000.0);
	FLog("App Sleep Time: %fms", app_sleep_time_elapsed * 1000.0);

	FLog("VSync: %i", R_IsVSyncEnabled());
	
	Log_EndSection();
#endif
#if 1
	Log_BeginSection(String("Frame: ") + game_loop_frames);
	
	Log(String("Time Elapsed For Previous Frame: ") + time_elapsed * 1000000.0);
	Log(String("Frame Time: ") + frame_time_time_elapsed * 1000000.0);
	Log(String("Game Update Time: ") + game_update_time_elapsed * 1000000.0);
	Log(String("App Update Time: ") + app_update_time_elapsed * 1000000.0);
	Log(String("Swap Buffers Time: ") + swap_buffers_time_elapsed * 1000000.0);
	Log(String("App Sleep Time: ") + app_sleep_time_elapsed * 1000000.0);

	Log(("VSync: ") + R_IsVSyncEnabled());
	
	Log_EndSection();
#endif

	UpdateLogDisplay();
	
	game_loop_frames++;
    }

    timeEndPeriod(scheduler_period_ms);

    return 0;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    uint32 result = WinMainWrap(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    Quit();

    return result;
}
