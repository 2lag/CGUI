#include "win_func.h"
#include "../../resource.h"

// add hover detection
// fix "double height menu bar"
// figure out how to color rest of menu bar

LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
  static bool mload = false;
  switch( msg ) {
  case WM_CREATE: {
    HMENU hmenu = LoadMenuW(
      GetModuleHandleW( nullptr ),
      MAKEINTRESOURCEW( IDR_MENU )
    );

    MENUITEMINFO minfo{};
    minfo.cbSize = sizeof( minfo );
    minfo.fMask = MIIM_TYPE;
    minfo.fType = MFT_OWNERDRAW;

    SetMenu( hwnd, hmenu );

    SetMenuItemInfoW( hmenu, ID_FILE_NEW   , 0, &minfo );
    SetMenuItemInfoW( hmenu, ID_FILE_OPEN  , 0, &minfo );
    SetMenuItemInfoW( hmenu, ID_FILE_SAVE  , 0, &minfo );
    SetMenuItemInfoW( hmenu, ID_FILE_EXIT  , 0, &minfo );

    SetMenuItemInfoW( hmenu, ID_MACRO_RECORD  , 0, &minfo );
    SetMenuItemInfoW( hmenu, ID_MACRO_PLAYBACK, 0, &minfo );
  } break;
  case WM_DESTROY: {
    PostQuitMessage( 0 );
    return 0;
  } break;
  case WM_DRAWITEM: {
    LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lp;
    if( lpdis->CtlType == ODT_MENU ) {
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

      switch( lpdis->itemID ) {
      case ID_FILE_NEW: {
        TextOutW( lpdis->hDC, 20,  4, L"New"  , 3 );
      } break;
      case ID_FILE_OPEN: {
        TextOutW( lpdis->hDC, 20, 23, L"Open" , 4 );
      } break;
      case ID_FILE_SAVE: {
        TextOutW( lpdis->hDC, 20, 42, L"Save" , 4 );
      } break;
      case ID_FILE_EXIT: {
        TextOutW( lpdis->hDC, 20, 61, L"Exit" , 4 );
      } break;
      case ID_MACRO_RECORD: {
        TextOutW( lpdis->hDC, 10,  4, L"Record"  , 7 );
      } break;
      case ID_MACRO_PLAYBACK: {
        TextOutW( lpdis->hDC, 10, 24, L"Playback", 8 );
      } break;
      }
    }
  } break;
  case WM_MEASUREITEM: {
    LPMEASUREITEMSTRUCT lpmis = (LPMEASUREITEMSTRUCT)lp;
    if( lpmis->CtlType == ODT_MENU ) {
      lpmis->itemWidth = 50;
      lpmis->itemHeight = 20;
    }
  } break;
  }
  return DefWindowProcW( hwnd, msg, wp, lp );
}