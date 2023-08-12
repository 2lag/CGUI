#include "wnd_resize.h"

bool user_resizing = false;
POINT ruser_start{};

void wnd_resize_on( HWND hwnd, POINT m_pos, s32 d_side ) {
  if( d_side ) {
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

void wnd_resize( HWND hwnd, POINT m_pos, RECT wnd_sz, s32 d_side ) {
  if( !user_resizing ) {
    ruser_start = m_pos;
    return;
  }

  POINT wnd_pos {
    wnd_sz.left - 1,
    wnd_sz.top - 1
  };
  ClientToScreen( hwnd, &wnd_pos );

  POINT m_delta {
    m_pos.x - ruser_start.x,
    m_pos.y - ruser_start.y
  };

  s32 wnd_szx = wnd_sz.right - wnd_sz.left + 2,
      wnd_szy = wnd_sz.bottom - wnd_sz.top + 2;

  std::cout << m_delta.x << " " << m_delta.y << std::endl;
  // change cursor to respective drag direction
  if( d_side == 4 ) {
    wnd_pos.x += m_delta.x;
    wnd_szx -= m_delta.x;
  }
  if( d_side == 2 ) {
    wnd_szx += m_delta.x;
  }
  // figure out how to make d_side stick around
  //   so it doesnt toggle off if dragged outside the zone when already dragging
  SetWindowPos( hwnd, 0,
    wnd_pos.x, wnd_pos.y,
    wnd_szx, wnd_szy,
    SWP_NOZORDER
  );
}

// add function for if titlebar clicked twice, maximize/minimize depending on size