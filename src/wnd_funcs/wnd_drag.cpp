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
  MONITORINFO m_info;
  m_info.cbSize = sizeof( m_info );
  (void)GetMonitorInfoW( c_mon, &m_info );
  s32 wnd_szx = r_wnd.right - r_wnd.left,
      wnd_szy = r_wnd.bottom - r_wnd.top,
      m_szx = m_info.rcMonitor.right - m_info.rcMonitor.left,
      m_szy = m_info.rcWork.bottom - m_info.rcWork.top;

  u32 swp_flags = SWP_NOSIZE | SWP_NOZORDER;
  POINT wnd_pos = {
    r_wnd.left + m_delta.x,
    r_wnd.top  + m_delta.y
  };

  if( wnd_szx == m_szx && wnd_szy == m_szy ) {
    swp_flags = SWP_NOZORDER;
    is_maxd = false;

    f32 wnd_xper = (f32)m_pos.x / (f32)wnd_szx;
    s32 nwnd_szx = max_prev_sz.right - max_prev_sz.left;
    wnd_pos.x = (salt)( (f32)nwnd_szx * wnd_xper );
    duser_start.x = (salt)( (f32)nwnd_szx * wnd_xper );
  }

  (void)SetWindowPos( hwnd, 0,
    wnd_pos.x, wnd_pos.y,
    max_prev_sz.right - max_prev_sz.left,
    max_prev_sz.bottom - max_prev_sz.top,
    swp_flags
  );
}

// add functionality for maximizing window by dragging it to the top and letting go ( maybe call in lmb up ? )
// also half sizing dragging it to middle of either side