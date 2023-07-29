#pragma once
#include <d3d9.h>
#pragma comment( lib, "d3d9" )
#include <d3dx9.h>
#pragma comment( lib, "d3dx9" )


extern LPDIRECT3D9       d3d;
extern LPDIRECT3DDEVICE9 d3d_dev;
extern LPD3DXFONT        d3d_font;

void d3d_init  ( HWND hwnd );
void d3d_render( HWND hwnd );
void d3d_clean ( void );