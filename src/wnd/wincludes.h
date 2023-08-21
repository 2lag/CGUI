#pragma once
#define WIN32_LEAN_AND_MEAN
#include  <windows.h>
#include <windowsx.h>
#include   <dwmapi.h>
#pragma comment( lib, "dwmapi" )
#include <unordered_map>
#include <functional>

#ifdef _DEBUG
#include <iostream>
#endif

#include "../typedef.h"

extern bool  user_resizing;
extern POINT max_prev_pos;
extern RECT  max_prev_sz;
extern bool  is_maxd;

inline POINT operator+=( POINT &a, const POINT b ) {
  a.x += b.x;
  a.y += b.y;
  return a;
}

inline POINT operator+=( const POINT a, s32 b ) {
  POINT r;
  r.x = a.x + b;
  r.y = a.y + b;
  return r;
}

inline POINT operator+( const POINT &a, const POINT &b ) {
  POINT r;
  r.x = a.x + b.x;
  r.y = a.y + b.y;
  return r;
}

inline POINT operator-=( const POINT a, s32 b ) {
  POINT r;
  r.x = a.x - b;
  r.y = a.y - b;
  return r;
}

inline POINT operator-( POINT a ) {
  return { -a.x, -a.y };
}

inline POINT operator-( const POINT &a, const POINT &b ) {
  POINT r;
  r.x = a.x - b.x;
  r.y = a.y - b.y;
  return r;
}

inline bool operator==( POINT a, POINT b ) {
  return( a.x == b.x && a.y == b.y );
}

inline POINT operator/( const POINT &a, s32 b ) {
  POINT r;
  r.x = a.x / b;
  r.y = a.y / b;
  return r;
}

inline bool operator!( const POINT &a ) {
  return ( a.x == 0 || a.y == 0 );
}

inline POINT to_sz_point( RECT r ) {
  return {
    r.right - r.left,
    r.bottom - r.top
  };
}

inline POINT to_pos_point( RECT r ) {
  return {
    r.left,
    r.top
  };
}

#define get_monitor_info( c_mon ) \
MONITORINFO i_mon; \
i_mon.cbSize = sizeof( i_mon ); \
GetMonitorInfoW( c_mon, &i_mon )