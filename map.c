#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <curses.h>

static WINDOW *mainwin;

#define MAP_NLINES      23
#define MAP_NCOLS       69

#define NLINES      (  7 + MAP_NLINES )
#define NCOLS       ( 20 + MAP_NCOLS )

#define getSRand()      ( ( float ) rand( ) / ( float ) RAND_MAX )
#define getRand( x )    ( int ) ( ( x ) * getSRand( ) )

#define X_MAP_MAX    1000
#define Y_MAP_MAX    500

char map[ Y_MAP_MAX ][ X_MAP_MAX ];

int offsetx, offsety;

int ObstacleCreate( void )
{
   int LocX, LocY, X, Y;
   int SizeX, SizeY;
   const int LimitX = 6, LimitY = 3;

   LocX = getRand( ( X_MAP_MAX - 2 - LimitX ) ) + 1;
   LocY = getRand( ( Y_MAP_MAX - 2 - LimitY ) ) + 1;

   SizeX = getRand( LimitX ) + 2;
   SizeY = getRand( LimitY ) + 2;

   for ( Y = 0 ; Y < SizeY ; Y++ )
   {
      for ( X = 0 ; X < SizeX ; X++ )
      {
         map[ Y + LocY ][ X + LocX ] = '^';
      }
   }
   
   return ( SizeY * SizeX );
}

void MapInit( int PY, int PX )
{
   int X, Y;
   int Density = 0;

   for ( Y = 0 ; Y < Y_MAP_MAX ; Y++ )
   {
      for ( X = 0 ; X < X_MAP_MAX ; X++ )
      {
         if ( Y == 0 || X == 0 )
         {
            map[ Y ][ X ] = '+';
         }
         else if ( ( Y == ( Y_MAP_MAX - 1 ) ) || ( X == ( X_MAP_MAX - 1 ) ) )
         {
            map[ Y ][ X ] = '+';
         }
         else
         {
            map[ Y ][ X ] = ' ';
         }
      }
   }

   // Add noise to the map.
   while ( Density < ( ( Y_MAP_MAX * X_MAP_MAX ) / 3 ) )
   {
      Density += ObstacleCreate( );
   }

   map[ PY ][ PX ] = '@';

   return;
}


void win_startup( void )
{
   initscr( );
   cbreak( );
   noecho( );
   curs_set( 0 );
   nonl( );

   offsety = ( LINES - NLINES ) / 2;
   offsetx = ( COLS - NCOLS ) / 2;

   mainwin = newwin( NLINES, NCOLS, offsety, offsetx );
   nodelay( mainwin, TRUE );
   keypad( mainwin, TRUE );

   return;
}

void win_update( int Y, int X )
{
   wborder( mainwin, 0, 0, 0, 0, 0, 0, 0, 0 );

   mvwhline( mainwin, ( MAP_NLINES - 1 ), 1, '-', ( MAP_NCOLS - 1 ) );
   mvwvline( mainwin, 1, ( MAP_NCOLS - 1 ), '|', ( MAP_NLINES - 1 ) );

   mvwprintw( mainwin, 1, (MAP_NCOLS), "%3d,%3d", Y, X );

   wrefresh( mainwin );

   return;
}

void win_shutdown( void )
{
   delwin( mainwin );

   endwin( );

   return;
}

char vp_map( int y, int x )
{
   if ( ( y < 0 ) || ( x < 0 ) )
   {
      return '~';
   }
   else if ( ( y >= Y_MAP_MAX ) || ( x >= X_MAP_MAX ) )
   {
      return '~';
   }
   else
   {
      return map[ y ][ x ];
   }
}

void win_map_viewport( int Y, int X )
{
   int x, y;
   int vp_y;
   int vp_x;

   vp_y = Y - ( ( MAP_NLINES - 2 ) / 2 );

   for ( y = 1 ; y < ( MAP_NLINES - 1 ) ; y++ )
   {
      vp_x = X - ( ( MAP_NCOLS - 2 ) / 2 );
      for ( x = 1 ; x < ( MAP_NCOLS - 1 ) ; x++ )
      {
         mvwprintw( mainwin, y, x, "%c", vp_map( vp_y, vp_x ) );
         vp_x++;
      }
      vp_y++;
   }

   return;
}

void get_input( int *Y, int *X )
{
   int c;
   int dy=0, dx=0;

   c = wgetch( mainwin );

   if ( c != ERR )
   {
      if ( (char)c == 'k' )
      {
         dy=-1;
      } 
      else if ( (char)c == 'j' )
      {
         dy=1;
      }
      else if ( (char)c == 'h' )
      {
         dx=-1;
      }
      else if ( (char)c == 'l' )
      {
         dx=1;
      }

      if ( map[ *Y+dy ][ *X+dx ] == ' ' )
      {
         map[ *Y ][ *X ] = ' ';
         *Y+=dy; *X+=dx;
         map[ *Y ][ *X ] = '@';
      }
   }

   return;
}


int main( int argc, char *argv[] )
{
   int Y, X;

   srand( time( NULL ) );

   Y = getRand( Y_MAP_MAX );
   X = getRand( X_MAP_MAX );

   MapInit( Y, X );

   win_startup( );

   win_update( Y, X );

   while ( 1 )
   {
      get_input( &Y, &X );

      win_map_viewport( Y, X );

      win_update( Y, X );

      wrefresh( mainwin );
   }

   win_shutdown( );

   return 0;
}

