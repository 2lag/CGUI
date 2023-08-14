#include "wnd_func.h"
// combine these in win_feats.h or smthn for cleanliness
#include "wnd_drag.h"
#include "wnd_title.h"
#include "wnd_resize.h"

/*
  finish wnd_resize.cpp
  check wnd_drag bugs then finish remaining funcs
  simplify/optimize all code ( code review !! )
  then go back to source.cpp todo list  
*/

LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
  RECT wnd_sz;
  POINT m_pos;
  GetClientRect( hwnd, &wnd_sz );
  GetCursorPos( &m_pos );
  ScreenToClient( hwnd, &m_pos );
  
  s32 d_side{};
  wnd_resize_get_side( d_side, m_pos, wnd_sz );

#ifdef _DEBUG
  AllocConsole();
  FILE* new_std;
  freopen_s( &new_std, "CONOUT$", "w", stdout );

#endif

  switch( msg ) {
  case WM_DESTROY: {
    PostQuitMessage( 0 );
    return 0;
  } break;
  case WM_LBUTTONDBLCLK: {
    wnd_resize_title( hwnd, { 0, 6, wnd_sz.right - 75, 25 }, m_pos );
  } break;
  case WM_LBUTTONDOWN: {
    wnd_resize_on( hwnd, m_pos, d_side );

    wnd_drag_on( hwnd, { 0, 6, wnd_sz.right - 75, 25 }, m_pos );

    // simplify
    RECT cls{ wnd_sz.right - 25, 5, wnd_sz.right - 05, 25 };
    RECT max{ wnd_sz.right - 50, 5, wnd_sz.right - 25, 25 };
    RECT min{ wnd_sz.right - 75, 5, wnd_sz.right - 50, 25 };
    wnd_title_clicked_cls( hwnd, PtInRect( &cls, m_pos ) );
    wnd_title_clicked_max( hwnd, PtInRect( &max, m_pos ) );
    wnd_title_clicked_min( hwnd, PtInRect( &min, m_pos ) );
  } break;
  case WM_LBUTTONUP: {
    wnd_drag_max( hwnd, m_pos );
    wnd_drag_off();
    wnd_resize_off();
  } break;
  case WM_MOUSEMOVE: {
    wnd_drag( hwnd, m_pos );
    if( !is_maxd )
      wnd_resize( hwnd, m_pos, wnd_sz, d_side );
  } break;
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint( hwnd, &ps );

    wnd_title_draw( hdc, m_pos, wnd_sz );

    EndPaint( hwnd, &ps );
  } break;
  }
  return DefWindowProcW( hwnd, msg, wp, lp );
}