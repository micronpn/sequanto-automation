#include <cassert>

#include <sequanto/tree.h>
#include <sequanto/protocol.h>
#include <stdexcept>

using namespace std;
using namespace sequanto::automation;

Tree::Tree ()
   : m_root(NULL)
{
}

Node * Tree::FindNode ( const char * _path )
{
   assert ( m_root != NULL );

   string path ( _path );
   if ( path.empty() || path[0] != '/' )
   {
      return NULL;
   }
   if ( path == "/" )
   {
      return m_root;
   }
   // Skip the first slash
   size_t start = 1;
   size_t end = path.find ( '/', start );
   Node * node = m_root;
   while ( end != string::npos )
   {
      node = node->FindChild ( path.substr(start, end - start) );
      if ( node == NULL )
      {
         return NULL;
      }
      start = end + 1;
      end = path.find ( '/', start );
   }
   node = node->FindChild ( path.substr ( start, path.size() - 1 ) );
   return node;
}

SQBool Tree::HandleInfo(SQStream * _stream, const char * _path)
{
   Node * node = FindNode ( _path );
   if ( node == NULL )
   {
      return SQ_FALSE;
   }
   const NodeInfo & nodeInfo = node->Info();

   sq_stream_enter_write ( _stream );
   sq_stream_write_string ( _stream, "+INFO " );
   switch ( nodeInfo.GetType() )
   {
   case NodeInfo::SQ_NODE_TYPE_CALLABLE:
      sq_stream_write_string ( _stream, "Callable " );
      sq_protocol_write_type ( _stream, nodeInfo.GetReturnType() );
      for ( size_t i = 0; i < nodeInfo.GetNumberOfParameters(); i++ )
      {
         sq_stream_write_byte ( _stream, ' ' );
         sq_protocol_write_type ( _stream, nodeInfo.GetParameterType(i) );
      }
      break;

   case NodeInfo::SQ_NODE_TYPE_LIST:
      sq_stream_write_string ( _stream, "List" );
      break;

   case NodeInfo::SQ_NODE_TYPE_READABLE_PROPERTY:
      sq_stream_write_string ( _stream, "ReadableProperty " );
      sq_protocol_write_type ( _stream, nodeInfo.GetReturnType() );
      break;

   case NodeInfo::SQ_NODE_TYPE_WRITABLE_PROPERTY:
      sq_stream_write_string ( _stream, "WritableProperty " );
      sq_protocol_write_type ( _stream, nodeInfo.GetReturnType() );
      break;

   case NodeInfo::SQ_NODE_TYPE_READ_WRITE_PROPERTY:
      sq_stream_write_string ( _stream, "ReadWriteProperty " );
      sq_protocol_write_type ( _stream, nodeInfo.GetReturnType() );
      break;

   case NodeInfo::SQ_NODE_TYPE_MONITOR:
      sq_stream_write_string ( _stream, "Monitor" );
      for ( size_t i = 0; i < nodeInfo.GetNumberOfParameters(); i++ )
      {
         sq_stream_write_byte ( _stream, ' ' );
         sq_protocol_write_type ( _stream, nodeInfo.GetParameterType(i) );
      }
      break;

   default:
      throw runtime_error ( "SequantoAutomation_CXX: Unsupported NodeType." );
   }
   sq_stream_write_string ( _stream, "\r\n" );
   sq_stream_exit_write ( _stream );
   return SQ_TRUE;
}

SQBool Tree::HandleList(SQStream * _stream, const char * _path)
{
   Node * node = FindNode ( _path );
   if ( node == NULL )
   {
      return SQ_FALSE;
   }
   const NodeInfo & nodeInfo = node->Info();
   if ( nodeInfo.GetType() == NodeInfo::SQ_NODE_TYPE_LIST )
   {
      sq_stream_enter_write ( _stream );
      sq_stream_write_string ( _stream, "+LIST " );
      Node::Iterator * it = node->ListChildren();
      for ( ; it->HasNext (); it->Next() )
      {
         Node * node = it->GetCurrent();
         sq_stream_write_string ( _stream, node->GetName().c_str() );
         sq_stream_write_byte ( _stream, ' ' );
      }
      delete it;
      sq_stream_write_string ( _stream, "\r\n" );
      sq_stream_exit_write ( _stream );
   }
   else
   {
      sq_protocol_write_failure_with_text_message ( _stream, "The given path does not point to a list" );
   }
   return SQ_TRUE;
}

SQBool Tree::HandleEnable ( SQStream * _stream, const char * _path )
{
   Node * node = FindNode ( _path );
   if ( node == NULL )
   {
      return SQ_FALSE;
   }
   const NodeInfo & nodeInfo = node->Info();

   if ( nodeInfo.GetType() == NodeInfo::SQ_NODE_TYPE_MONITOR )
   {
      try
      {
         node->HandleMonitorStateChange ( true );
         sq_protocol_write_success_message ( _stream );
      }
      catch ( const std::exception & _ex )
      {
         sq_protocol_write_failure_with_text_message ( _stream, _ex.what() );
      }
   }
   else
   {
      sq_protocol_write_failure_with_text_message ( _stream, "The given path does not point to a monitor" );
   }

   return SQ_TRUE;
}

SQBool Tree::HandleDisable ( SQStream * _stream, const char * _path )
{
   Node * node = FindNode ( _path );
   if ( node == NULL )
   {
      return SQ_FALSE;
   }
   const NodeInfo & nodeInfo = node->Info();

   if ( nodeInfo.GetType() == NodeInfo::SQ_NODE_TYPE_MONITOR )
   {
      try
      {
         node->HandleMonitorStateChange ( false );
         sq_protocol_write_success_message ( _stream );
      }
      catch ( const std::exception & _ex )
      {
         sq_protocol_write_failure_with_text_message ( _stream, _ex.what() );
      }
   }
   else
   {
      sq_protocol_write_failure_with_text_message ( _stream, "The given path does not point to a monitor" );
   }

   return SQ_TRUE;
}

SQBool Tree::HandleGet ( SQStream * _stream, const char * _path )
{
   Node * node = FindNode ( _path );
   if ( node == NULL )
   {
      return SQ_FALSE;
   }
   const NodeInfo & nodeInfo = node->Info();

   if ( (nodeInfo.GetType() & NodeInfo::SQ_NODE_TYPE_ANY_READABLE_PROPERTY) != 0 )
   {
      SQValue value;
      sq_value_init ( &value );
      try
      {
         node->HandleGet ( value );
         WriteOkValue ( _stream, value );
      }
      catch ( const std::exception & _ex )
      {
         sq_protocol_write_failure_with_text_message ( _stream, _ex.what() );
      }
      sq_value_free ( &value );
   }
   else
   {
      sq_protocol_write_failure_with_text_message ( _stream, "The given path does not point to a readable property" );
   }

   return SQ_TRUE;
}

SQBool Tree::HandleSet ( SQStream * _stream, const char * _path, const SQValue * const _value )
{
   Node * node = FindNode ( _path );
   if ( node == NULL )
   {
      return SQ_FALSE;
   }
   const NodeInfo & nodeInfo = node->Info();

   if ( (nodeInfo.GetType() & NodeInfo::SQ_NODE_TYPE_ANY_WRITABLE_PROPERTY) != 0 )
   {
      try
      {
         node->HandleSet ( _value );
         sq_protocol_write_success_message ( _stream );
      }
      catch ( const std::exception & _ex )
      {
         sq_protocol_write_failure_with_text_message ( _stream, _ex.what() );
      }
   }
   else
   {
      sq_protocol_write_failure_with_text_message ( _stream, "The given path does not point to a writable property" );
   }
   return SQ_TRUE;
}

SQBool Tree::HandleCall ( SQStream * _stream, const char * _path, const SQValue * const _values, int _numberOfValues )
{
   Node * node = FindNode ( _path );
   if ( node == NULL )
   {
      return SQ_FALSE;
   }
   const NodeInfo & nodeInfo = node->Info();

   if ( (nodeInfo.GetType() & NodeInfo::SQ_NODE_TYPE_CALLABLE) != 0 )
   {
      SQValue outputValue;
      sq_value_init ( &outputValue );
      try
      {
         node->HandleCall ( _numberOfValues, _values, outputValue );
         if ( outputValue.m_type == VALUE_TYPE_NO_VALUE )
         {
            sq_protocol_write_success_message ( _stream );
         }
         else
         {
            WriteOkValue ( _stream, outputValue );
         }
      }
      catch ( const std::exception & _ex )
      {
         sq_protocol_write_failure_with_text_message ( _stream, _ex.what() );
      }
      sq_value_free ( &outputValue );
   }
   else
   {
      sq_protocol_write_failure_with_text_message ( _stream, "The given path does not point to a writable property" );
   }
   return SQ_TRUE;
}

void Tree::SetRoot ( Node * _root )
{
   m_root = _root;
}

void Tree::WriteOkValue(SQStream *_stream, const SQValue &_value)
{
   sq_protocol_write_success_with_values_message ( _stream, &_value, 1 );
}
