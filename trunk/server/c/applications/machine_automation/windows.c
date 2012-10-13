#include <sequanto/automation.h>

#ifdef SQ_WINDOWS_AT_SPI_C

void windows_init_subsystem ()
{
}

void windows_deinit_subsystem ()
{
}

AccessibleObject * windows_to_accessible ( SQByteArray * _pointer )
{
   AccessibleObject * accessibleObject = NULL;
   memcpy ( _pointer->m_start, &accessibleObject );
   return accessibleObject;
}

SQByteArray * windows_from_accessible ( AccessibleObject * _accessibleObject )
{
   SQByteArray * ret = sq_byte_array_create_prealloc ( sizeof(AccessibleObject*) );
   memcpy ( _accessibleObject, ret->m_start );
   return ret;
}

SQByteArray * windows_desktop ( int _desktop )
{
}

void windows_ref ( SQByteArray * _pointer )
{
   AccessibleObject * accessibleObject = windows_to_accessible(_pointer);
   AccessibleObject_ref ( accessibleObject );
}

void windows_unref ( SQByteArray * _pointer )
{
   AccessibleObject * accessibleObject = windows_to_accessible(_pointer);
   AccessibleObject_unref ( accessibleObject );
}

char * windows_name ( SQByteArray * _pointer )
{
   AccessibleObject * accessibleObject = windows_to_accessible(_pointer);
   return AccessibleObject_name ( accessibleObject );
}

#else

void windows_init_subsystem ()
{
}

void windows_deinit_subsystem ()
{
}

SQByteArray * windows_desktop ( int _desktop )
{
   return sq_byte_array_create_prealloc ( 0 );
}

void windows_ref ( SQByteArray * _pointer )
{
}

void windows_unref ( SQByteArray * _pointer )
{
}

static char EMPTY[] = "";

char * windows_name ( SQByteArray * _pointer )
{
   return EMPTY;
}

long windows_children ( SQByteArray * _pointer )
{
   return 0;
}

SQByteArray * windows_child ( SQByteArray * _pointer, long _childNum )
{
   return sq_byte_array_create_prealloc ( 0 );
}

#endif
