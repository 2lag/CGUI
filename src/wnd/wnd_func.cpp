#include "wnd_func.h"
#include "funcs/wnd_includes.h"

/*
  simplify/optimize all code ( code review !! )
    - wnd_drag_quart/half/max
    - wincludes.h
  then go back to source.cpp todo list  
*/

LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
  RECT wnd_sz;
  GetClientRect( hwnd, &wnd_sz );

  POINT m_pos;
  GetCursorPos( &m_pos );
  ScreenToClient( hwnd, &m_pos );

  RECT drag { 0, 6, wnd_sz.right - 75, 25 },
     cls { wnd_sz.right - 25, 5,
           wnd_sz.right - 05, 25
  }, max { wnd_sz.right - 50, 5,
           wnd_sz.right - 25, 25
  }, min { wnd_sz.right - 75, 5,
           wnd_sz.right - 50, 25
  };
  
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
    wnd_resize_title( hwnd, PtInRect( &drag, m_pos ) );
  } break;
  case WM_LBUTTONDOWN: {
    wnd_resize_on( hwnd, m_pos, wnd_sz );
    wnd_drag_on( hwnd, m_pos, PtInRect( &drag, m_pos ) );
    wnd_title_cls( hwnd, PtInRect( &cls, m_pos ) );
    wnd_title_max( hwnd, PtInRect( &max, m_pos ) );
    wnd_title_min( hwnd, PtInRect( &min, m_pos ) );
  } break;
  case WM_LBUTTONUP: {
    if( !user_resizing ) {
      wnd_drag_max( hwnd, m_pos );
      wnd_drag_half( hwnd, m_pos );
      wnd_drag_quart( hwnd, m_pos );
    }
    wnd_drag_off();
    wnd_resize_off();
  } break;
  case WM_MOUSEMOVE: {
    wnd_drag( hwnd, m_pos );
    if( is_maxd )
      break;
    wnd_resize_get_cursor( m_pos, wnd_sz );
    wnd_resize( hwnd, m_pos, wnd_sz );
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