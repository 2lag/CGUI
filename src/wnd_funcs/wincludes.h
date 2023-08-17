#pragma once
#define WIN32_LEAN_AND_MEAN
#include  <windows.h>
#include <windowsx.h>
#include   <dwmapi.h>
#pragma comment( lib, "dwmapi" )

#ifdef _DEBUG
#include <iostream>
#endif

extern bool  user_resizing;
extern POINT max_prev_pos;
extern RECT  max_prev_sz;
extern bool  is_maxd;

inline POINT operator+=( POINT &a, const POINT &b ) {
  a.x += b.x;
  a.y += b.y;
  return a;
}

inline POINT operator-( POINT &a ) {
  return { -a.x, -a.y };
}

inline bool operator!( POINT &a ) {
  return ( a.x == 0 || a.y == 0 );
}