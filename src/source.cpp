#include "win/win_func.h"
#include "dx/d3d_func.h"
#include "typedef.h"

// figure out how to disable scaling on resize
// scale text as well with the dimensions/drawing

// clean up all files to reduce process "weight"
// add tps to bottom right
// make text box blank by default
// editable if clicked, disable anywhere else

// work on menu bar, file, tool, etc
// file ->
//   new|open|save
// tool ->
//   macro

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
    CreateSolidBrush( RGB( 24, 24, 24 ) ),
    L"type", L"type_class"
  };

  (void)RegisterClassExW( &wnd_cls );

  HWND hwnd = CreateWindowExW( 0,
    L"type_class", L"type",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    420, 420,
    0, 0, inst, 0
  );

  s32 show_dark = 1;
  (void)DwmSetWindowAttribute(
    hwnd, 20,
    &show_dark,
    sizeof( s32 )
  );

  (void)ShowWindow( hwnd, cmdshow );

  d3d_init( hwnd );

  MSG msg;
  while( true ) {
    while( PeekMessageW( &msg, 0, 0, 0, PM_REMOVE ) ) {
      (void)TranslateMessage( &msg );
      (void)DispatchMessageW( &msg );
    }

    if( msg.message == WM_QUIT )
      break;

    d3d_render( hwnd );
  }

  d3d_clean();

  return msg.wParam;
}