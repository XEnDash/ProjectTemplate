#include "log.h"

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

#include "app.h"
#include "string_functions.h"
#include "file.h"

#define LOG_CLASS_NAME "logCN"
#define LOG_WIDTH 640
#define LOG_HEIGHT 480

char *log_buffer;
uint32 log_buffer_size = 1*1024*1024;
uint32 log_buffer_used_size = 0;

HWND log_hwnd = 0;
HDC log_dc = 0;

int32 log_scroll_pos = 0;
int32 log_scroll_last_pos = 0;

uint32 log_number_of_lines = 0;
uint32 log_line_height;

bool log_initialized = false;
bool log_redraw = false;

uint32 log_level;

LRESULT CALLBACK LogWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
	case WM_DESTROY:
	{
	    return 0;
	} break;

	case WM_QUIT:
	{
	    return 0;
	} break;

	case WM_PAINT:
	{
	    PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

	    RECT log_rect = { 0 };
	    RECT win_rect = { 0 };
	    GetWindowRect(log_hwnd, &win_rect);

	    log_rect = { 0, 0 - log_scroll_pos,
			 win_rect.right - win_rect.left,
			 win_rect.bottom - win_rect.top};
    
	    DrawText(log_dc, log_buffer, log_buffer_used_size, &log_rect,
		     DT_LEFT | DT_EXTERNALLEADING | DT_WORDBREAK);

	    EndPaint(hwnd, &ps);

	    SCROLLINFO scroll_info = { 0 };
	    scroll_info.cbSize = sizeof(SCROLLINFO);
	    scroll_info.fMask = SIF_RANGE;
	    scroll_info.nMin = 0;
	    scroll_info.nMax = log_number_of_lines * log_line_height;
    
	    SetScrollInfo(log_hwnd, SB_VERT, &scroll_info, true);
	    
	} break;

	case WM_VSCROLL:
	{
	    switch LOWORD(wParam)
	    {
		case SB_THUMBTRACK:
		{
		    log_scroll_last_pos = log_scroll_pos;
		    log_scroll_pos = HIWORD(wParam);
		    SetScrollPos(log_hwnd, SB_VERT, log_scroll_pos, TRUE);
		    log_redraw = true;
		    UpdateLogDisplay();
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

bool Log_Init(uint32 level)
{
    if(log_buffer)
	delete log_buffer;

    log_buffer = (char *)lnew(log_buffer_size);
    DAssert(log_buffer);

    WNDCLASS wndclass;
    ZeroMemory(&wndclass, sizeof(WNDCLASS));

    wndclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = LogWindowProc; 
    wndclass.hInstance = 0;
    wndclass.hIcon = LoadIcon(0, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(0, IDC_ARROW);
    wndclass.hbrBackground = GetSysColorBrush(COLOR_ACTIVEBORDER);
    wndclass.lpszMenuName = 0;
    wndclass.lpszClassName = LOG_CLASS_NAME;

    if(!RegisterClass(&wndclass))
    {
	//Log("RegisterClass failed."); // TODO(daniel): logless logging
	return -1;
    }

    log_hwnd = CreateWindow(LOG_CLASS_NAME, "Log Window", WS_OVERLAPPEDWINDOW | WS_VSCROLL,
			     1600 - LOG_WIDTH, 900 - LOG_HEIGHT, LOG_WIDTH, LOG_HEIGHT, 0,
			     NULL, 0, 0);
    
    if(!log_hwnd)
    {
	//Log("CreateWindow failed."); // TODO(daniel): logless logging
	return -1;
    }
    
    log_dc = GetDC(log_hwnd);

    if(!log_dc)
    {
	//Log("GetDC failed."); // TODO(daniel): logless logging
	return -1;
    }

    SCROLLINFO scroll_info = { 0 };
    scroll_info.cbSize = sizeof(SCROLLINFO);
    scroll_info.fMask = SIF_POS | SIF_RANGE;
    scroll_info.nPos = 0;
    scroll_info.nMin = 0;
    scroll_info.nMax = 10000;
    
    SetScrollInfo(log_hwnd, SB_VERT, &scroll_info, true);

    SIZE font_metrics;
    if(!GetTextExtentPoint32(log_dc, "Test String", 11, &font_metrics))
    {
	//Log("GetTextExtentPoint32 failed.");
	return -1;
    }

    log_line_height = font_metrics.cy;

    ShowWindow(log_hwnd, SW_SHOW);

    log_initialized = true;

    Log_SetLevel(level);

    Log("=== Log Initialized ===");
    
    return true;
}

void Log_SetLevel(uint32 level)
{
    DAssert(level >= LOG_LEVEL_NONE && level <= LOG_LEVEL_VERBOSE);
    log_level = level;
}

void SLog(const char *s, char *file, uint32 line)
{
    if(!log_initialized)
	return;

    if(log_level == LOG_LEVEL_NONE)
	return;
    
    bool32 printmeta = (log_level == LOG_LEVEL_VERBOSE);

    //uint32 s_len = STR_Length((char *)s);

    if(printmeta)
    {
	char buffer[4096];
	sprintf(buffer, "%s [file=%s, line=%i]", s, file, line);
    
	uint32 s_len = STR_Length((char *)buffer);

	if(log_buffer_used_size + s_len + 1 > log_buffer_size)
	{
	    App_MessageBox("Log size limit reached, please allocate more memory for the log", "Error");
	    log_initialized = false;
	    return;
	}
    
	sprintf(log_buffer + log_buffer_used_size, buffer);
	log_buffer_used_size += s_len;

	DebugOutputLog(buffer);
    }
    else
    {
	uint32 s_len = STR_Length((char *)s);
        if(log_buffer_used_size + s_len + 1 > log_buffer_size)
	{
	    App_MessageBox("Log size limit reached, please allocate more memory for the log", "Error");
	    log_initialized = false;
	    return;
	}

	sprintf(log_buffer + log_buffer_used_size, s);
	log_buffer_used_size += s_len;

	DebugOutputLog(s);
    }
    
    sprintf(log_buffer + log_buffer_used_size, "\n");
    log_buffer_used_size += 1;

    DebugOutputLog("\n");

    log_number_of_lines++;
    log_redraw = true;
}

void FLog(const char *s, ...)
{
    if(!log_initialized)
	return;

    if(log_level == LOG_LEVEL_NONE)
	return;

    char buffer[4096];

    va_list ap;
    va_start(ap, s);
    
    vsprintf(buffer, s, ap);
    
    va_end(ap);

    uint32 buf_len = STR_Length((char *)buffer);

    if(log_buffer_used_size + buf_len + 1 > log_buffer_size)
    {
	App_MessageBox("Log size limit reached, please allocate more memory for the log", "Error");
	log_initialized = false;
	return;
    }
    
    sprintf(log_buffer + log_buffer_used_size, buffer);
    log_buffer_used_size += buf_len;
    sprintf(log_buffer + log_buffer_used_size, "\n");
    log_buffer_used_size += 1;

    DebugOutputLog(buffer);
    DebugOutputLog("\n");
    
    log_number_of_lines++;
    log_redraw = true;
}

void UpdateLogDisplay()
{
    if(log_redraw)
    {
	RedrawWindow(log_hwnd, 0, 0, RDW_INVALIDATE);
	log_redraw = false;
    }
}

bool DumpLogToFile()
{
    File log_file;
    if(!FILE_Open(&log_file, LOG_FILE_NAME, OPEN_MODE_ALWAYS))
	return false;

    if(!FILE_Clear(&log_file))
	return false;

    if(!FILE_Write(&log_file, log_buffer, log_buffer_used_size))
	return false;

    FILE_Close(&log_file);

    return true;
}
