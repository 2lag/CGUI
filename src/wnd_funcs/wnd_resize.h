#pragma once
#include "wincludes.h"

#include "../typedef.h"

enum wnd_resize_cases {
  EDGE_TOP_LEFT        = 1,
  EDGE_TOP             = 2,
  EDGE_TOP_RIGHT       = 3,
  EDGE_RIGHT           = 4,
  EDGE_BOTTOM_RIGHT    = 5,
  EDGE_BOTTOM          = 6,
  EDGE_BOTTOM_LEFT     = 7,
  EDGE_LEFT            = 8
};

void wnd_resize_get_side( s32 &d_side, POINT m_pos, RECT wnd_sz );
void wnd_resize_on( HWND hwnd, POINT m_pos, s32 d_side );
void wnd_resize_off();
void wnd_resize( HWND hwnd, POINT m_pos, s32 d_side );