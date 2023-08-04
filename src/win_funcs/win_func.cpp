#include "win_func.h"
#include "../../resource.h"

// study these to make menu darkmode
// https://github.com/notepad-plus-plus/notepad-plus-plus/blob/master/PowerEditor/src/DarkMode/
// https://github.com/notepad-plus-plus/notepad-plus-plus/blob/dea388bf594df4934d41d86127ff949d96fd3dc9/PowerEditor/src/NppDarkMode.cpp#L771

// menu ids ( search for these to figure out how to draw )
// https://github.com/notepad-plus-plus/notepad-plus-plus/blob/master/PowerEditor/src/menuCmdID.h

// found this
// https://github.com/notepad-plus-plus/notepad-plus-plus/blob/dea388bf594df4934d41d86127ff949d96fd3dc9/PowerEditor/src/WinControls/ToolBar/ToolBar.cpp#L501

LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
  static bool mload = false;
  switch( msg ) {
  case WM_CREATE: {
    HMENU hmenu = CreateMenu(),
          hfile = CreatePopupMenu(),
          hedit = CreatePopupMenu(),
         hmacro = CreatePopupMenu();

    AppendMenuW( hfile, MF_STRING, IDM_FILE_NEW , L"New" ); // swap _STRING to _OWNERDRAW for all?
    AppendMenuW( hfile, MF_STRING, IDM_FILE_OPEN, L"Open" );
    AppendMenuW( hfile, MF_STRING, IDM_FILE_SAVE, L"Save" );
    AppendMenuW( hfile, MF_STRING, IDM_FILE_EXIT, L"Exit" );


    AppendMenuW( hedit, MF_STRING, IDM_EDIT_CUT       , L"Cut"        );
    AppendMenuW( hedit, MF_STRING, IDM_EDIT_COPY      , L"Copy"       );
    AppendMenuW( hedit, MF_STRING, IDM_EDIT_PASTE     , L"Paste"      );
    AppendMenuW( hedit, MF_STRING, IDM_EDIT_SELECT_ALL, L"Select All" );
    AppendMenuW( hedit, MF_STRING, IDM_EDIT_FIND      , L"Find"       );

    AppendMenuW( hmacro, MF_STRING, IDM_MACRO_RECORD  , L"Record"   );
    AppendMenuW( hmacro, MF_STRING, IDM_MACRO_PLAYBACK, L"Playback" );

    AppendMenuW( hmenu, MF_POPUP , (u32)hfile   , L"File"  );
    AppendMenuW( hmenu, MF_STRING, IDM_EDIT_UNDO, L"<-"    );
    AppendMenuW( hmenu, MF_STRING, IDM_EDIT_REDO, L"->"    );
    AppendMenuW( hmenu, MF_POPUP , (u32)hedit   , L"Edit"  );
    AppendMenuW( hmenu, MF_POPUP , (u32)hmacro  , L"Macro" );

    SetMenu( hwnd, hmenu );
  } break;
  case WM_DESTROY: {
    PostQuitMessage( 0 );
    return 0;
  } break;
  }
  return DefWindowProcW( hwnd, msg, wp, lp );
}

