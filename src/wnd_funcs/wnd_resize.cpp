#include "wnd_resize.h"

bool user_resizing = false;
POINT ruser_start{};

void wnd_resize_on( HWND hwnd, POINT m_pos, bool in_rt ) {
  if( in_rt ) {
    user_resizing = true;
    ruser_start = m_pos;
    (void)SetCapture( hwnd );
  }
}

void wnd_resize_off( ) {
  if( user_resizing ) {
    user_resizing = false;
    (void)ReleaseCapture();
  }
}

void wnd_resize( HWND hwnd, POINT m_pos, RECT wnd_sz ) {
  if( !user_resizing ) {
    ruser_start = m_pos;
    return;
  }

  u32 flags =  SWP_NOZORDER; // set SWP_NOMOVE when changing right/bottom

  POINT wnd_pos {
    wnd_sz.left,
    wnd_sz.top - 1
  };
  ClientToScreen( hwnd, &wnd_pos );

  POINT m_delta {
    m_pos.x - ruser_start.x,
    m_pos.y - ruser_start.y
  };

  s32 wnd_szx = wnd_sz.right - wnd_sz.left + 1,
      wnd_szy = wnd_sz.bottom - wnd_sz.top + 2;

  std::cout << m_delta.x << " " << m_delta.y << std::endl;
  // change cursor to respective drag direction
  if( m_pos.x <= 2 ) {
    wnd_pos.x += m_delta.x;
    if( m_delta.x < 0 )
      wnd_szx -= m_delta.x;
  }
  // fix resizing shaking right side
  // also fix it lagging behind
  SetWindowPos( hwnd, 0,
    wnd_pos.x, wnd_pos.y,
    wnd_szx, wnd_szy,
    flags
  );
}

// add function for if titlebar clicked twice, maximize/minimize depending on size