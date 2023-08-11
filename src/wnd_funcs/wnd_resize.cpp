#include "wnd_resize.h"

bool user_resizing = false;
POINT ruser_start{};

void wnd_resize( HWND hwnd, POINT m_pos, RECT wnd_sz ) {
  if( !( GetAsyncKeyState( VK_LBUTTON ) & 0x8000 ) ) // maybe rm 0x8000
    return;

  POINT sm_pos;
  (void)GetCursorPos( &sm_pos );

  POINT m_delta {
    sm_pos.x - ruser_start.x,
    sm_pos.y - ruser_start.y
  };

  POINT wnd_pos{
    wnd_sz.left,
    wnd_sz.top
  };
  ClientToScreen( hwnd, &wnd_pos );
  // make sure all these vars are right
  // add functionality to make sure that the user doesn't drag past the current monitors size
  salt nwnd_x = wnd_pos.x,
       nwnd_y = wnd_pos.y,
       nwnd_w = wnd_sz.right - wnd_sz.left,
       nwnd_h = wnd_sz.bottom - wnd_sz.top;

  s32 wnd_flags = SWP_NOZORDER;

  if( m_pos.x < 3 ) {
    
  } else if( m_pos.x > wnd_sz.right - 3 ) {
  
  } else if( m_pos.y < wnd_sz.top ) {
  
  } else if( m_pos.y > wnd_sz.bottom - 3 ) {

  }
  // if mouse pos x < 3                , drag left   edge ( move position and compensate size ) & wnd_flags
  // if mouse pos y < 3                , drag top    edge ( move position and compensate size ) & wnd_flags
  // if mouse pos x > wnd_sz.right  - 3, drag right  edge ( only change size based on m_pos.x delta )
  // if mouse pos y > wnd_sz.bottom - 3, drag bottom edge ( only change size based on m_pos.y delta )

  SetWindowPos( hwnd, 0,
    nwnd_x, nwnd_y,
    nwnd_w, nwnd_h,
    wnd_flags
  );
}

// add function for if titlebar clicked twice, maximize/minimize depending on size