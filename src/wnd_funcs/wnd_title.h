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
  bool hover;
};

inline wnd_obj wnd_obj_create( RECT r, COLORREF col, bool hover ) {
  wnd_obj obj;
  obj.r = r;
  obj.col = CreateSolidBrush( col );
  obj.hover = hover;
  return obj;
}

void wnd_title_draw( HDC hdc, POINT m_pos, RECT wnd_sz );
void wnd_title_clicked_cls( HWND hwnd, POINT m_pos, RECT space );