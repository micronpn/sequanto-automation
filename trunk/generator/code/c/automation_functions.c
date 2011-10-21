static const char INCORRECT_TYPE_GIVEN[] SQ_CONST_VARIABLE = "Incorrect type given";
static const char ILLEGAL_PARAMETERS[] SQ_CONST_VARIABLE = "Illegal parameters.";
static const char COULD_NOT_FIND_GIVEN_OBJECT[] SQ_CONST_VARIABLE = "Could not find given object.";
static const char OBJECT_IS_NOT_A_PROPERTY[] SQ_CONST_VARIABLE = "Object is not a property.";
static const char OBJECT_IS_NOT_A_LIST[] SQ_CONST_VARIABLE = "Object is not a list.";
static const char OBJECT_IS_NOT_A_CALLABLE[] SQ_CONST_VARIABLE = "Object is not a callable.";
static const char OBJECT_IS_NOT_A_MONITOR[] SQ_CONST_VARIABLE = "Object is not a monitor.";

#ifdef SQ_ARDUINO
static SQInfo bufferedInfo;
static SQCallableInfo bufferedCallableInfo;
static SQPropertyInfo bufferedPropertyInfo;
static SQMonitorInfo bufferedMonitorInfo;

const SQInfo * const sq_get_info ( size_t _index )
{
   memcpy_P ( &bufferedInfo, &INFO_LIST[_index], sizeof(SQInfo) );
   return &bufferedInfo;
}

const SQCallableInfo * const sq_get_callable_info ( size_t _index )
{
   memcpy_P ( &bufferedCallableInfo, &CALLABLE_LIST[_index], sizeof(SQCallableInfo) );
   return &bufferedCallableInfo;
}

const SQPropertyInfo * const sq_get_property_info ( size_t _index )
{
   memcpy_P ( &bufferedPropertyInfo, &PROPERTY_LIST[_index], sizeof(SQPropertyInfo) );
   return &bufferedPropertyInfo;
}

const SQMonitorInfo * const sq_get_monitor_info ( size_t _index )
{
   memcpy_P ( &bufferedMonitorInfo, &MONITOR_LIST[_index], sizeof(SQMonitorInfo) );
   return &bufferedMonitorInfo;
}

#else

const SQInfo * sq_get_info ( size_t _index )
{
   return &INFO_LIST[_index];
}

const SQCallableInfo * sq_get_callable_info ( size_t _index )
{
   return &CALLABLE_LIST[_index];
}

const SQPropertyInfo * sq_get_property_info ( size_t _index )
{
   return &PROPERTY_LIST[_index];
}

const SQMonitorInfo * sq_get_monitor_info ( size_t _index )
{
   return &MONITOR_LIST[_index];
}

#endif

const SQBranch * sq_automation_find_branch ( const char * const _name )
{
   int i;
   for ( i = 0; i < NUMBER_OF_BRANCHES; i++ )
   {
      if ( SQ_CONSTANT_STRNCMP(_name, SQ_STRING_CONSTANT_FROM_ARRAY(&BRANCH_LIST[i].name), BRANCH_LIST[i].length) == 0 )
      {
         return &BRANCH_LIST[i];
      }
   }
   return NULL;
}

const char * sq_automation_branch_name ( const SQBranch * const _branch, const char * const _name )
{
   const char * branchName = _name + _branch->length;
   if ( branchName[0] == '\0' )
   {
      return ROOT;
   }
   else
   {
      return branchName;
   }
}

SQBool sq_automation_handle_list_if_branch ( SQStream * _stream, const char * const _name )
{
   SQBool ret = SQ_FALSE;
   const SQBranch * const branch = sq_automation_find_branch ( _name );

   if ( branch != NULL )
   {
      ret = branch->list_handler ( _stream, sq_automation_branch_name(branch, _name) );
      return ret;
   }
   return SQ_FALSE;
}

SQBool sq_automation_handle_info_if_branch ( SQStream * _stream, const char * const _name )
{
   SQBool ret = SQ_FALSE;
   const SQBranch * const branch = sq_automation_find_branch ( _name );
   if ( branch != NULL )
   {
      ret = branch->info_handler ( _stream, sq_automation_branch_name(branch, _name) );
      return ret;

   }
   return SQ_FALSE;
}

SQBool sq_automation_handle_get_if_branch ( SQStream * _stream, const char * const _name )
{
   SQBool ret = SQ_FALSE;
   const SQBranch * const branch = sq_automation_find_branch ( _name );
   if ( branch != NULL )
   {
      ret = branch->get_handler ( _stream, sq_automation_branch_name(branch, _name) );
      return ret;

   }
   return SQ_FALSE;
}

SQBool sq_automation_handle_set_if_branch ( SQStream * _stream, const char * const _name, const SQValue * const _value )
{
   SQBool ret = SQ_FALSE;
   const SQBranch * const branch = sq_automation_find_branch ( _name );
   if ( branch != NULL )
   {
      ret = branch->set_handler ( _stream, sq_automation_branch_name(branch, _name), _value );
      return ret;

   }
   return SQ_FALSE;
}

SQBool sq_automation_handle_enable_if_branch ( SQStream * _stream, const char * const _name )
{
   SQBool ret = SQ_FALSE;
   const SQBranch * const branch = sq_automation_find_branch ( _name );
   if ( branch != NULL )
   {
      ret = branch->enable_handler ( _stream, sq_automation_branch_name(branch, _name) );
      return ret;

   }
   return SQ_FALSE;
}

SQBool sq_automation_handle_disable_if_branch ( SQStream * _stream, const char * const _name )
{
   SQBool ret = SQ_FALSE;
   const SQBranch * const branch = sq_automation_find_branch ( _name );
   if ( branch != NULL )
   {
      ret = branch->enable_handler ( _stream, sq_automation_branch_name(branch, _name) );
      return ret;

   }
   return SQ_FALSE;
}

SQBool sq_automation_handle_call_if_branch ( SQStream * _stream, const char * const _name, const SQValue * const _values, int _numberOfValues )
{
   SQBool ret = SQ_FALSE;
   const SQBranch * const branch = sq_automation_find_branch ( _name );
   if ( branch != NULL )
   {
      ret = branch->call_handler ( _stream, sq_automation_branch_name(branch, _name), _values, _numberOfValues );
      return ret;
   }
   return SQ_FALSE;
}

const SQInfo * sq_automation_find_info ( const char * const _name, int * _foundIndex )
{
   int i;
   
   for ( i = 0; i < NUMBER_OF_INFO; i++ )
   {
      if ( SQ_CONSTANT_STRCMP(_name, SQ_STRING_CONSTANT_FROM_ARRAY(&INFO_LIST[i].name)) == 0 )
      {
         if ( _foundIndex != NULL )
         {
            *_foundIndex = i;
         }
         return sq_get_info ( i );
      }
   }
   return NULL;
}

void sq_automation_property_get ( const SQInfo * const _info, SQStream * _stream )
{
   const SQPropertyInfo * const propertyInfo = sq_get_property_info(_info->index);
   
   propertyInfo->get(_stream);
}

void sq_automation_property_set ( const SQInfo * const _info, SQStream * _stream, const SQValue * const _value )
{
   const SQPropertyInfo * const propertyInfo = sq_get_property_info(_info->index);

   if ( _value->m_type != propertyInfo->type )
   {
      sq_protocol_write_failure_with_text_message ( _stream, sq_get_constant_string(INCORRECT_TYPE_GIVEN) );
   }
   else
   {
      propertyInfo->set ( _value );
      sq_protocol_write_success_message ( _stream );
   }
}

void sq_automation_call ( const SQInfo * const _info, SQStream * _stream, const SQValue * const _inputValues, int _numberOfValues )
{
   int i;
   const SQCallableInfo * const callableInfo = sq_get_callable_info(_info->index);
   
   SQ_UNUSED_PARAMETER(_numberOfValues);
   
   for ( i = 0; i < NUMBER_OF_PARAMETERS && i < SQ_MAX_PARAMETERS; i++ )
   {
      if ( _inputValues[i].m_type != sq_automation_get_parameter(callableInfo, i) )
      {
         sq_protocol_write_failure_with_text_message ( _stream, sq_get_constant_string(ILLEGAL_PARAMETERS) );
         return;
      }
   }
   
   // The callable will write success and perhaps a return value
   
   callableInfo->function ( _stream, _inputValues );
}

void sq_automation_monitor_enable ( const SQInfo * const _info, SQStream * _stream, SQBool _enable )
{
   const SQMonitorInfo * const monitorInfo = sq_get_monitor_info(_info->index);

   sq_protocol_write_success_message ( _stream );
   
   monitor_state[monitorInfo->index] = _enable;
}

void sq_parser_call ( SQParser * _parser, SQStream * _stream, const char * const _objectPath, const SQValue * const _inputValues, int _numberOfValues )
{
   const SQInfo * info;

   SQ_UNUSED_PARAMETER(_parser);
   
   if ( sq_automation_handle_call_if_branch(_stream, _objectPath, _inputValues, _numberOfValues) == SQ_TRUE )
   {
      return;
   }
   
   info = sq_automation_find_info ( _objectPath, NULL );
   
   if ( info == NULL )
   {
      sq_protocol_write_failure_with_text_message ( _stream, sq_get_constant_string(COULD_NOT_FIND_GIVEN_OBJECT) );
      return;
   }
   
   if ( info->type != INFO_TYPE_CALLABLE )
   {
      sq_protocol_write_failure_with_text_message ( _stream, sq_get_constant_string(OBJECT_IS_NOT_A_CALLABLE) );
      return;
   }
   
   sq_automation_call ( info, _stream, _inputValues, _numberOfValues );
}

void sq_parser_property_get ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
   const SQInfo * info;

   SQ_UNUSED_PARAMETER(_parser);

   if ( sq_automation_handle_get_if_branch ( _stream, _objectPath ) == SQ_TRUE )
   {
      return;
   }
   
   info = sq_automation_find_info ( _objectPath, NULL );
   
   if ( info == NULL )
   {
      sq_protocol_write_failure_with_text_message ( _stream, sq_get_constant_string(COULD_NOT_FIND_GIVEN_OBJECT) );
      return;
   }
   
   if ( info->type != INFO_TYPE_PROPERTY )
   {
      sq_protocol_write_failure_with_text_message ( _stream, sq_get_constant_string(OBJECT_IS_NOT_A_PROPERTY) );
      return;
   }
   
   sq_automation_property_get ( info, _stream );
}

void sq_parser_property_set ( SQParser * _parser, SQStream * _stream, const char * const _objectPath, const SQValue * const _value )
{
   const SQInfo * info;

   SQ_UNUSED_PARAMETER(_parser);

   if ( sq_automation_handle_set_if_branch ( _stream, _objectPath, _value ) == SQ_TRUE )
   {
      return;
   }

   info = sq_automation_find_info ( _objectPath, NULL );

   if ( info == NULL )
   {
      sq_protocol_write_failure_with_text_message ( _stream, sq_get_constant_string(COULD_NOT_FIND_GIVEN_OBJECT) );
      return;
   }
   
   if ( info->type != INFO_TYPE_PROPERTY )
   {
      sq_protocol_write_failure_with_text_message ( _stream, sq_get_constant_string(OBJECT_IS_NOT_A_PROPERTY) );
      return;
   }
   
   sq_automation_property_set ( info, _stream, _value );
}

void sq_parser_info ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
   int i;
   const SQCallableInfo * callableInfo;
   const SQPropertyInfo * propertyInfo;
   const SQMonitorInfo * monitorInfo;
   const SQInfo * info;

   SQ_UNUSED_PARAMETER(_parser);
   
   if ( sq_automation_handle_info_if_branch ( _stream, _objectPath ) == SQ_TRUE )
   {
      return;
   }
   
   info = sq_automation_find_info ( _objectPath, NULL );
   if ( info == NULL )
   {
      sq_protocol_write_failure_with_text_message ( _stream, sq_get_constant_string(COULD_NOT_FIND_GIVEN_OBJECT) );
      return;
   }

   sq_stream_enter_write ( _stream );
   switch ( info->type )
   {
   case INFO_TYPE_LIST:
      sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("+INFO List")) );
      break;

   case INFO_TYPE_PROPERTY:
      sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("+INFO ")) );
      propertyInfo = sq_get_property_info(info->index);
      if ( propertyInfo->get != NULL )
      {
          if ( propertyInfo->set != NULL )
          {
              sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("ReadWriteProperty ")) );
          }
          else
          {
              sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("ReadableProperty ")) );
          }
      }
      else
      {
          sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("WritableProperty ")) );
      }
      sq_protocol_write_type ( _stream, propertyInfo->type );
      break;

   case INFO_TYPE_CALLABLE:
      sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("+INFO Callable ")) );
      callableInfo = sq_get_callable_info(info->index);
      sq_protocol_write_type ( _stream, callableInfo->returnType );
      for ( i = 0; i < NUMBER_OF_PARAMETERS && sq_automation_get_parameter(callableInfo, i) != VALUE_TYPE_NO_VALUE; i++ )
      {
         sq_stream_write_byte ( _stream, ' ' );
         sq_protocol_write_type ( _stream, sq_automation_get_parameter(callableInfo, i) );
      }
      break;
      
   case INFO_TYPE_MONITOR:
      sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("+INFO Monitor")) );
      monitorInfo = sq_get_monitor_info(info->index);
      for ( i = 0; i < NUMBER_OF_MONITOR_VALUES && sq_automation_get_monitor_value_type(monitorInfo, i) != VALUE_TYPE_NO_VALUE; i++ )
      {
         sq_stream_write_byte ( _stream, ' ' );
         sq_protocol_write_type ( _stream, sq_automation_get_monitor_value_type(monitorInfo, i) );
      }
      if ( monitor_state[monitorInfo->index] )
      {
         sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT(" Enabled")) );
      }
      else
      {
         sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT(" Disabled")) );
      }
      break;
      
   default:
      sq_stream_write_string ( _stream, "- \"Internal-Error: unknown type " );
      sq_protocol_write_integer ( _stream, info->type );
      sq_stream_write_string ( _stream, "\"" );
      break;
   }
   sq_stream_write_string ( _stream, sq_get_constant_string(NEWLINE) );
   sq_stream_exit_write ( _stream );
}

void sq_parser_list ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
   int foundIndex;
   size_t i;
   char c;
   const SQInfo * info;
   size_t objectPathLength;

   SQ_UNUSED_PARAMETER(_parser);
   
   if ( sq_automation_handle_list_if_branch ( _stream, _objectPath ) == SQ_TRUE )
   {
      return;
   }
   
   objectPathLength = strlen(_objectPath);
   
   info = sq_automation_find_info ( _objectPath, &foundIndex );
   if ( info == NULL )
   {
      sq_protocol_write_failure_with_text_message ( _stream, sq_get_constant_string(COULD_NOT_FIND_GIVEN_OBJECT) );
      return;
   }
   
   if ( info->type != INFO_TYPE_LIST )
   {
      sq_protocol_write_failure_with_text_message ( _stream, sq_get_constant_string(OBJECT_IS_NOT_A_LIST) );
      return;
   }

   sq_stream_enter_write ( _stream );
   sq_stream_write_string ( _stream, sq_get_constant_string(SQ_STRING_CONSTANT("+LIST ")) );
   
   foundIndex ++;
   for ( ; foundIndex < NUMBER_OF_INFO; foundIndex++ )
   {
      if ( SQ_CONSTANT_STRNCMP(_objectPath, SQ_STRING_CONSTANT_FROM_ARRAY(&INFO_LIST[foundIndex].name), objectPathLength ) == 0 )
      {
         if ( SQ_CONSTANT_STRCHR(SQ_STRING_CONSTANT_FROM_ARRAY(&INFO_LIST[foundIndex].name) + objectPathLength + 1, '/' ) == NULL )
         {
            i = objectPathLength;
            if ( objectPathLength != 1 )
            {
               i ++;
            }
            for ( c = SQ_CHARACTER_CONSTANT(SQ_STRING_CONSTANT_FROM_ARRAY(&INFO_LIST[foundIndex].name) + i); c != '\0'; i++ )
            {
               sq_stream_write_byte ( _stream,  c );

               c = SQ_CHARACTER_CONSTANT(SQ_STRING_CONSTANT_FROM_ARRAY(&INFO_LIST[foundIndex].name) + i + 1);
            }
            sq_stream_write_byte ( _stream, ' ' );
         }
      }
      else
      {
         break;
      }
   }
   sq_stream_write_string ( _stream, sq_get_constant_string(NEWLINE) );
   sq_stream_exit_write ( _stream );
}

void sq_parser_enable_internal ( SQParser * _parser, SQStream * _stream, const char * const _objectPath, SQBool _enable )
{
   const SQInfo * const info = sq_automation_find_info ( _objectPath, NULL );

   SQ_UNUSED_PARAMETER(_parser);
   
   if ( info == NULL )
   {
      sq_protocol_write_failure_with_text_message ( _stream, sq_get_constant_string(COULD_NOT_FIND_GIVEN_OBJECT) );
      return;
   }
   
   if ( info->type != INFO_TYPE_MONITOR )
   {
      sq_protocol_write_failure_with_text_message ( _stream, sq_get_constant_string(OBJECT_IS_NOT_A_MONITOR) );
      return;
   }

   sq_automation_monitor_enable ( info, _stream, _enable );
}

void sq_parser_enable ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
   if ( sq_automation_handle_enable_if_branch(_stream, _objectPath) == SQ_TRUE )
   {
      return;
   }
   
   sq_parser_enable_internal ( _parser, _stream, _objectPath, SQ_TRUE );
}

void sq_parser_disable ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
   if ( sq_automation_handle_disable_if_branch(_stream, _objectPath) == SQ_TRUE )
   {
      return;
   }
   
   sq_parser_enable_internal ( _parser, _stream, _objectPath, SQ_FALSE );
}
