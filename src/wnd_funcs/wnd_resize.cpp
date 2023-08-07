#include "wnd_resize.h"

void wnd_resize( HWND hwnd, POINT m_pos, RECT wnd_sz ) {
  // if mouse pos x < 3                , drag left   edge ( move position and compensate size )
  // if mouse pos x > wnd_sz.right  - 3, drag right  edge ( only change size based on m_pos.x delta )
  // if mouse pos y < 3                , drag top    edge ( move position and compensate size )
  // if mouse pos y > wnd_sz.bottom - 3, drag bottom edge ( only change size based on m_pos.y delta )

  // use SetWindowPos to change size
}