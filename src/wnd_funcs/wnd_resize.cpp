#include "wnd_resize.h"

bool user_resizing = false;
POINT ruser_start{};

void wnd_resize( HWND hwnd, POINT m_pos, RECT wnd_sz ) {
  // if user_resizing, do resize
  // if not, get m_pos
  // follow the same style as in wnd_resize.cpp

  // if mouse pos x < 3                , drag left   edge ( move position and compensate size )
  // if mouse pos y < 3                , drag top    edge ( move position and compensate size )
  // if mouse pos x > wnd_sz.right  - 3, drag right  edge ( only change size based on m_pos.x delta )
  // if mouse pos y > wnd_sz.bottom - 3, drag bottom edge ( only change size based on m_pos.y delta )

  // use SetWindowPos to change size
}

// create wnd_resize_on in WM_MOUSEMOVE
// toggles user_resizing based off of m_pos listed above

// add function for if titlebar clicked twice, maximize/minimize depending on IsZoomed