static const char INCORRECT_TYPE_GIVEN[] SQ_CONST_VARIABLE = "Incorrect type given";
static const char PLUS_SPACE[] SQ_CONST_VARIABLE = "+ ";
static const char ILLEGAL_PARAMETERS[] SQ_CONST_VARIABLE = "Illegal parameters.";
static const char COULD_NOT_FIND_GIVEN_OBJECT[] SQ_CONST_VARIABLE = "Could not find given object.";
static const char OBJECT_IS_NOT_A_PROPERTY[] SQ_CONST_VARIABLE = "Object is not a property.";
static const char OBJECT_IS_NOT_A_LIST[] SQ_CONST_VARIABLE = "Object is not a list.";
static const char OBJECT_IS_NOT_A_CALLABLE[] SQ_CONST_VARIABLE = "Object is not a callable.";

#ifdef SQ_ARDUINO
static SQInfo bufferedInfo;
static SQCallableInfo bufferedCallableInfo;
static SQPropertyInfo bufferedPropertyInfo;

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

#else

const SQInfo * const sq_get_info ( size_t _index )
{
   return &INFO_LIST[_index];
}

const SQCallableInfo * const sq_get_callable_info ( size_t _index )
{
   return &CALLABLE_LIST[_index];
}

const SQPropertyInfo * const sq_get_property_info ( size_t _index )
{
   return &PROPERTY_LIST[_index];
}

#endif

const SQInfo* const sq_automation_find_info ( const char * const _name, int * _foundIndex )
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
   
   sq_stream_enter_write ( _stream );
   sq_stream_write_string ( _stream, sq_get_constant_string(PLUS_SPACE) );
   propertyInfo->get(_stream);
   sq_stream_write_string ( _stream, sq_get_constant_string(NEWLINE) );
   sq_stream_exit_write ( _stream );
}

void sq_automation_property_set ( const SQInfo * const _info, SQStream * _stream, const SQValue * const _value )
{
   const SQPropertyInfo * const propertyInfo = sq_get_property_info(_info->index);

   sq_stream_enter_write ( _stream );
   if ( _value->m_type != propertyInfo->type )
   {
      sq_protocol_write_failure_with_text ( _stream, sq_get_constant_string(INCORRECT_TYPE_GIVEN) );
   }
   else
   {
      propertyInfo->set ( _value );
      sq_protocol_write_success ( _stream );
   }
   sq_stream_exit_write ( _stream );
}

void sq_automation_call ( const SQInfo * const _info, SQStream * _stream, const SQValue * const _inputValues, int _numberOfValues )
{
   int i;
   const SQCallableInfo * const callableInfo = sq_get_callable_info(_info->index);

   sq_stream_enter_write ( _stream );
   for ( i = 0; i < NUMBER_OF_PARAMETERS; i++ )
   {
      if ( _inputValues[i].m_type != sq_automation_get_parameter(callableInfo, i) )
      {
         sq_protocol_write_failure_with_text ( _stream, sq_get_constant_string(ILLEGAL_PARAMETERS) );
         sq_stream_exit_write ( _stream );
         return;
      }
   }
   sq_stream_write_string ( _stream, sq_get_constant_string(PLUS_SPACE) );
   callableInfo->function ( _stream, _inputValues );
   sq_stream_write_string ( _stream, sq_get_constant_string(NEWLINE) );
   sq_stream_exit_write ( _stream );
}

void sq_parser_call ( SQParser * _parser, SQStream * _stream, const char * const _objectPath, const SQValue * const _inputValues, int _numberOfValues )
{
   const SQInfo * const info = sq_automation_find_info ( _objectPath, NULL );

   sq_stream_enter_write ( _stream );
   if ( info == NULL )
   {
      sq_protocol_write_failure_with_text ( _stream, sq_get_constant_string(COULD_NOT_FIND_GIVEN_OBJECT) );
      sq_stream_exit_write ( _stream );
      return;
   }
   
   if ( info->type != INFO_TYPE_CALLABLE )
   {
      sq_protocol_write_failure_with_text ( _stream, sq_get_constant_string(OBJECT_IS_NOT_A_CALLABLE) );
      sq_stream_exit_write ( _stream );
      return;
   }
   
   sq_automation_call ( info, _stream, _inputValues, _numberOfValues );
   sq_stream_exit_write ( _stream );
}

void sq_parser_property_get ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
   const SQInfo * const info = sq_automation_find_info ( _objectPath, NULL );

   sq_stream_enter_write ( _stream );
   if ( info == NULL )
   {
      sq_protocol_write_failure_with_text ( _stream, sq_get_constant_string(COULD_NOT_FIND_GIVEN_OBJECT) );
      sq_stream_exit_write ( _stream );
      return;
   }
   
   if ( info->type != INFO_TYPE_PROPERTY )
   {
      sq_protocol_write_failure_with_text ( _stream, sq_get_constant_string(OBJECT_IS_NOT_A_PROPERTY) );
      sq_stream_exit_write ( _stream );
      return;
   }
   
   sq_automation_property_get ( info, _stream );
   sq_stream_exit_write ( _stream );
}

void sq_parser_property_set ( SQParser * _parser, SQStream * _stream, const char * const _objectPath, const SQValue * const _value )
{
   const SQInfo * const info = sq_automation_find_info ( _objectPath, NULL );

   sq_stream_enter_write ( _stream );
   if ( info == NULL )
   {
      sq_protocol_write_failure_with_text ( _stream, sq_get_constant_string(COULD_NOT_FIND_GIVEN_OBJECT) );
      sq_stream_exit_write ( _stream );
      return;
   }
   
   if ( info->type != INFO_TYPE_PROPERTY )
   {
      sq_protocol_write_failure_with_text ( _stream, sq_get_constant_string(OBJECT_IS_NOT_A_PROPERTY) );
      sq_stream_exit_write ( _stream );
      return;
   }
   
   sq_automation_property_set ( info, _stream, _value );
   sq_stream_exit_write ( _stream );
}

void sq_parser_info ( SQParser * _parser, SQStream * _stream, const char * const _objectPath )
{
   int i;
   const SQCallableInfo * callableInfo;
   const SQPropertyInfo * propertyInfo;
   const SQInfo * const info = sq_automation_find_info ( _objectPath, NULL );
   
   sq_stream_enter_write ( _stream );
   if ( info == NULL )
   {
      sq_protocol_write_failure_with_text ( _stream, sq_get_constant_string(COULD_NOT_FIND_GIVEN_OBJECT) );
      sq_stream_exit_write ( _stream );
      return;
   }
   
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
   size_t objectPathLength = strlen(_objectPath);
   
   info = sq_automation_find_info ( _objectPath, &foundIndex );
   sq_stream_enter_write ( _stream );
   if ( info == NULL )
   {
      sq_protocol_write_failure_with_text ( _stream, sq_get_constant_string(COULD_NOT_FIND_GIVEN_OBJECT) );
      sq_stream_exit_write ( _stream );
      return;
   }
   
   if ( info->type != INFO_TYPE_LIST )
   {
      sq_protocol_write_failure_with_text ( _stream, sq_get_constant_string(OBJECT_IS_NOT_A_LIST) );
      sq_stream_exit_write ( _stream );
      return;
   }
   
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
