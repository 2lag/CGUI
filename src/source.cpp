#include "wnd_funcs/wnd_func.h"

/*
  complete wnd_func.cpp bugs
  add resize functionality
  build out menu bar
  add tps counter
  build out text box using vectors ?
  implement rest of menu bar funcs
  implement line nums and scrolling/scroll tracking
  handle keyboard shortcuts
  add font sizes & stuff like bold/italic/strikethru/etc
  add syntax highlighting for cpp ( language server ? )
*/

s32 WINAPI WinMain( _In_     HINSTANCE inst   ,
                    _In_opt_ HINSTANCE p_inst ,
                    _In_     LPSTR     cmdline,
                    _In_     s32       cmdshow) {
  WNDCLASSEX wnd_cls {
    sizeof( WNDCLASSEX ),
    CS_HREDRAW | CS_VREDRAW,
    wnd_proc,
    0, 0,
    inst,
    0,
    LoadCursor( 0, IDC_ARROW ),
    CreateSolidBrush( RGB( 32, 32, 32 ) ),
    L"type", L"type_class"
  };

  (void)RegisterClassExW( &wnd_cls );

  HWND hwnd = CreateWindowExW( 0,
    L"type_class", L"type",
    WS_POPUPWINDOW,
    100, 100,
    420, 420,
    0, 0, inst, 0
  );

  (void)ShowWindow( hwnd, cmdshow );
  (void)UpdateWindow( hwnd );

  MSG msg;
  while( true ) {
    while( PeekMessageW( &msg, 0, 0, 0, PM_REMOVE ) ) {
      (void)TranslateMessage( &msg );
      (void)DispatchMessageW( &msg );
    }

    if( msg.message == WM_QUIT )
      break;
  }

  return (s32)msg.wParam;
}