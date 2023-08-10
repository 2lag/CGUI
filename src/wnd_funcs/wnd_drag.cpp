#include "wnd_drag.h"

bool user_dragging = false;
POINT duser_start{};

void wnd_drag_on( HWND hwnd, RECT title_bar, POINT m_pos ) {
  if( PtInRect( &title_bar, duser_start ) ) {
    user_dragging = true;
    duser_start = { m_pos.x, m_pos.y };
    (void)SetCapture( hwnd );
  }
}

void wnd_drag_off() {
  if( user_dragging ) {
    user_dragging = false;
    (void)ReleaseCapture();
  }
}

void wnd_drag( HWND hwnd, POINT m_pos ) {
  if( !user_dragging ) {
    duser_start = m_pos;
    return;
  }

  POINT m_delta {
    m_pos.x - duser_start.x,
    m_pos.y - duser_start.y
  };

  RECT r_wnd;
  (void)GetWindowRect( hwnd, &r_wnd );

  HMONITOR c_mon = MonitorFromWindow( hwnd, MONITOR_DEFAULTTONEAREST );
  MONITORINFO i_mon;
  i_mon.cbSize = sizeof( i_mon );
  (void)GetMonitorInfoW( c_mon, &i_mon );
  s32 wnd_szx = r_wnd.right - r_wnd.left,
      wnd_szy = r_wnd.bottom - r_wnd.top,
      m_szx = i_mon.rcMonitor.right - i_mon.rcMonitor.left,
      m_szy = i_mon.rcWork.bottom - i_mon.rcWork.top;

  u32 swp_flags = SWP_NOSIZE | SWP_NOZORDER;
  POINT wnd_pos = {
    r_wnd.left + m_delta.x,
    r_wnd.top  + m_delta.y
  };

  if( wnd_szx == m_szx && wnd_szy == m_szy ) {
    swp_flags = SWP_NOZORDER;
    is_maxd = false;

    f32 wnd_xper = (f32)m_pos.x / (f32)wnd_szx;
    salt nwnd_szx = max_prev_sz.right - max_prev_sz.left;
    wnd_pos.x = duser_start.x = (salt)( (f32)nwnd_szx * wnd_xper );
  }

  (void)SetWindowPos( hwnd, 0,
    wnd_pos.x, wnd_pos.y,
    max_prev_sz.right - max_prev_sz.left,
    max_prev_sz.bottom - max_prev_sz.top,
    swp_flags
  );
}

void wnd_drag_max( HWND hwnd, POINT m_pos ) {
  POINT sm_pos;
  GetCursorPos( &sm_pos );

  HMONITOR c_mon = MonitorFromPoint( sm_pos, MONITOR_DEFAULTTONEAREST );
  MONITORINFO i_mon;
  i_mon.cbSize = sizeof( i_mon );
  (void)GetMonitorInfoW( c_mon, &i_mon );
  salt mon_szx = i_mon.rcWork.right - i_mon.rcWork.left,
       mon_szy = i_mon.rcWork.bottom - i_mon.rcWork.top;

  // figure out how to make it work across multiple monitors
  // its getting the proper size
  // the issue lies with sm_pos

  bool within_range = ( sm_pos.y <= i_mon.rcWork.top &&
                        sm_pos.x > salt( (f32)mon_szx * 0.2f ) &&
                        sm_pos.x < salt( (f32)mon_szx * 0.8f ) );

  if( within_range ) {
    is_maxd = true;

    GetClientRect( hwnd, &max_prev_sz );

    (void)SetWindowPos( hwnd, 0,
      i_mon.rcWork.left,
      i_mon.rcWork.top,
      mon_szx, mon_szy,
      SWP_NOZORDER
    );
  }
}

// also add half sizing dragging it to middle of either side ( same call location )
// also do top and bottom i guess for vertical monitors