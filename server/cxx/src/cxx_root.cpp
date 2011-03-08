#include <sequanto/automation.h>
#include <sequanto/listnode.h>
#include <sequanto/tree.h>

#include "cxx_root.h"

static sequanto::automation::Tree s_tree;
static sequanto::automation::ListNode * s_root = NULL;

sequanto::automation::ListNode * sequanto::automation::sq_get_cxx_root ()
{
   if ( s_root == NULL )
   {
      s_root = new ListNode ( "<ROOT>" );
      s_tree.SetRoot ( s_root );
   }
   return s_root;
}

extern "C"
{
   void sq_parser_info ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
   {
      s_tree.HandleInfo ( _stream, _objectPath );
   }

   void sq_parser_list ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
   {
      s_tree.HandleList ( _stream, _objectPath );
   }

   void sq_parser_call ( SQParser * _parser, SQStream * _stream, const char * const _objectPath, const SQValue * const _inputValues, int _numberOfValues )
   {
      s_tree.HandleCall ( _stream, _objectPath, _inputValues, _numberOfValues );
   }

   void sq_parser_property_get ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
   {
      s_tree.HandleGet ( _stream, _objectPath );
   }
   
   void sq_parser_property_set ( SQParser * _parser, SQStream * _stream, const char * const _objectPath, const SQValue * _value )
   {
      s_tree.HandleSet ( _stream, _objectPath, _value );
   }

   void sq_parser_disable ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
   {
      s_tree.HandleDisable ( _stream, _objectPath );
   }

   void sq_parser_enable ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
   {
      s_tree.HandleEnable ( _stream, _objectPath );
   }
}
