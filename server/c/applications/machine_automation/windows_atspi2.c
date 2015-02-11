#include <sequanto/automation.h>
#include <atspi/atspi.h>
#include <memory.h>

#include "config.h"

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

char * windows_report_error ( GError * _error )
{
    char * ret;
    if ( _error->message != NULL )
    {
        ret = SQ_STRDUP_FUNCTION(_error->message);
    }
    else
    {
        ret = SQ_STRDUP_FUNCTION("ERROR");
    }
    g_error_free(_error);
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
    /* AtspiAccessible * accessibleObject = windows_to_accessible(_pointer); */
    SQ_UNUSED_PARAMETER(_pointer);
}

void windows_unref ( SQByteArray * _pointer )
{
    /* AtspiAccessible * accessibleObject = windows_to_accessible(_pointer); */
    SQ_UNUSED_PARAMETER(_pointer);
}

char * windows_name ( SQByteArray * _pointer )
{
   GError * error = NULL;
   gchar *name;
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
   name = atspi_accessible_get_name ( accessibleObject, &error );
   if ( error != NULL )
   {
       return windows_report_error(error);
   }
   else
   {
       return name;
   }
}

int windows_process_id ( SQByteArray * _pointer )
{
   GError * error = NULL;
   gint processId;
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
   processId = atspi_accessible_get_process_id ( accessibleObject, &error );
   if ( error != NULL )
   {
       g_error_free(error);
       return -1;
   }
   else
   {
       return processId;
   }
}

int windows_x ( SQByteArray * _pointer )
{
   GError * error = NULL;
   int x = -1;
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
   if ( ATSPI_IS_COMPONENT(accessibleObject) )
   {
       AtspiComponent * component = ATSPI_COMPONENT(accessibleObject);
       AtspiPoint *position = atspi_component_get_position (component, ATSPI_COORD_TYPE_SCREEN, &error);
       if ( error != NULL )
       {
           g_error_free(error);
       }
       if ( position != NULL )
       {
           x = position->x;
           g_free ( position );
       }
   }
   return x;
}

int windows_y ( SQByteArray * _pointer )
{
   GError * error = NULL;
   int y = -1;
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
   if ( ATSPI_IS_COMPONENT(accessibleObject) )
   {
       AtspiComponent * component = ATSPI_COMPONENT(accessibleObject);
       AtspiPoint *position = atspi_component_get_position (component, ATSPI_COORD_TYPE_SCREEN, &error);
       if ( error != NULL )
       {
           g_error_free(error);
       }
       if ( position != NULL )
       {
           y = position->y;
           g_free ( position );
       }
   }
   return y;
}

int windows_width ( SQByteArray * _pointer )
{
   GError * error = NULL;
   int width = -1;
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
   if ( ATSPI_IS_COMPONENT(accessibleObject) )
   {
       AtspiComponent * component = ATSPI_COMPONENT(accessibleObject);
       AtspiPoint * size = atspi_component_get_size (component, &error);
       if ( error != NULL )
       {
           g_error_free(error);
       }
       if ( size != NULL )
       {
           width = size->x;
           g_free ( size );
       }
   }
   return width;
}

int windows_height ( SQByteArray * _pointer )
{
   GError * error = NULL;
   int height = -1;
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
   if ( ATSPI_IS_COMPONENT(accessibleObject) )
   {
       AtspiComponent * component = ATSPI_COMPONENT(accessibleObject);
       AtspiPoint * size = atspi_component_get_size (component, &error);
       if ( error != NULL )
       {
           g_error_free(error);
       }
       if ( size != NULL )
       {
           height = size->y;
           g_free ( size );
       }
   }
   return height;
}

char * windows_role ( SQByteArray * _pointer )
{
   GError * error = NULL;
   char * role = NULL;
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
   role = atspi_accessible_get_role_name (accessibleObject, &error);
   if ( error != NULL )
   {
       return windows_report_error(error);
   }
   if ( role != NULL )
   {
       return role;
   }
   else
   {
       return SQ_STRDUP_FUNCTION("");
   }
}

char * windows_text ( SQByteArray * _pointer )
{
   GError * error = NULL;
   char * text = NULL;
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
   if ( ATSPI_IS_TEXT(accessibleObject) )
   {
       AtspiText * textObj = ATSPI_TEXT(accessibleObject);
       gint count = atspi_text_get_character_count ( textObj, &error );
       if ( error != NULL )
       {
           return windows_report_error ( error );
       }

       text = atspi_text_get_text (textObj, 0, count, &error);
       if ( error != NULL )
       {
           return windows_report_error(error);
       }
       else
       {
           return text;
       }
   }
   else if ( ATSPI_IS_VALUE(accessibleObject) )
   {
       AtspiValue * valueObj = ATSPI_VALUE(accessibleObject);

       gdouble current = atspi_value_get_current_value ( valueObj, &error );

       if ( error != NULL )
       {
           return windows_report_error(error);
       }
       else
       {
           return g_strdup_printf("%f", current);
       }
   }
   return SQ_STRDUP_FUNCTION("");
}


int windows_actions ( SQByteArray * _pointer )
{
   GError * error = NULL;
   gint count = 0;
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
   if ( ATSPI_IS_ACTION(accessibleObject) )
   {
       AtspiAction * actionObj = ATSPI_ACTION(accessibleObject);
       count = atspi_action_get_n_actions ( actionObj, &error );
       if ( error != NULL )
       {
           g_error_free(error);
       }
   }
   return count;
}

char * windows_action_name ( SQByteArray * _pointer, int _actionIndex )
{
   GError * error = NULL;
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
   if ( ATSPI_IS_ACTION(accessibleObject) )
   {
       AtspiAction * actionObj = ATSPI_ACTION(accessibleObject);
       gchar * name = atspi_action_get_name ( actionObj, _actionIndex, &error );
       if ( error != NULL )
       {
           return windows_report_error(error);
       }
       return name;
   }
   return SQ_STRDUP_FUNCTION("UNKNOWN");
}

void windows_action_do ( SQByteArray * _pointer, int _actionIndex )
{
   GError * error = NULL;
   AtspiAccessible * accessibleObject = windows_to_accessible(_pointer);
   if ( ATSPI_IS_ACTION(accessibleObject) )
   {
       AtspiAction * actionObj = ATSPI_ACTION(accessibleObject);
       atspi_action_do_action ( actionObj, _actionIndex, &error );
       if ( error != NULL )
       {
           g_error_free(error);
       }
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
       return NULL;
   }
   else
   {
       return windows_from_accessible ( child );
   }
}
