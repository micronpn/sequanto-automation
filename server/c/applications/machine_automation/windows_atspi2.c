#include <sequanto/automation.h>
#include <atspi/atspi.h>
#include <memory.h>

void windows_init_subsystem ()
{
    atspi_init();
}

void windows_deinit_subsystem ()
{
    atspi_exit();
}

AtspiAccessible * windows_to_accessible ( const SQByteArray * _pointer )
{
   AtspiAccessible * accessibleObject = NULL;
   memcpy ( &accessibleObject, _pointer->m_start, sizeof(AtspiAccessible*) );
   
   g_assert ( ATSPI_IS_ACCESSIBLE(accessibleObject) );
   
   return accessibleObject;
}

SQByteArray * windows_from_accessible ( const AtspiAccessible * _accessibleObject )
{
   SQByteArray * ret = sq_byte_array_create_prealloc ( sizeof(AtspiAccessible*) );
   memcpy ( ret->m_start, &_accessibleObject, sizeof(AtspiAccessible*) );
   return ret;
}

int windows_desktops ()
{
    return atspi_get_desktop_count();
}

SQByteArray * windows_desktop ( int _desktop )
{
    AtspiAccessible * accessible = atspi_get_desktop(_desktop);
    return windows_from_accessible ( accessible );
}

void windows_ref ( SQByteArray * _pointer )
{
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
}

void windows_unref ( SQByteArray * _pointer )
{
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
}

char * windows_name ( SQByteArray * _pointer )
{
   GError * error = NULL;
   gchar *name;
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
   name = atspi_accessible_get_name ( accessibleObject, &error );
   if ( error != NULL )
   {
       g_error_free(error);
       return strdup("ERROR");
   }
   else
   {
       return name;
   }
}

long windows_children ( SQByteArray * _pointer )
{
   GError * error = NULL;
   gint childCount;
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
   
   childCount = atspi_accessible_get_child_count ( accessibleObject, &error );
   if ( error != NULL )
   {
       g_error_free(error);
       return 0;
   }
   else
   {
       return childCount;
   }
}

SQByteArray * windows_child ( SQByteArray * _parent, long _index )
{
   GError * error = NULL;
   AtspiAccessible * child;
   AtspiAccessible * accessibleObject = windows_to_accessible(_parent);
   
   child = atspi_accessible_get_child_at_index ( accessibleObject, _index, &error );
   
   if ( error != NULL )
   {
       g_error_free(error);
       return sq_byte_array_create_prealloc ( 0 );
   }
   else
   {
       return windows_from_accessible ( child );
   }
}
