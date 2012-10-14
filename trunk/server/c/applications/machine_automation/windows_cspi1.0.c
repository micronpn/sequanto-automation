#include <sequanto/automation.h>
#include <cspi/spi.h>
#include <memory.h>

void windows_init_subsystem ()
{
    SPI_init();
}

void windows_deinit_subsystem ()
{
    SPI_exit();
}

Accessible * windows_to_accessible ( const SQByteArray * _pointer )
{
   Accessible * accessibleObject = NULL;
   memcpy ( &accessibleObject, _pointer->m_start, sizeof(Accessible*) );
   return accessibleObject;
}

SQByteArray * windows_from_accessible ( const Accessible * _accessibleObject )
{
   SQByteArray * ret = sq_byte_array_create_prealloc ( sizeof(Accessible*) );
   memcpy ( ret->m_start, &_accessibleObject, sizeof(Accessible*) );
   return ret;
}

int windows_desktops ()
{
    return SPI_getDesktopCount ();
}

SQByteArray * windows_desktop ( int _desktop )
{
    return windows_from_accessible ( SPI_getDesktop ( _desktop ) );
}

void windows_ref ( SQByteArray * _pointer )
{
   Accessible * accessibleObject = windows_to_accessible(_pointer);
   Accessible_ref ( accessibleObject );
}

void windows_unref ( SQByteArray * _pointer )
{
   Accessible * accessibleObject = windows_to_accessible(_pointer);
   Accessible_unref ( accessibleObject );
}

char * windows_name ( SQByteArray * _pointer )
{
   Accessible * accessibleObject = windows_to_accessible(_pointer);
   return Accessible_getName ( accessibleObject );
}

long windows_children ( SQByteArray * _pointer )
{
   Accessible * accessibleObject = windows_to_accessible(_pointer);
   return Accessible_getChildCount ( accessibleObject );
}

SQByteArray * windows_child ( SQByteArray * _parent, long _index )
{
   Accessible * accessibleObject = windows_to_accessible(_parent);
   return windows_from_accessible ( Accessible_getChildAtIndex ( accessibleObject, _index ) );
}
