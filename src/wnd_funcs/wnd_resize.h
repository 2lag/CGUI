#pragma once
#include "wincludes.h"

#include "../typedef.h"

void wnd_resize_on( HWND hwnd, POINT m_pos, bool in_rt );
void wnd_resize_off();
void wnd_resize( HWND hwnd, POINT m_pos, RECT wnd_sz );