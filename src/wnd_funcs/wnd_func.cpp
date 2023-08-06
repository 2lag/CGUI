#include "wnd_func.h"

#include "wnd_title.h"
#include "wnd_drag.h"

/*
  fix title bar drag
  remove debug shit
  find out if you can fix title bar flicker
  build out title bar functions
  make custom menu bar
  build out menu bar functions
  then go back to source.cpp todo list
*/
#ifdef _DEBUG
#include <iostream>
#endif

LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
  RECT wnd_sz;
  POINT m_pos;
  (void)GetClientRect( hwnd, &wnd_sz );
  (void)GetCursorPos( &m_pos );
  (void)ScreenToClient( hwnd, &m_pos );

#ifdef _DEBUG;
  (void)AllocConsole();
  FILE* new_std;
  (void)freopen_s( &new_std, "CONOUT$", "w", stdout );
  std::cout << m_pos.x << " " << m_pos.y << std::endl;
#endif

  switch( msg ) {
  case WM_DESTROY: {
    PostQuitMessage( 0 );
    return 0;
  } break;
  case WM_LBUTTONDOWN: {
    wnd_drag_on( hwnd, { 0, 0, wnd_sz.right - 75, 25 }, m_pos );
  } break;
  case WM_LBUTTONUP: {
    wnd_drag_off();
  } break;
  case WM_MOUSEMOVE: {
    wnd_drag( hwnd, m_pos );
    (void)InvalidateRect( hwnd, nullptr, false );
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