#include "win_func.h"
#include "menu_ids.h"

// study these to make menu darkmode
// https://github.com/notepad-plus-plus/notepad-plus-plus/blob/master/PowerEditor/src/DarkMode/
// https://github.com/notepad-plus-plus/notepad-plus-plus/blob/dea388bf594df4934d41d86127ff949d96fd3dc9/PowerEditor/src/NppDarkMode.cpp#L771

// menu ids ( search for these to figure out how to draw )
// https://github.com/notepad-plus-plus/notepad-plus-plus/blob/master/PowerEditor/src/menuCmdID.h

LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
  static bool mload = false;
  switch( msg ) {
  case WM_CREATE: {

  } break;
  case WM_DESTROY: {
    PostQuitMessage( 0 );
    return 0;
  } break;
  }
  return DefWindowProcW( hwnd, msg, wp, lp );
}