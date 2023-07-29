#include "../typedef.h"
#include "d3d_func.h"


LPDIRECT3D9       d3d;
LPDIRECT3DDEVICE9 d3d_dev;
LPD3DXFONT        d3d_font;

void d3d_init( HWND hwnd ) {
  d3d = Direct3DCreate9( D3D_SDK_VERSION );

  D3DPRESENT_PARAMETERS d3d_pp { 0, 0,
    D3DFMT_A8R8G8B8, 0,
    D3DMULTISAMPLE_NONE, 0,
    D3DSWAPEFFECT_DISCARD, hwnd, 1, 0,
    D3DFMT_UNKNOWN, 1, 0, 0
  };

  (void)d3d->CreateDevice( 0L,
    D3DDEVTYPE_HAL, hwnd, 32L,
    &d3d_pp, &d3d_dev
  );

  (void)D3DXCreateFontW(
    d3d_dev, 16, 0, 400, 1,
    0, 1, 0, 0, 0 | (0<<4),
    L"Courier New",
    &d3d_font
  );
}

void d3d_render( HWND hwnd ) {
  (void)d3d_dev->Clear( 0, 0, 1L,
    D3DCOLOR_XRGB ( 24, 24, 24 ),
    1.f, 0
  );
  (void)d3d_dev->BeginScene();

  //do rendering here
  RECT win_sz;
  (void)GetClientRect( hwnd, &win_sz );

  f32 win_w = (f32)win_sz.right - (f32)win_sz.left;
  f32 win_h = (f32)win_sz.bottom - (f32)win_sz.top;
  
  struct Vertex {
    f32 x, y, z, rhw;
    ptr color;
  };

  static Vertex txt_box_vert[ 4 ] {
    { 36       ,         40, 0.f, 1.f, D3DCOLOR_XRGB( 32, 32, 32 ) },
    { win_w - 2,         40, 0.f, 1.f, D3DCOLOR_XRGB( 32, 32, 32 ) },
    { win_w - 2, win_h - 20, 0.f, 1.f, D3DCOLOR_XRGB( 32, 32, 32 ) },
    { 36       , win_h - 20, 0.f, 1.f, D3DCOLOR_XRGB( 32, 32, 32 ) }
  };

  (void)d3d_dev->SetFVF( 0x004 | 0x040 );
  (void)d3d_dev->DrawPrimitiveUP(
    D3DPT_TRIANGLEFAN,
    2, txt_box_vert,
    sizeof( Vertex )
  );

  RECT txt_rect {
    36, 40,
    (salt)win_w - 2,
    (salt)win_h - 20
  };
  (void)d3d_font->DrawTextW(
    nullptr,  L"test", -1,
    &txt_rect, DT_LEFT | DT_TOP,
    D3DCOLOR_ARGB( 255, 255, 255, 255 )
  );

  (void)d3d_dev->EndScene();
  (void)d3d_dev->Present( 0, 0, 0, 0 );
}

void d3d_clean( void ) {
  (void)d3d_font->Release();
  (void)d3d_dev->Release();
  (void)d3d->Release();
}