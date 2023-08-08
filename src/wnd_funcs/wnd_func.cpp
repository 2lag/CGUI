#include "wnd_func.h"
// combine these in= win_feats.h or smthn for cleanliness
#include "wnd_drag.h"
#include "wnd_title.h"
#include "wnd_resize.h"

/*
  fix wnd_title_clicked_max by making custom implementation
  then do the same for wnd_drag when dragging as maximized window
    restore to previous size and move window to drag under where it was initially dragged from
  add resize functionality
    if mouse is within 3/5 pixels of any edge & lmb clicked, do resize
  remove debug shit
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

#ifdef _DEBUG
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
  case WM_GETMINMAXINFO: {
    LPMINMAXINFO lpmmi = (LPMINMAXINFO)lp;
    lpmmi->ptMaxPosition = {
      0, 1
    };
    lpmmi->ptMaxSize = {
      GetSystemMetrics( SM_CXFULLSCREEN ),
      GetSystemMetrics( SM_CYFULLSCREEN ) + 22
    };
  } break;
  case WM_LBUTTONDOWN: {
    wnd_drag_on( hwnd, { 0, 0, wnd_sz.right - 75, 25 }, m_pos );

    RECT cls{ wnd_sz.right - 25, 0, wnd_sz.right - 00, 25 };
    RECT max{ wnd_sz.right - 50, 0, wnd_sz.right - 25, 25 };
    RECT min{ wnd_sz.right - 75, 0, wnd_sz.right - 50, 25 };
    wnd_title_clicked_cls( hwnd, PtInRect( &cls, m_pos ) );
    wnd_title_clicked_max( hwnd, PtInRect( &max, m_pos ) );
    wnd_title_clicked_min( hwnd, PtInRect( &min, m_pos ) );

    wnd_resize( hwnd, m_pos, wnd_sz );
  } break;
  case WM_LBUTTONUP: {
    wnd_drag_off();
  } break;
  case WM_MOUSEMOVE: {
    wnd_drag( hwnd, m_pos );
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