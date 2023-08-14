#pragma warning( disable : 26812 )
#include "wnd_resize.h"

bool user_resizing = false;
POINT ruser_start{};

void wnd_resize_on( HWND hwnd, POINT m_pos, s32 d_side ) {
  if( d_side != 0 ) {
    user_resizing = true;
    ruser_start = m_pos;
    SetCapture( hwnd );
  }
}

void wnd_resize_off() {
  if( user_resizing ) {
    user_resizing = false;
    ReleaseCapture();
  }
}

void wnd_resize_get_side( s32 &d_side, POINT m_pos, RECT wnd_sz ) {
  salt wnd_szx = wnd_sz.right - wnd_sz.left,
       wnd_szy = wnd_sz.bottom - wnd_sz.top;

  bool on_left   = ( m_pos.x <= 5 ),
       on_top    = ( m_pos.y <= 5 ),
       on_right  = ( m_pos.x >= wnd_szx - 5 ),
       on_bottom = ( m_pos.y >= wnd_szy - 5 ),
       in_hcenter = ( !on_right && !on_left ),
       in_vcenter = ( !on_bottom && !on_top );

  HCURSOR cur_nesw = LoadCursorW( 0, IDC_SIZENESW ),
          cur_nwse = LoadCursorW( 0, IDC_SIZENWSE ),
          cur_ns   = LoadCursorW( 0, IDC_SIZENS   ),
          cur_we   = LoadCursorW( 0, IDC_SIZEWE   ),
          cur_def  = LoadCursorW( 0, IDC_ARROW    );

  if( on_top ) {
    d_side = in_hcenter ? EDGE_TOP : ( on_left ? EDGE_TOP_LEFT : EDGE_TOP_RIGHT );
    SetCursor( in_hcenter ? cur_ns : ( on_left ? cur_nwse : cur_nesw ) );
  } else if( on_bottom ) {
    d_side = in_hcenter ? EDGE_BOTTOM : ( on_left ? EDGE_BOTTOM_LEFT : EDGE_BOTTOM_RIGHT );
    SetCursor( in_hcenter ? cur_ns : ( on_left ? cur_nesw : cur_nwse ) );
  } else if( on_left ) {
    d_side = in_vcenter ? EDGE_LEFT : EDGE_TOP_LEFT;
    SetCursor( in_vcenter ? cur_we : cur_nesw );
  } else if( on_right ) {
    d_side = in_vcenter ? EDGE_RIGHT : EDGE_BOTTOM_RIGHT;
    SetCursor( in_vcenter ? cur_we : cur_nwse );
  } else {
    d_side = 0;
    SetCursor( cur_def );
  }
}

void wnd_resize( HWND hwnd, POINT m_pos, RECT wnd_sz, s32 d_side ) {
  if( !d_side || !user_resizing ) {
    ruser_start = m_pos;
    return;
  }

  POINT pwnd_sz {
    wnd_sz.right - wnd_sz.left + 2,
    wnd_sz.bottom - wnd_sz.top + 2
  },
  m_delta {
    m_pos.x - ruser_start.x,
    m_pos.y - ruser_start.y
  },
  wnd_pos {
    wnd_sz.left - 1,
    wnd_sz.top  - 1
  };
  ClientToScreen( hwnd, &wnd_pos );

  auto wnd_adj_pos_sz = [&]( POINT p, POINT s ) {
    wnd_pos += p;
    pwnd_sz += s;
  };

  // make it stay dragging current side until mouse released
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

  // * DO THIS ON PC CUZ LAPPYTOPPY ONLY GOT 1 SCREEN DUR *
  // get current monitor from screen mouse pos & make it so it cant be resized off monitor

  if( pwnd_sz.x < 200 ) {
    pwnd_sz.x = 200;
    wnd_pos.x -= ( m_delta.x > 0 ) ? m_delta.x : 0;
  }
  if( pwnd_sz.y < 200 ) {
    pwnd_sz.y = 200;
    wnd_pos.y -= ( m_delta.y > 0 ) ? m_delta.y : 0;
  }

  SetWindowPos( hwnd, 0,
    wnd_pos.x, wnd_pos.y,
    pwnd_sz.x, pwnd_sz.y,
    SWP_NOZORDER
  );
}

// figure out why this isn't working
void wnd_resize_title( HWND hwnd, RECT title_bar, POINT m_pos ) {
  if( !PtInRect( &title_bar, m_pos ) )
    return;

  if( !is_maxd ) {
    GetClientRect( hwnd, &max_prev_sz );
    max_prev_pos = {
      max_prev_sz.left,
      max_prev_sz.top
    };
    ClientToScreen( hwnd, &max_prev_pos );

    HMONITOR c_mon = MonitorFromWindow( hwnd, MONITOR_DEFAULTTONEAREST );
    MONITORINFO m_info;
    m_info.cbSize = sizeof( m_info );
    GetMonitorInfoW( c_mon, &m_info );
    s32 m_width  = m_info.rcWork.right - m_info.rcWork.left,
        m_height = m_info.rcWork.bottom - m_info.rcWork.top;

    is_maxd = true;

    SetWindowPos( hwnd, 0,
      m_info.rcWork.left,
      m_info.rcWork.top,
      m_width,
      m_height,
      SWP_NOZORDER
    );
  } else {
    is_maxd = false;

    HMONITOR c_mon = MonitorFromWindow( hwnd, MONITOR_DEFAULTTONEAREST );
    MONITORINFO i_mon;
    i_mon.cbSize = sizeof( i_mon );
    GetMonitorInfoW( c_mon, &i_mon );
    salt mon_szx = i_mon.rcWork.right - i_mon.rcWork.left,
         mon_szy = i_mon.rcWork.bottom - i_mon.rcWork.top;

    RECT r_wnd;
    GetClientRect( hwnd, &r_wnd );
    salt wnd_szx = max_prev_sz.right - max_prev_sz.left,
         wnd_szy = max_prev_sz.bottom - max_prev_sz.top;

    if( max_prev_pos.x == 0 || max_prev_pos.y == 0 ) {
      max_prev_pos = {
        ( mon_szx / 2 ) - ( wnd_szx / 2 ),
        ( mon_szy / 2 ) - ( wnd_szy / 2 )
      };
    }

    SetWindowPos( hwnd, 0,
      max_prev_pos.x,
      max_prev_pos.y,
      wnd_szx,
      wnd_szy,
      SWP_NOZORDER
    );
  }
}