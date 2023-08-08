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

void wnd_drag( HWND hwnd, POINT m_pos, RECT wnd_sz ) {
  if( user_dragging ) {
    POINT m_delta {
      m_pos.x - duser_start.x,
      m_pos.y - duser_start.y
    };

    RECT r_wnd;
    (void)GetWindowRect( hwnd, &r_wnd );
    // static bool was_maxed = false;
    // if isZoomed || was_maxed
    //   was_maxed = true;
    //   m_pos.x / r_wmd.right - r_wnd.left for %
    //   ShowWindow SW_RESTORE
    //   get new size pixels (2nd getwindowrect)
    //   mutliply the .right by the var we got above
    //   setwindowpos
    //   return;
    // }
    // was_maxed = false; maybe put this outside the user_dragging if statement if this doesn't work
    // so itll run everytime and for the entire time youre dragging after being maximized
    //   add early return to avoid conflicts
    // the y is fine

    (void)SetWindowPos( hwnd, 0,
      r_wnd.left + m_delta.x,
      r_wnd.top  + m_delta.y,
      0, 0,
      SWP_NOSIZE | SWP_NOZORDER
    );

    return;
  }
  duser_start.x = m_pos.x;
  duser_start.y = m_pos.y;
}