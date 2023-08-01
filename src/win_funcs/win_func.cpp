#include "win_func.h"

LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
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
  case WM_PAINT: { // fix me
    MENUINFO m_info{
      sizeof( m_info ),
      MIM_BACKGROUND | MIM_APPLYTOSUBMENUS,
      0, 0, /* <--this zero may not be right */
      CreateSolidBrush( RGB( 32, 32, 32 ) ),
      0, 0
    };

    SetMenuInfo( GetMenu( hwnd ), &m_info );
  } break;
  }
  return DefWindowProcW( hwnd, msg, wp, lp );
}