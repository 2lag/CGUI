#include "wnd_func.h"

#include "wnd_title.h"
#include "wnd_drag.h"

/*
  slim down current code with functions and other opts
  build out the maximize title bar functions
  remove title bar to-do stuf from source.cpp
  add resize functionality
  make custom menu bar
  build out menu bar functions
  then go back to source.cpp todo list
*/


LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
  RECT wnd_sz;
  POINT m_pos;
  (void)GetClientRect( hwnd, &wnd_sz );
  (void)GetCursorPos( &m_pos );
  (void)ScreenToClient( hwnd, &m_pos );

  switch( msg ) {
  case WM_DESTROY: {
    PostQuitMessage( 0 );
    return 0;
  } break;
  case WM_LBUTTONDOWN: {
    wnd_drag_on( hwnd, { 0, 0, wnd_sz.right - 75, 25 }, m_pos );

    RECT cls{ wnd_sz.right - 25, 0, wnd_sz.right - 00, 25 };
    wnd_title_clicked_cls( hwnd, PtInRect( &cls, m_pos ) );

    RECT max{ wnd_sz.right - 50, 0, wnd_sz.right - 25, 25 };
    wnd_title_clicked_max( hwnd, PtInRect( &max, m_pos ) );
    
    RECT min{ wnd_sz.right - 75, 0, wnd_sz.right - 50, 25 };
    wnd_title_clicked_min( hwnd, PtInRect( &min, m_pos ) );

    (void)InvalidateRect( hwnd, nullptr, false );
  } break;
  case WM_LBUTTONUP: {
    wnd_drag_off();
    (void)InvalidateRect( hwnd, nullptr, false );
  } break;
  case WM_MOUSEMOVE: {
    wnd_drag( hwnd, m_pos );
    return 0;
  } break;
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint( hwnd, &ps );

    wnd_title_draw( hdc, m_pos, wnd_sz );

    (void)EndPaint( hwnd, &ps );
  } break;
  }
  return DefWindowProcW( hwnd, msg, wp, lp );
}