#include "wnd_title.h"

void wnd_title_draw( HDC hdc, POINT m_pos, RECT wnd_sz ) {
  wnd_obj obj[] = {
    wnd_obj_create( {                 0, 0, wnd_sz.right     , 25 }, COL_L_GRY, false ),
    wnd_obj_create( {                 0, 0, wnd_sz.right     , 24 }, COL_D_GRY, false ),
    wnd_obj_create( { wnd_sz.right - 25, 0, wnd_sz.right     , 25 }, COL_L_GRY, true  ),
    wnd_obj_create( { wnd_sz.right - 24, 0, wnd_sz.right     , 24 }, COL_D_GRY, false ),
    wnd_obj_create( { wnd_sz.right - 50, 0, wnd_sz.right - 25, 25 }, COL_L_GRY, true  ),
    wnd_obj_create( { wnd_sz.right - 49, 0, wnd_sz.right - 25, 24 }, COL_D_GRY, false ),
    wnd_obj_create( { wnd_sz.right - 75, 0, wnd_sz.right - 50, 25 }, COL_L_GRY, true  ),
    wnd_obj_create( { wnd_sz.right - 74, 0, wnd_sz.right - 50, 24 }, COL_D_GRY, false )
  };

  (void)SetBkMode( hdc, TRANSPARENT );
  (void)SetTextColor( hdc, COL_L_GRY );

  for( s32 idx = 0; idx < 8; ++idx ) {
    (void)FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    if( obj[ idx ].next ) {
      ++idx;
      (void)FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    }

#define if_idx_draw_txt( val, txt, xoff, yoff ) \
if( idx == val ) \
(void)TextOutW( hdc, obj[ idx ].r.right - xoff, obj[ idx ].r.bottom - yoff, txt, 1 )

    if_idx_draw_txt( 3, L"X", 16, 20 );
    if_idx_draw_txt( 5, L"O", 17, 20 );
    if_idx_draw_txt( 7, L"_", 16, 25 );
  }
  (void)TextOutW( hdc, 6, 4, L"Type", 4 );

  for( auto& it : obj )
    (void)DeleteObject( it.col );
}

void wnd_title_clicked_cls( HWND hwnd, bool mouse_over ) {
  if( !mouse_over )
    return;

  ExitProcess( 0 );
}

RECT max_prev_sz;
bool is_maxd = false;
void wnd_title_clicked_max( HWND hwnd, bool mouse_over ) {
  if( !mouse_over )
    return;

  static POINT prev_pos;
  if( !is_maxd ) {
    GetClientRect( hwnd, &max_prev_sz );
    prev_pos = {
      max_prev_sz.left,
      max_prev_sz.top
    };
    ClientToScreen( hwnd, &prev_pos );

    HMONITOR c_mon = MonitorFromWindow( hwnd, MONITOR_DEFAULTTONEAREST );
    MONITORINFO m_info;
    m_info.cbSize = sizeof( m_info );
    GetMonitorInfoW( c_mon, &m_info );
    s32 m_width  = m_info.rcMonitor.right - m_info.rcMonitor.left,
        m_height = m_info.rcWork.bottom - m_info.rcWork.top;

    SetWindowPos( hwnd, 0,
      m_info.rcMonitor.left,
      m_info.rcMonitor.top,
      m_width,
      m_height,
      SWP_NOZORDER
    );

    is_maxd = true;
  } else {
    SetWindowPos( hwnd, 0,
      prev_pos.x,
      prev_pos.y,
      max_prev_sz.right - max_prev_sz.left,
      max_prev_sz.bottom - max_prev_sz.top,
      SWP_NOZORDER
    );

    is_maxd = false;
  }
}

void wnd_title_clicked_min( HWND hwnd, bool mouse_over ) {
  if( !mouse_over )
    return;

  (void)ShowWindow( hwnd, SW_MINIMIZE );
}