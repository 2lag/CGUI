#include "win_func.h"
#include "../../resource.h"

/* backup
HMENU hmenu;


  case WM_PAINT: { // fix me
    MENUINFO m_info{
      sizeof( m_info ),
      MIM_BACKGROUND | MIM_APPLYTOSUBMENUS | MIM_STYLE,
      MNS_NOCHECK, 0,
      CreateSolidBrush( RGB( 120, 110, 120 ) ),
      0, 0
    };

    SetMenuInfo( hmenu, &m_info );
  } break;

    old wm create

      hmenu = CreateMenu();
    HMENU hfile = CreateMenu(),
      htool = CreateMenu();

    InsertMenuW( hfile, 0, MF_INSERT, 0, L"&New"  );
    InsertMenuW( hfile, 1, MF_INSERT, 1, L"&Open" );
    InsertMenuW( hfile, 2, MF_INSERT, 2, L"&Save" );
    InsertMenuW( hfile, 3, MF_SEPARATOR, 3, nullptr );
    InsertMenuW( hfile, 4, MF_INSERT, 4, L"&Exit" );

    InsertMenuW( htool, 0, MF_INSERT, 0, L"&Macro" );

    AppendMenuW( hmenu, MF_POPUP, (u32)hfile, L"&File" );
    AppendMenuW( hmenu, MF_POPUP, (u32)htool, L"&Tool" );

    // with MF_OWNERDRAW TO FULLY CUSTOMIZE IT

    SetMenu( hwnd, hmenu );
*/

// https://coding-examples.com/c/mfc/mfc-owner-drawn-menu-using-wm_drawitem-wm_measureitem/
// https://learn.microsoft.com/en-us/windows/win32/menurc/using-menus?redirectedfrom=MSDN#_win32_Owner_Drawn_Menus_and_the_WM_DRAWITEM_Message
LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
  static bool mload = false;
  switch( msg ) {
  case WM_CREATE: {
    HMENU hmenu = LoadMenuW(
      GetModuleHandleW( nullptr ),
      MAKEINTRESOURCEW( IDR_MENU )
    );

    MENUINFO minfo{};
    minfo.cbSize = sizeof( minfo );
    minfo.fMask = MIM_STYLE | MIM_APPLYTOSUBMENUS;
    minfo.dwStyle = MF_OWNERDRAW;

    SetMenu( hwnd, hmenu );
    SetMenuInfo( hmenu, &minfo );

  } break;
  case WM_DESTROY: {
    PostQuitMessage( 0 );
    return 0;
  } break;
  case WM_DRAWITEM: {
    LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lp;
    if( lpdis->CtlType == ODT_MENU ) {
      HFONT hfont = (HFONT)GetStockObject( DEFAULT_GUI_FONT );
      SelectObject( lpdis->hDC, hfont );

      HBRUSH brush_bg = CreateSolidBrush( RGB( 32, 32, 32 ) );
      FillRect( lpdis->hDC, &lpdis->rcItem, brush_bg );
      DeleteObject( brush_bg );

      switch( lpdis->itemID ) {
      case ID_FILE_NEW: {
        DrawTextW( lpdis->hDC, L"&New", 3, &lpdis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
      } break;
      case ID_FILE_OPEN: {

      } break;
      case ID_FILE_SAVE: {

      } break;
      case ID_FILE_EXIT: {

      } break;
      case ID_TOOLS_MACRO: {

      } break;
      }
    }
  } break;
  }
  return DefWindowProcW( hwnd, msg, wp, lp );
}