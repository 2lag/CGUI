#include "wnd_drag.h"

bool user_dragging = false;
POINT duser_start{};

void wnd_drag_on( HWND hwnd, RECT title_bar, POINT m_pos ) {
  if( PtInRect( &title_bar, duser_start ) ) {
    user_dragging = true;
    duser_start = m_pos;
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
      m_szx = i_mon.rcWork.right - i_mon.rcWork.left,
      m_szy = i_mon.rcWork.bottom - i_mon.rcWork.top;

  u32 swp_flags = SWP_NOSIZE | SWP_NOZORDER;
  POINT wnd_pos = {
    r_wnd.left + m_delta.x,
    r_wnd.top  + m_delta.y
  };

  if( wnd_szx == m_szx && wnd_szy == m_szy ) {
    swp_flags = SWP_NOZORDER;
    f32 wnd_xper = (f32)m_pos.x / (f32)wnd_szx;
    salt nwnd_szx = max_prev_sz.right - max_prev_sz.left;
    wnd_pos.x = duser_start.x = (salt)( (f32)nwnd_szx * wnd_xper );
    is_maxd = false;
  }

  (void)SetWindowPos( hwnd, 0,
    wnd_pos.x, wnd_pos.y,
    max_prev_sz.right - max_prev_sz.left,
    max_prev_sz.bottom - max_prev_sz.top,
    swp_flags
  );
}
// ( not having this issue on laptop rm if fixed on pc )
// fix bug w/ pressin max button out of dragged max on side screens
// also fix size not resetting when dragged out of maximize for some mf reason :|
void wnd_drag_max( HWND hwnd, POINT m_pos ) {
  POINT sm_pos;
  (void)GetCursorPos( &sm_pos );

  HMONITOR c_mon = MonitorFromPoint( sm_pos, MONITOR_DEFAULTTONEAREST );
  static HMONITOR pc_mon = nullptr;
  static RECT pm_rect{};

  if( c_mon != pc_mon ) {
    MONITORINFO i_mon;
    i_mon.cbSize = sizeof( i_mon );
    (void)GetMonitorInfoW( c_mon, &i_mon );

    if( !EqualRect( &i_mon.rcWork, &pm_rect ) ) {
      pm_rect = i_mon.rcWork;
      pc_mon = c_mon;
    }
  }

  MONITORINFO i_mon;
  i_mon.cbSize = sizeof( i_mon );
  (void)GetMonitorInfoW( pc_mon, &i_mon );
  salt mon_szx = i_mon.rcWork.right - i_mon.rcWork.left,
      pmon_szx = pm_rect.right - pm_rect.left,
       mon_szy = i_mon.rcWork.bottom - i_mon.rcWork.top,
      pmon_szy = pm_rect.bottom - pm_rect.top;

  salt sm_pos_adj;
  if( sm_pos.x < 0 )
    sm_pos_adj = sm_pos.x + pmon_szx;
  else if( sm_pos.x > pmon_szx )
    sm_pos_adj = sm_pos.x - ( pm_rect.right - mon_szx );
  else
    sm_pos_adj = sm_pos.x;

  bool within_range = ( sm_pos.y <= i_mon.rcWork.top &&
                        sm_pos_adj > salt( (f32)mon_szx * 0.2f ) &&
                        sm_pos_adj < salt( (f32)mon_szx * 0.8f ) );

  if( within_range ) {
    is_maxd = true;

    (void)GetClientRect( hwnd, &max_prev_sz );

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