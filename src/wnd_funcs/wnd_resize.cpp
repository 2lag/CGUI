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
// simplify this please ..
void wnd_resize_get_side( s32 &d_side, POINT m_pos, RECT wnd_sz ) {
  salt wnd_szx = wnd_sz.right - wnd_sz.left,
      wnd_szy = wnd_sz.bottom - wnd_sz.top;

  if( m_pos.x <= 5 && m_pos.y <= 5 ) {
    d_side = EDGE_TOP_LEFT;
    SetCursor( LoadCursorW( 0, IDC_SIZENWSE ) );
  } else if( m_pos.y <= 5 &&
             m_pos.x <= wnd_szx - 5 && m_pos.x >= 5 ) {
    d_side = EDGE_TOP;
    SetCursor( LoadCursorW( 0, IDC_SIZENS ) );
  } else if( m_pos.x >= wnd_szx - 5 && m_pos.y < 5 ) {
    d_side = EDGE_TOP_RIGHT;
    SetCursor( LoadCursorW( 0, IDC_SIZENESW ) );
  } else if( m_pos.x >= wnd_sz.right - 5 &&
             m_pos.y >= 5 && m_pos.y <= wnd_szy - 5 ) {
    d_side = EDGE_RIGHT;
    SetCursor( LoadCursorW( 0, IDC_SIZEWE ) );
  } else if( m_pos.x >= wnd_szx - 5 && m_pos.y >= wnd_szy - 5 ) {
    d_side = EDGE_BOTTOM_RIGHT;
    SetCursor( LoadCursorW( 0, IDC_SIZENWSE ) );
  } else if( m_pos.y >= wnd_sz.bottom - 5 &&
             m_pos.x >= 5 && m_pos.x <= wnd_szx - 5 ) {
    SetCursor( LoadCursorW( 0, IDC_SIZENS ) );
    d_side = EDGE_BOTTOM;
  } else if( m_pos.x <= 5 && m_pos.y >= wnd_szy - 5 ) {
    d_side = EDGE_BOTTOM_LEFT;
    SetCursor( LoadCursorW( 0, IDC_SIZENESW ) );
  } else if( m_pos.x <= 5  &&
             m_pos.y <= wnd_szy - 5 && m_pos.y >= 5 ) {
    d_side = EDGE_LEFT;
    SetCursor( LoadCursorW( 0, IDC_SIZEWE ) );
  } else {
    SetCursor( LoadCursorW( 0, IDC_ARROW ) );
    d_side = 0;
  }
}
// make edge stick to cursor better
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

  // simplify this
  // also make it stay dragging current side until mouse released
  switch( d_side ) {
  case EDGE_TOP_LEFT: {
    wnd_pos = {
      wnd_pos.x += m_delta.x,
      wnd_pos.y += m_delta.y
    };
    wnd_szx -= m_delta.x;
    wnd_szy -= m_delta.y;
  } break;
  case EDGE_TOP: {
    wnd_pos.y += m_delta.y;
    wnd_szy -= m_delta.y;
  } break;
  case EDGE_TOP_RIGHT: {
    wnd_pos.y += m_delta.y;
    wnd_szy -= m_delta.y;
    wnd_szx += m_delta.x;
    ruser_start.x = m_pos.x;
  } break;
  case EDGE_RIGHT: {
    wnd_szx += m_delta.x;
    ruser_start.x = m_pos.x;
  } break;
  case EDGE_BOTTOM_RIGHT: {
    wnd_szx += m_delta.x;
    wnd_szy += m_delta.y;
    ruser_start = m_pos;
  } break;
  case EDGE_BOTTOM: {
    wnd_szy += m_delta.y;
    ruser_start.y = m_pos.y;
  } break;
  case EDGE_BOTTOM_LEFT: {
    wnd_pos.x += m_delta.x;
    wnd_szx -= m_delta.x;
    wnd_szy += m_delta.y;
    ruser_start.y = m_pos.y;
  } break;
  case EDGE_LEFT: {
    wnd_pos.x += m_delta.x;
    wnd_szx -= m_delta.x;
  } break;
  }

  // * DO THIS ON PC CUZ LAPPYTOPPY ONLY GOT 1 SCREEN DUR *
  // get current monitor from screen mouse pos
  // check wnd_pos and wnd_sz so it cant be dragged off the current monitor

  // also cap min size bleh

  SetWindowPos( hwnd, 0,
    wnd_pos.x, wnd_pos.y,
    wnd_szx, wnd_szy,
    SWP_NOZORDER
  );
}

// add function for if titlebar double clicked, do max/restore
// call in WM_LBUTTONDBLCLK
// take code from uhhhhhhhhhhhhh wnd_title.cpp iirc