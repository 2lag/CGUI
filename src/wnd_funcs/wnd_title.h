#pragma once
#include "wincludes.h"

#include "../typedef.h"

enum wnd_col {
  COL_D_GRY = RGB(  32,  32,  32 ),
  COL_L_GRY = RGB( 120, 120, 120 ),
  COL_D_RED = RGB(  64,  32,  32 ),
};

struct wnd_obj {
  RECT r;
  HBRUSH col;
  bool next;
};

inline wnd_obj wnd_obj_create( RECT r, COLORREF col, bool next ) {
  wnd_obj obj;
  obj.r = r;
  obj.col = CreateSolidBrush( col );
  obj.next = next;
  return obj;
}

void wnd_title_draw( HDC hdc, POINT m_pos, RECT wnd_sz );
void wnd_title_cls( HWND hwnd, bool mouse_over );
void wnd_title_max( HWND hwnd, bool mouse_over );
void wnd_title_min( HWND hwnd, bool mouse_over );