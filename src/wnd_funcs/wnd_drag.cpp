#include "wnd_drag.h"

bool user_dragging = false;
POINT duser_start{};

void wnd_drag_on( HWND hwnd, POINT m_pos, bool mouse_over ) {
  if( mouse_over ) {

    if( !is_maxd ) {
      GetClientRect( hwnd, &max_prev_sz );
      max_prev_pos = {
        max_prev_sz.left,
        max_prev_sz.top
      };
      ClientToScreen( hwnd, &max_prev_pos );
    }

    user_dragging = true;
    duser_start = m_pos;
    SetCapture( hwnd );
  }
} 

void wnd_drag_off() {
  if( user_dragging ) {
    user_dragging = false;
    ReleaseCapture();
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
  GetWindowRect( hwnd, &r_wnd );

  HMONITOR c_mon = MonitorFromWindow( hwnd, MONITOR_DEFAULTTONEAREST );
  MONITORINFO i_mon;
  i_mon.cbSize = sizeof( i_mon );
  GetMonitorInfoW( c_mon, &i_mon );
  s32 wnd_szx = r_wnd.right - r_wnd.left,
      wnd_szy = r_wnd.bottom - r_wnd.top,
      m_szx = i_mon.rcWork.right - i_mon.rcWork.left,
      m_szy = i_mon.rcWork.bottom - i_mon.rcWork.top;

  u32 swp_flags = SWP_NOSIZE | SWP_NOZORDER;
  POINT wnd_pos = {
    r_wnd.left + m_delta.x,
    r_wnd.top  + m_delta.y
  };

  bool is_fullscreen = wnd_szx == m_szx && wnd_szy == m_szy,
       is_halfscreen = wnd_szx == ( m_szx / 2 ) && wnd_szy == m_szy;
  if( is_fullscreen || is_halfscreen ) {
    swp_flags = SWP_NOZORDER;
    f32 wnd_xper = (f32)m_pos.x / (f32)wnd_szx;
    salt nwnd_szx = max_prev_sz.right - max_prev_sz.left;
    wnd_pos.x = duser_start.x = (salt)( (f32)nwnd_szx * wnd_xper );
    is_maxd = false;
  }

  SetWindowPos( hwnd, 0,
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
  static HMONITOR pc_mon = nullptr;
  static RECT pm_rect{};

  if( c_mon != pc_mon ) {
    MONITORINFO i_mon;
    i_mon.cbSize = sizeof( i_mon );
    GetMonitorInfoW( c_mon, &i_mon );

    if( !EqualRect( &i_mon.rcWork, &pm_rect ) ) {
      pm_rect = i_mon.rcWork;
      pc_mon = c_mon;
    }
  }

  MONITORINFO i_mon;
  i_mon.cbSize = sizeof( i_mon );
  GetMonitorInfoW( pc_mon, &i_mon );
  POINT mon_sz {
    i_mon.rcWork.right - i_mon.rcWork.left,
    i_mon.rcWork.bottom - i_mon.rcWork.top
  },
       pmon_sz {
    pm_rect.right - pm_rect.left,
    pm_rect.bottom - pm_rect.top
  };

  salt sm_pos_adj;
  if( sm_pos.x < 0 )
    sm_pos_adj = sm_pos.x + pmon_sz.x;
  else if( sm_pos.x > pmon_sz.x )
    sm_pos_adj = sm_pos.x - ( pm_rect.right - mon_sz.x );
  else
    sm_pos_adj = sm_pos.x;

  bool within_range = ( sm_pos.y <= i_mon.rcWork.top &&
                        sm_pos_adj > salt( (f32)mon_sz.x * 0.2f ) &&
                        sm_pos_adj < salt( (f32)mon_sz.x * 0.8f ) );

  if( within_range ) {
    is_maxd = true;

    GetClientRect( hwnd, &max_prev_sz );

    SetWindowPos( hwnd, 0,
      i_mon.rcWork.left,
      i_mon.rcWork.top,
      mon_sz.x, mon_sz.y,
      SWP_NOZORDER
    );
  }
}

void wnd_drag_half( HWND hwnd, POINT m_pos ) {
  POINT sm_pos;
  GetCursorPos( &sm_pos );

  HMONITOR c_mon = MonitorFromPoint( sm_pos, MONITOR_DEFAULTTONEAREST );
  static HMONITOR pc_mon = nullptr;
  static RECT pm_rect{};
  static POINT monitor_offset{};

  if( c_mon != pc_mon ) {
    MONITORINFO i_mon;
    i_mon.cbSize = sizeof( i_mon );
    GetMonitorInfoW( c_mon, &i_mon );

    if( !EqualRect( &i_mon.rcWork, &pm_rect ) ) {
      pm_rect = i_mon.rcWork;
      pc_mon = c_mon;

      monitor_offset.x = i_mon.rcWork.left;
      monitor_offset.y = i_mon.rcWork.top;
    }
  }

  MONITORINFO i_mon;
  i_mon.cbSize = sizeof( i_mon );
  GetMonitorInfoW( pc_mon, &i_mon );
  POINT mon_sz {
    i_mon.rcWork.right - i_mon.rcWork.left,
    i_mon.rcWork.bottom - i_mon.rcWork.top
  },
    pmon_sz {
    pm_rect.right - pm_rect.left,
    pm_rect.bottom - pm_rect.top
  };

  POINT sm_pos_adj;
  if( sm_pos.x < 0 )
    sm_pos_adj.x = sm_pos.x + pmon_sz.x;
  else if( sm_pos.x > pmon_sz.x )
    sm_pos_adj.x = sm_pos.x - ( pm_rect.right - mon_sz.x );
  else
    sm_pos_adj.x = sm_pos.x;

  sm_pos_adj.y = sm_pos.y - monitor_offset.y;

  bool within_left_range  = ( sm_pos_adj.y >= mon_sz.y * 0.2f &&
                              sm_pos_adj.y <= mon_sz.y * 0.8f &&
                              sm_pos_adj.x <= 20 ),
       within_right_range = ( sm_pos_adj.y >= mon_sz.y * 0.2f &&
                              sm_pos_adj.y <= mon_sz.y * 0.8f &&
                              sm_pos_adj.x >= mon_sz.x - 20 );

  if( within_left_range || within_right_range ) {
    is_maxd = true;

    GetClientRect( hwnd, &max_prev_sz );

    if( within_left_range ) {
      SetWindowPos( hwnd, 0,
        i_mon.rcWork.left,
        i_mon.rcWork.top,
        mon_sz.x / 2, mon_sz.y,
        SWP_NOZORDER
      );
    } else {
      SetWindowPos( hwnd, 0,
        i_mon.rcWork.right - mon_sz.x / 2,
        i_mon.rcWork.top,
        mon_sz.x / 2, mon_sz.y,
        SWP_NOZORDER
      );
    }
  }
}

void wnd_drag_quart( HWND hwnd, POINT m_pos ) {
  POINT sm_pos;
  GetCursorPos( &sm_pos );

  HMONITOR c_mon = MonitorFromPoint( sm_pos, MONITOR_DEFAULTTONEAREST );
  static HMONITOR pc_mon = nullptr;
  static RECT pm_rect{};
  static POINT monitor_offset{};

  if( c_mon != pc_mon ) {
    MONITORINFO i_mon;
    i_mon.cbSize = sizeof( i_mon );
    GetMonitorInfoW( c_mon, &i_mon );

    if( !EqualRect( &i_mon.rcWork, &pm_rect ) ) {
      pm_rect = i_mon.rcWork;
      pc_mon = c_mon;

      monitor_offset.x = i_mon.rcWork.left;
      monitor_offset.y = i_mon.rcWork.top;
    }
  }

  MONITORINFO i_mon;
  i_mon.cbSize = sizeof( i_mon );
  GetMonitorInfoW( pc_mon, &i_mon );

  POINT mon_sz {
    i_mon.rcWork.right - i_mon.rcWork.left,
    i_mon.rcWork.bottom - i_mon.rcWork.top
  },
    pmon_sz {
    pm_rect.right - pm_rect.left,
    pm_rect.bottom - pm_rect.top
  };

  POINT sm_pos_adj{};
  if( sm_pos.x < 0 )
    sm_pos_adj.x = sm_pos.x + pmon_sz.x;
  else if( sm_pos.x > pmon_sz.x )
    sm_pos_adj.x = sm_pos.x - ( pm_rect.right - mon_sz.x );
  else
    sm_pos_adj.x = sm_pos.x;

  sm_pos_adj.y = sm_pos.y - monitor_offset.y;

  std::cout << sm_pos_adj.x << " " << sm_pos_adj.y << std::endl;

  bool within_tl_range = ( sm_pos_adj.y <= mon_sz.y * 0.2f &&
                           sm_pos_adj.x <= mon_sz.x * 0.2f ),
       within_tr_range = ( sm_pos_adj.y <= mon_sz.y * 0.2f &&
                           sm_pos_adj.x >= mon_sz.x * 0.8f ),
       within_br_range = ( sm_pos_adj.y >= mon_sz.y * 0.8f &&
                           sm_pos_adj.x >= mon_sz.x * 0.8f ),
       within_bl_range = ( sm_pos_adj.y >= mon_sz.y * 0.8f &&
                           sm_pos_adj.x <= mon_sz.x * 0.2f ),
       within_range    = ( within_tl_range || within_tr_range ||
                           within_bl_range || within_br_range );

  if( within_range ) {
    is_maxd = true;

    GetClientRect( hwnd, &max_prev_sz );

    if( within_tl_range ) {
      SetWindowPos( hwnd, 0,
        i_mon.rcWork.left,
        i_mon.rcWork.top,
        mon_sz.x / 2, mon_sz.y / 2,
        SWP_NOZORDER
      );
    }
    if( within_tr_range ) {
      SetWindowPos( hwnd, 0,
        i_mon.rcWork.right - mon_sz.x / 2,
        i_mon.rcWork.top,
        mon_sz.x / 2, mon_sz.y / 2,
        SWP_NOZORDER
      );
    }
    if( within_br_range ) {
      SetWindowPos( hwnd, 0,
        i_mon.rcWork.right - mon_sz.x / 2,
        ( mon_sz.y / 2 ) + monitor_offset.y,
        mon_sz.x / 2, mon_sz.y / 2,
        SWP_NOZORDER
      );
    }
    if( within_bl_range ) {
      SetWindowPos( hwnd, 0,
        i_mon.rcWork.left,
        ( mon_sz.y / 2 ) + monitor_offset.y,
        mon_sz.x / 2, mon_sz.y / 2,
        SWP_NOZORDER
      );
    }
  }
}