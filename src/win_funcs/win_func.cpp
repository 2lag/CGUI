#include "win_func.h"

LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
  static bool mload = false;
  switch( msg ) {
  case WM_CREATE: {
    HMENU hfile  = CreateMenu(),
          hmacro = CreateMenu(),
          hmenu  = CreateMenu();

    AppendMenuW( hfile, MF_OWNERDRAW, 0000, L"&New"  );
    AppendMenuW( hfile, MF_OWNERDRAW, 0001, L"&Open" );
    AppendMenuW( hfile, MF_OWNERDRAW, 0002, L"&Save" );
    AppendMenuW( hfile, MF_OWNERDRAW | MF_SEPARATOR, 0003, nullptr );
    AppendMenuW( hfile, MF_OWNERDRAW, 0004, L"&Exit" );

    AppendMenuW( hmacro, MF_OWNERDRAW, 0010, L"&Record"   );
    AppendMenuW( hmacro, MF_OWNERDRAW, 0011, L"&Playback" );

    AppendMenuW( hmenu, MF_OWNERDRAW | MF_POPUP, (u32)hfile , L"File"  );
    AppendMenuW( hmenu, MF_OWNERDRAW | MF_POPUP, (u32)hmacro, L"Macro" );
    
    SetMenu( hwnd, hmenu );

  } break;
  case WM_DESTROY: {
    PostQuitMessage( 0 );
    return 0;
  } break;
  case WM_DRAWITEM: {
    LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lp;

    SelectObject( lpdis->hDC,
      (HFONT)GetStockObject(
        DEFAULT_GUI_FONT
      )
    );

    FillRect( lpdis->hDC, &lpdis->rcItem,
      CreateSolidBrush( RGB( 32, 32, 32 ) )
    );

    SetTextColor( lpdis->hDC, RGB( 222, 222, 222 ) );
    SetBkMode( lpdis->hDC, TRANSPARENT );

    // figure out how to write to submenu option in bar
    // and draw over the rest of the menu bar

    // swap to this for darkmode theme after you figure it out
    // https://github.com/notepad-plus-plus/notepad-plus-plus/blob/master/PowerEditor/src/DarkMode/DarkMode.cpp

#define w_menu_txt( id, x, y, txt, len ) \
if( lpdis->itemID == id ) { \
  TextOutW( lpdis->hDC, \
    lpdis->rcItem.left + x, \
    lpdis->rcItem.top  + y, \
    txt, len \
  ); \
}
    w_menu_txt( 0000, 20, 5, L"New" , 3 );
    w_menu_txt( 0001, 20, 5, L"Open", 4 );
    w_menu_txt( 0002, 20, 5, L"Save", 4 );
    w_menu_txt( 0003,  0, 0, L"____________", 12 );
    w_menu_txt( 0004, 20, 5, L"Exit", 4 );

    w_menu_txt( 0010, 10, 5, L"Record"  , 6 );
    w_menu_txt( 0011, 10, 5, L"Playback", 8 );

  } break;
  case WM_MEASUREITEM: {
    LPMEASUREITEMSTRUCT lpmis = (LPMEASUREITEMSTRUCT)lp;
    if( lpmis->CtlType == ODT_MENU ) { // make separator size smaller than the rest
      lpmis->itemWidth = 50;
      lpmis->itemHeight = 25;
    }
  } break;
  }
  return DefWindowProcW( hwnd, msg, wp, lp );
}