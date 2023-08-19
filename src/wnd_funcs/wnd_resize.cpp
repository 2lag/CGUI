#pragma warning( disable : 26812 )
#include "wnd_resize.h"

bool user_resizing = false;
POINT ruser_start{};
s32 d_side{};

void wnd_resize_get_side( POINT m_pos, RECT wnd_sz ) {
  POINT pwn_sz {
    wnd_sz.right - wnd_sz.left,
    wnd_sz.bottom - wnd_sz.top
  };

  bool on_left   = ( m_pos.x <= 5 ),
       on_top    = ( m_pos.y <= 5 ),
       on_right  = ( m_pos.x >= pwn_sz.x - 5 ),
       on_bottom = ( m_pos.y >= pwn_sz.y - 5 ),
       in_hcenter =  ( !on_right && !on_left ),
       in_vcenter =  ( !on_bottom && !on_top );

  if( on_top )
    d_side = in_hcenter ? EDGE_TOP : ( on_left ? EDGE_TOP_LEFT : EDGE_TOP_RIGHT );
  else if( on_bottom )
    d_side = in_hcenter ? EDGE_BOTTOM : ( on_left ? EDGE_BOTTOM_LEFT : EDGE_BOTTOM_RIGHT );
  else if( on_left )
    d_side = in_vcenter ? EDGE_LEFT : EDGE_TOP_LEFT;
  else if( on_right )
    d_side = in_vcenter ? EDGE_RIGHT : EDGE_BOTTOM_RIGHT;
  else
    d_side = 0;
}

void wnd_resize_get_cursor( POINT m_pos, RECT wnd_sz ) {
  POINT pwn_sz {
    wnd_sz.right - wnd_sz.left,
    wnd_sz.bottom - wnd_sz.top
  };

  bool on_left   = ( m_pos.x <= 5 ),
       on_top    = ( m_pos.y <= 5 ),
       on_right  = ( m_pos.x >= pwn_sz.x - 5 ),
       on_bottom = ( m_pos.y >= pwn_sz.y - 5 ),
       in_hcenter =  ( !on_right && !on_left ),
       in_vcenter =  ( !on_bottom && !on_top );

  HCURSOR cur_nesw = LoadCursorW( 0, IDC_SIZENESW ),
          cur_nwse = LoadCursorW( 0, IDC_SIZENWSE ),
          cur_ns   = LoadCursorW( 0, IDC_SIZENS   ),
          cur_we   = LoadCursorW( 0, IDC_SIZEWE   ),
          cur_def  = LoadCursorW( 0, IDC_ARROW    );

  if( on_top )
    SetCursor( in_hcenter ? cur_ns : ( on_left ? cur_nwse : cur_nesw ) );
  else if( on_bottom )
    SetCursor( in_hcenter ? cur_ns : ( on_left ? cur_nesw : cur_nwse ) );
  else if( on_left )
    SetCursor( in_vcenter ? cur_we : cur_nesw );
  else if( on_right )
    SetCursor( in_vcenter ? cur_we : cur_nwse );
  else
    SetCursor( cur_def );
}

void wnd_resize_on( HWND hwnd, POINT m_pos, RECT wnd_sz ) {
  wnd_resize_get_side( m_pos, wnd_sz );
  if( d_side == 0 )
    return;

  user_resizing = true;
  ruser_start = m_pos;
  SetCapture( hwnd );
}

void wnd_resize_off() {
  if( !user_resizing )
    return;
  
  user_resizing = false;
  ReleaseCapture();
}

void wnd_resize( HWND hwnd, POINT m_pos, RECT wnd_sz ) {
  if( !d_side || !user_resizing ) {
    ruser_start = m_pos;
    return;
  }

  POINT _wnd_sz {
    wnd_sz.right - wnd_sz.left + 2,
    wnd_sz.bottom - wnd_sz.top + 2
  }, m_delta {
    m_pos.x - ruser_start.x,
    m_pos.y - ruser_start.y
  }, wnd_pos {
    wnd_sz.left - 1,
    wnd_sz.top  - 1
  };
  ClientToScreen( hwnd, &wnd_pos );

  auto wnd_adj_pos_sz = [&]( POINT pos, POINT size ) {
    wnd_pos += pos;
    _wnd_sz += size;
  };

  switch( d_side ) {
  case EDGE_TOP_LEFT:
    wnd_adj_pos_sz( m_delta, -m_delta );
    break;
  case EDGE_TOP:
    wnd_adj_pos_sz( { 0, m_delta.y }, { 0, -m_delta.y } );
    break;
  case EDGE_TOP_RIGHT:
    wnd_adj_pos_sz( { 0, m_delta.y }, { m_delta.x, -m_delta.y } );
    ruser_start.x = m_pos.x;
    break;
  case EDGE_RIGHT:
    wnd_adj_pos_sz( {},{ m_delta.x, 0 } );
    ruser_start.x = m_pos.x;
    break;
  case EDGE_BOTTOM_RIGHT:
    wnd_adj_pos_sz( {}, m_delta );
    ruser_start = m_pos;
    break;
  case EDGE_BOTTOM:
    wnd_adj_pos_sz( {}, { 0, m_delta.y } );
    ruser_start.y = m_pos.y;
    break;
  case EDGE_BOTTOM_LEFT:
    wnd_adj_pos_sz( { m_delta.x, 0 }, { -m_delta.x, m_delta.y } );
    ruser_start.y = m_pos.y;
    break;
  case EDGE_LEFT:
    wnd_adj_pos_sz( { m_delta.x, 0 }, { -m_delta.x, 0 } );
    break;
  }

  HMONITOR c_mon = MonitorFromWindow( hwnd, MONITOR_DEFAULTTONEAREST );
  get_monitor_info( c_mon );

  if( wnd_pos.x < i_mon.rcWork.left ) {
    wnd_pos.x = i_mon.rcWork.left;
    _wnd_sz.x += m_delta.x;
  }
  if( wnd_pos.y < i_mon.rcWork.top ) {
    wnd_pos.y = i_mon.rcWork.top;
    _wnd_sz.y += m_delta.y;
  }
  if( wnd_pos.x + _wnd_sz.x >= i_mon.rcWork.right )
    _wnd_sz.x = i_mon.rcWork.right - wnd_pos.x;
  if( wnd_pos.y + _wnd_sz.y >= i_mon.rcWork.bottom )
    _wnd_sz.y = i_mon.rcWork.bottom - wnd_pos.y;

  if( _wnd_sz.x < 200 ) {
    _wnd_sz.x = 200;
    wnd_pos.x -= ( m_delta.x > 0 ) ? m_delta.x : 0;
  }
  if( _wnd_sz.y < 200 ) {
    _wnd_sz.y = 200;
    wnd_pos.y -= ( m_delta.y > 0 ) ? m_delta.y : 0;
  }

  SetWindowPos( hwnd, 0,
    wnd_pos.x, wnd_pos.y,
    _wnd_sz.x, _wnd_sz.y,
    SWP_NOZORDER
  );
}

void wnd_resize_title( HWND hwnd, bool mouse_over ) {
  if( !mouse_over )
    return;

  HMONITOR c_mon = MonitorFromWindow( hwnd, MONITOR_DEFAULTTONEAREST );
  get_monitor_info( c_mon );

  POINT mon_sz {
    i_mon.rcWork.right - i_mon.rcWork.left,
    i_mon.rcWork.bottom - i_mon.rcWork.top
  };

  if( !is_maxd ) {
    is_maxd = true;

    GetClientRect( hwnd, &max_prev_sz );
    max_prev_pos = {
      max_prev_sz.left,
      max_prev_sz.top
    };
    ClientToScreen( hwnd, &max_prev_pos );

    SetWindowPos( hwnd, 0,
      i_mon.rcWork.left,
      i_mon.rcWork.top,
      mon_sz.x, mon_sz.y,
      SWP_NOZORDER
    );
    
    return;
  }
  is_maxd = false;

  RECT r_wnd;
  GetClientRect( hwnd, &r_wnd );
  POINT wnd_sz {
    max_prev_sz.right - max_prev_sz.left,
    max_prev_sz.bottom - max_prev_sz.top
  };

  if( !max_prev_pos )
    max_prev_pos = ( mon_sz - wnd_sz ) / 2;

  SetWindowPos( hwnd, 0,
    max_prev_pos.x,
    max_prev_pos.y,
    wnd_sz.x, wnd_sz.y,
    SWP_NOZORDER
  );
}