#include "win_func.h"

LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
  static bool mload = false;
  switch( msg ) {
  case WM_CREATE: {
    HMENU hfile  = CreateMenu(),
          hmacro = CreateMenu(),
          hmenu  = CreateMenu();

    AppendMenuW( hfile, MF_OWNERDRAW, 0, L"&New"  );
    AppendMenuW( hfile, MF_OWNERDRAW, 1, L"&Open" );
    AppendMenuW( hfile, MF_OWNERDRAW, 2, L"&Save" );
    AppendMenuW( hfile, MF_OWNERDRAW | MF_SEPARATOR, 3, nullptr );
    AppendMenuW( hfile, MF_OWNERDRAW, 4, L"&Exit" );

    AppendMenuW( hmacro, MF_OWNERDRAW, 0, L"&Record"   );
    AppendMenuW( hmacro, MF_OWNERDRAW, 1, L"&Playback" );

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

    TextOutW( lpdis->hDC, lpdis->rcItem.left + 20, lpdis->rcItem.top + 4, L"fuck", 4 );
    // figure out how to differentiate various sub menus
    // only thing so far is a switch with lpdis->itemID and that doesn't differentiate
  } break;
  case WM_MEASUREITEM: {
    LPMEASUREITEMSTRUCT lpmis = (LPMEASUREITEMSTRUCT)lp;
    if( lpmis->CtlType == ODT_MENU ) {
      lpmis->itemWidth = 50;
      lpmis->itemHeight = 25;
    }
  } break;
  }
  return DefWindowProcW( hwnd, msg, wp, lp );
}