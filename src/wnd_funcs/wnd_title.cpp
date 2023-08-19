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

  SetBkMode( hdc, TRANSPARENT );
  SetTextColor( hdc, COL_L_GRY );

  for( s32 idx = 0; idx < 8; ++idx ) {
    FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    if( obj[ idx ].next ) {
      ++idx;
      FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    }

#define if_idx_draw_txt( val, txt, xoff, yoff ) \
if( idx == val ) \
TextOutW( hdc, obj[ idx ].r.right - xoff, obj[ idx ].r.bottom - yoff, txt, 1 )

    if_idx_draw_txt( 3, L"X", 16, 20 );
    if_idx_draw_txt( 5, L"O", 17, 20 );
    if_idx_draw_txt( 7, L"_", 16, 25 );
  }
  TextOutW( hdc, 6, 4, L"Type", 4 );

  for( auto& it : obj )
    DeleteObject( it.col );
}

void wnd_title_cls( HWND hwnd, bool mouse_over ) {
  if( mouse_over )
    ExitProcess( 0 );
}

RECT max_prev_sz;
POINT max_prev_pos;
bool is_maxd = false;
void wnd_title_max( HWND hwnd, bool mouse_over ) {
  if( !mouse_over )
    return;

  HMONITOR c_mon = MonitorFromWindow( hwnd,
    is_maxd ? MONITOR_DEFAULTTOPRIMARY : MONITOR_DEFAULTTONEAREST
  );
  get_monitor_info( c_mon );

  POINT mon_sz { 
    i_mon.rcWork.right - i_mon.rcWork.left,
    i_mon.rcWork.bottom - i_mon.rcWork.top
  },
  nwnd_ps{},
  nwnd_sz{};

  if( !is_maxd ) {
    GetClientRect( hwnd, &max_prev_sz );
    max_prev_pos = {
      max_prev_sz.left,
      max_prev_sz.top
    };
    ClientToScreen( hwnd, &max_prev_pos );

    nwnd_ps = {
      i_mon.rcWork.left,
      i_mon.rcWork.top
    },
    nwnd_sz = mon_sz;
  } else {
    POINT wnd_sz {
      max_prev_sz.right - max_prev_sz.left,
      max_prev_sz.bottom - max_prev_sz.top
    };

    if( !max_prev_pos )
      max_prev_pos = ( mon_sz - wnd_sz ) / 2;

    nwnd_ps = max_prev_pos,
    nwnd_sz = wnd_sz;
  }
  is_maxd = !is_maxd;

  SetWindowPos( hwnd, 0,
    nwnd_ps.x, nwnd_ps.y,
    nwnd_sz.x, nwnd_sz.y,
    SWP_NOZORDER
  );
}

void wnd_title_min( HWND hwnd, bool mouse_over ) {
  if( mouse_over )
    ShowWindow( hwnd, SW_MINIMIZE );
}