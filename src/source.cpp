#include "wnd/wnd_func.h"
#include "wnd/funcs/wnd_tps.h"

s32 WINAPI WinMain( _In_     HINSTANCE inst    ,
                    _In_opt_ HINSTANCE         ,
                    _In_     LPSTR             ,
                    _In_     s32       cmdshow ) {
  WNDCLASSEX wnd_cls {
    sizeof( WNDCLASSEX ),
    CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
    wnd_proc, 0, 0, inst, 0,
    LoadCursorW( 0, IDC_ARROW ),
    CreateSolidBrush( RGB( 32, 32, 32 ) ),
    L"CGUI", L"cgui_class"
  };

  RegisterClassExW( &wnd_cls );

  HWND hwnd = CreateWindowExW( 0,
    L"cgui_class", L"CGUI",
    WS_POPUPWINDOW,
    100, 100,
    420, 420,
    0, 0, inst, 0
  );

  ShowWindow( hwnd, cmdshow );
  UpdateWindow( hwnd );

  MSG msg;
  while( true ) {
    while( PeekMessageW( &msg, nullptr, 0, 0, PM_REMOVE ) ) {
      TranslateMessage( &msg );
      DispatchMessageW( &msg );
    }

    if( msg.message == WM_QUIT )
      break;

    wnd_tps_calc();
  }
  return (s32)msg.wParam;
}