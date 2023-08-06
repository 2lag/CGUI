#include "wnd_drag.h"

bool user_dragging = false;
POINT user_start{};

void wnd_drag_on( HWND hwnd, RECT title_bar, POINT m_pos ) {
  if( PtInRect( &title_bar, user_start ) ) {
    user_dragging = true;
    user_start = { m_pos.x, m_pos.y };
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
      m_pos.x - user_start.x,
      m_pos.y - user_start.y
    };

    RECT r_wnd;
    (void)GetWindowRect( hwnd, &r_wnd );
    (void)SetWindowPos( hwnd, 0,
      r_wnd.left + m_delta.x,
      r_wnd.top  + m_delta.y,
      0, 0,
      SWP_NOSIZE | SWP_NOZORDER
    );
    user_start.x += m_delta.x;
    user_start.y += m_delta.y;
    return;
  }
  user_start.x = m_pos.x;
  user_start.y = m_pos.y;
}