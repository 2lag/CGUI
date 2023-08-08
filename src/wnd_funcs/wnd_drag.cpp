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
  if( user_dragging ) {
    POINT m_delta {
      m_pos.x - duser_start.x,
      m_pos.y - duser_start.y
    };

    RECT r_wnd;
    (void)GetWindowRect( hwnd, &r_wnd );

    (void)SetWindowPos( hwnd, 0,
      r_wnd.left + m_delta.x,
      r_wnd.top  + m_delta.y,
      0, 0,
      SWP_NOSIZE | SWP_NOZORDER
    );

    // make this work custom so itll work properly
    // store previous size before maximizing and pass between
    // build out pseudocode/structure here first after fixing wnd_title.cpp stuff
    if( IsZoomed( hwnd ) ) {
      (void)ShowWindow( hwnd, SW_RESTORE );
    }
    
    return;
  }
  duser_start.x = m_pos.x;
  duser_start.y = m_pos.y;
}