#pragma once
#include "wincludes.h"

#include "../typedef.h"

void wnd_drag_on( HWND hwnd, RECT title_bar, POINT m_pos );
void wnd_drag_off();
void wnd_drag( HWND hwnd, POINT m_pos );