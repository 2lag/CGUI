#pragma once
#define WIN32_LEAN_AND_MEAN
#include  <windows.h>
#include <windowsx.h>
#include   <dwmapi.h>
#pragma comment( lib, "dwmapi" )

#include "../typedef.h"

salt __stdcall wnd_proc(HWND hwnd, u32 msg, u32 wp, salt lp);