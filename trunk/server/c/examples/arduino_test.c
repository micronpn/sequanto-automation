#include <sequanto/automation.h>

#include "wiring.h"

static const int COLUMNS = 7;
static const int ROWS = 5;

static uint8_t display[] = { 0, 0, 0, 0, 0 };

SQBool clear_display ( SQBool _on )
{
   for ( int row = 0; row < ROWS; row ++ )
   {
      if ( _on == SQ_TRUE )
      {
         display[row] = 0xFF;
      }
      else
      {
         display[row] = 0x00;
      }
   }
   return SQ_TRUE;
}

SQBool set_display ( int _column, int _row, SQBool _on )
{
   if ( _column < 0 || _column >= COLUMNS || _row < 0 || _row >= ROWS )
   {
      return SQ_FALSE;
   }
   
   if ( _on == SQ_TRUE )
   {
      display[_row] = display[_row] | (1 << _column);
   }
   else
   {
      display[_row] = display[_row] & ~(1 << _column);
   }
   return SQ_TRUE;
}

SQBool get_display ( int _column, int _row )
{
   if ( (display[_row] & (1 << _column)) > 0 )
   {
      return SQ_TRUE;
   }
   else
   {
      return SQ_FALSE;
   }
}

void draw_display ()
{
   for ( int row = 0; row < ROWS; row ++ )
   {
      digitalWrite ( row + 2, LOW );
      for ( int column = 0; column < COLUMNS; column++ )
      {
         if ( get_display(column, row) == SQ_TRUE )
         {
            digitalWrite ( column + 7, HIGH );
         }
         else
         {
            digitalWrite ( column + 7, LOW );
         }
      }
      digitalWrite ( row + 2, HIGH );
      for ( int column = 0; column < COLUMNS; column++ )
      {
         digitalWrite ( column + 7, LOW );
      }
   }
}

SQBool digital_io_write ( int _pin, SQBool _high )
{
   digitalWrite ( _pin, _high == SQ_TRUE ? HIGH : LOW );
   return SQ_TRUE;
}

char * firmware_version ( void )
{
   return sq_get_constant_string(SQ_STRING_CONSTANT(SVN_REVISION));
}

int main ()
{
   int i;
   for ( i = 2; i <= 13; i ++ )
   {
      pinMode ( i, OUTPUT );
   }
   
   static SQServer server;
   
   sq_init ();   
   
   sq_server_init ( &server, 4321 );
   
   while ( SQ_TRUE )
   {
      sq_server_poll ( &server );
      draw_display();
   }
   
   sq_shutdown ();   
}
