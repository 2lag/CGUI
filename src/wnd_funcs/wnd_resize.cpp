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
  if( m_pos.x <= 5 )
    d_side = 4;
  else if( m_pos.x >= wnd_sz.right - 5 )
    d_side = 2;
  else if( m_pos.y <= 5 )
    d_side = 1;
  else if( m_pos.y >= wnd_sz.bottom - 5 )
    d_side = 3;
  else
    d_side = 0;
}
// make sure window cant be dragged off edge
// make edge stick to cursor better
// then add corner drag functionality ( ptinrect ? )
void wnd_resize( HWND hwnd, POINT m_pos, s32 d_side ) {
  if( !d_side || !user_resizing ) {
    ruser_start = m_pos;
    return;
  }

  RECT wnd_sz{};
  GetClientRect( hwnd, &wnd_sz );

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
  
#if _DEBUG
  std::cout << m_delta.x << " " << m_delta.y << std::endl;
#endif

  // change cursor here

  if ( d_side == 1 ) {       // top
    wnd_pos.y += m_delta.y;
    wnd_szy -= m_delta.y;
  } else if( d_side == 2 ) { // right
    wnd_szx += m_delta.x;
    if( m_delta.x < 0 )
      ruser_start = m_pos;
  } else if( d_side == 3 ) { // bottom
    wnd_szy += m_delta.y;
    if( m_delta.y < 0 )
      ruser_start = m_pos;
  } else if( d_side == 4 ) { // left
    wnd_pos.x += m_delta.x;
    wnd_szx -= m_delta.x;
  }
  // * DO THIS ON PC CUZ LAPPYTOPPY ONLY GOT 1 SCREEN DUR *
  // get current monitor from screen mouse pos
  // check wnd_pos and wnd_sz so it isnt off the current monitor

  SetWindowPos( hwnd, 0,
    wnd_pos.x, wnd_pos.y,
    wnd_szx, wnd_szy,
    SWP_NOZORDER
  );
}

// add function for if titlebar double clicked, do max/restore
// call in WM_LBUTTONDBLCLK
// take code from uhhhhhhhhhhhhh wnd_title.cpp iirc