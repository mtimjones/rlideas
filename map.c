#include <stdio.h>
#include <stdlib.h>

#define getSRand()      ( (float) rand() / (float) RAND_MAX )
#define getRand( x )    (int)((x) * getSRand() )

#define X_MAX    250
#define Y_MAX    50

char map[ Y_MAX ][ X_MAX ];

void MapInit( void )
{
   int X, Y;

   for ( Y = 0 ; Y < Y_MAX ; Y++ )
   {
      for ( X = 0 ; X < X_MAX ; X++ )
      {
         if ( Y == 0 || X == 0 )
         {
            map[ Y ][ X ] = '+';
         }
         else if ( ( Y == ( Y_MAX - 1 ) ) || ( X == ( X_MAX - 1 ) ) )
         {
            map[ Y ][ X ] = '+';
         }
         else
         {
            map[ Y ][ X ] = ' ';
         }
      }
   }

   return;
}

int ObstacleCreate( void )
{
   int LocX, LocY, X, Y;
   int SizeX, SizeY;
   const int LimitX = 6, LimitY = 3;

   LocX = getRand( ( X_MAX - 2 - LimitX ) ) + 1;
   LocY = getRand( ( Y_MAX - 2 - LimitY ) ) + 1;

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

void MapPrint( void )
{
   int X, Y;

   for ( Y = 0 ; Y < Y_MAX ; Y++ )
   {
      for ( X = 0 ; X < X_MAX ; X++ )
      {
         printf( "%c", map[ Y ][ X ] );
      }
      printf("\n");
   }

   return;
}


int main( int argc, char *argv[] )
{
   int Density = 0;

   srand( time( NULL ) );

   MapInit( );

   while ( Density < ( ( Y_MAX * X_MAX ) / 3 ) )
   {
      Density += ObstacleCreate( );
   }

   MapPrint( );

   return 0;
}
