#pragma once
#include "wincludes.h"

#include "../typedef.h"

enum WND_RESIZE_CASES {
  EDGE_TOP_LEFT  = 1,
  EDGE_TOP         ,
  EDGE_TOP_RIGHT   ,
  EDGE_RIGHT       ,
  EDGE_BOTTOM_RIGHT,
  EDGE_BOTTOM      ,
  EDGE_BOTTOM_LEFT ,
  EDGE_LEFT
};

void wnd_resize_get_side( s32 &d_side, POINT m_pos, RECT wnd_sz );
void wnd_resize_on( HWND hwnd, POINT m_pos, s32 d_side );
void wnd_resize_off();
void wnd_resize_title( HWND hwnd, RECT title_bar, POINT m_pos );
void wnd_resize( HWND hwnd, POINT m_pos, RECT wnd_sz, s32 d_side );