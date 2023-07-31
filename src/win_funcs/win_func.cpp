#include "win_func.h"

salt __stdcall wnd_proc( HWND hwnd, u32 msg, u32 wp, salt lp ) {
  switch( msg ) {
  case WM_DESTROY: {
    PostQuitMessage( 0 );
    return 0;
  } break;
  case WM_CREATE: {
    HMENU hmenu = CreateMenu(),
          hfile = CreateMenu(),
          htool = CreateMenu();

    AppendMenuW( hfile, MF_STRING, 1u, L"&New"  );
    AppendMenuW( hfile, MF_STRING, 1u, L"&Open" );
    AppendMenuW( hfile, MF_STRING, 1u, L"&Save" );
    AppendMenuW( hfile, MF_STRING, 1u, L"&Exit" );

    AppendMenuW( hmenu, MF_POPUP , (u32)hfile, L"&File" );

    SetMenu( hwnd, hmenu );

  } break;
  }
  return DefWindowProcW( hwnd, msg, wp, lp );
}