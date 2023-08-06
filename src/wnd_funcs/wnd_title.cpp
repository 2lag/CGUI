#include "wnd_title.h"

void wnd_title_draw( HDC hdc, POINT m_pos, RECT wnd_sz ) {
  wnd_obj obj[] = {
    wnd_obj_create( {                 0, 0, wnd_sz.right -  0, 25 }, COL_L_GRY, false ),
    wnd_obj_create( {                 0, 0, wnd_sz.right -  0, 24 }, COL_D_GRY, false ),
    wnd_obj_create( { wnd_sz.right - 25, 0, wnd_sz.right -  0, 25 }, COL_L_GRY, true  ),
    wnd_obj_create( { wnd_sz.right - 24, 0, wnd_sz.right -  0, 24 }, COL_D_GRY, false ),
    wnd_obj_create( { wnd_sz.right - 50, 0, wnd_sz.right - 25, 25 }, COL_L_GRY, true  ),
    wnd_obj_create( { wnd_sz.right - 49, 0, wnd_sz.right - 25, 24 }, COL_D_GRY, false ),
    wnd_obj_create( { wnd_sz.right - 75, 0, wnd_sz.right - 50, 25 }, COL_L_GRY, true  ),
    wnd_obj_create( { wnd_sz.right - 74, 0, wnd_sz.right - 50, 24 }, COL_D_GRY, false )
  };

  (void)SetBkMode( hdc, TRANSPARENT );
  (void)SetTextColor( hdc, COL_L_GRY );

  HBRUSH b_hover = CreateSolidBrush( COL_D_RED );

  for( s32 idx = 0; idx < 8; ++idx ) {
    (void)FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    if( obj[ idx ].hover ) {
      ++idx;
      if( PtInRect( &obj[ idx ].r, m_pos ) )
        (void)FillRect( hdc, &obj[ idx ].r, b_hover );
      else
        (void)FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    }

    if( idx == 3 )
      (void)TextOutW( hdc, obj[ idx ].r.right - 16, obj[ idx ].r.bottom - 20, L"X", 1 );
    else if( idx == 5 )
      (void)TextOutW( hdc, obj[ idx ].r.right - 16, obj[ idx ].r.bottom - 20, L"O", 1 );
    else if( idx == 7 )
      (void)TextOutW( hdc, obj[ idx ].r.right - 16, obj[ idx ].r.bottom - 24, L"_", 1 );
  }
  (void)TextOutW( hdc, 6, 4, L"Type", 4 );

  for( auto& it : obj )
    DeleteObject( it.col );

  (void)DeleteObject( b_hover );
}

void wnd_title_clicked_cls( HWND hwnd, POINT m_pos, RECT space  ) {
  if( PtInRect( &space, m_pos ) )
    ExitProcess( 0 );
}