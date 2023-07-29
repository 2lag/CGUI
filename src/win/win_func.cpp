#include "win_func.h"

salt __stdcall wnd_proc( HWND hwnd, u32 msg, u32 wp, salt lp ) {
  switch( msg ) {
  case WM_DESTROY: {
    PostQuitMessage( 0 );
    return 0;
  } break;
    // next case here
  }
  return DefWindowProcW( hwnd, msg, wp, lp );
}