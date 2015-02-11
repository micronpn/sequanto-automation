#include <sequanto/automation.h>
#include <cspi/spi.h>
#include <memory.h>

#include "config.h"

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

char * windows_string ( char * _str )
{
    char * ret = SQ_STRDUP_FUNCTION(_str);
    SPI_freeString ( _str );
    return ret;
}

char * windows_name ( SQByteArray * _pointer )
{
   Accessible * accessibleObject;
   accessibleObject = windows_to_accessible(_pointer);
   return windows_string ( Accessible_getName ( accessibleObject ) );
}

int windows_process_id ( SQByteArray * _pointer )
{
   Accessible * accessibleObject = windows_to_accessible(_pointer);
   AccessibleApplication * application = Accessible_getHostApplication(accessibleObject);
   long ret = AccessibleApplication_getID ( application );
   AccessibleApplication_unref ( application );
   return ret;
}

int windows_x ( SQByteArray * _pointer )
{
   long int x = -1;
   long int y = -1;
   Accessible * accessibleObject = windows_to_accessible(_pointer);
   if ( Accessible_isComponent(accessibleObject) == SPI_TRUE )
   {
       AccessibleComponent * component = Accessible_getComponent(accessibleObject);
       AccessibleComponent_getPosition (component, &x, &y, SPI_COORD_TYPE_SCREEN);
   }
   return x;
}

int windows_y ( SQByteArray * _pointer )
{
   long int x = -1;
   long int y = -1;
   Accessible * accessibleObject = windows_to_accessible(_pointer);
   if ( Accessible_isComponent(accessibleObject) == SPI_TRUE )
   {
       AccessibleComponent * component = Accessible_getComponent(accessibleObject);
       AccessibleComponent_getPosition (component, &x, &y, SPI_COORD_TYPE_SCREEN);
   }
   return y;
}

int windows_width ( SQByteArray * _pointer )
{
   long int width = 0;
   long int height = 0;
   Accessible * accessibleObject = windows_to_accessible(_pointer);
   if ( Accessible_isComponent(accessibleObject) == SPI_TRUE )
   {
       AccessibleComponent * component = Accessible_getComponent(accessibleObject);
       AccessibleComponent_getSize (component, &width, &height);
   }
   return width;
}

int windows_height ( SQByteArray * _pointer )
{
   long int width = 0;
   long int height = 0;
   Accessible * accessibleObject = windows_to_accessible(_pointer);
   if ( Accessible_isComponent(accessibleObject) == SPI_TRUE )
   {
       AccessibleComponent * component = Accessible_getComponent(accessibleObject);
       AccessibleComponent_getSize (component, &width, &height);
   }
   return height;
}

char * windows_role ( SQByteArray * _pointer )
{
   Accessible * accessibleObject = windows_to_accessible(_pointer);
   return windows_string ( Accessible_getRoleName (accessibleObject) );
}

char * windows_text ( SQByteArray * _pointer )
{
   char * text = NULL;
   Accessible * accessibleObject = windows_to_accessible(_pointer);
   if ( Accessible_isText(accessibleObject) == SPI_TRUE )
   {
       AccessibleText * textObj = Accessible_getText(accessibleObject);
       long count = AccessibleText_getCharacterCount ( textObj );
       text = AccessibleText_getText ( textObj, 0, count );
       AccessibleText_unref ( textObj );
       return windows_string(text);
   }
   else if ( Accessible_isValue(accessibleObject) == SPI_TRUE )
   {
       AccessibleValue * valueObj = Accessible_getValue(accessibleObject);

       double current = AccessibleValue_getCurrentValue ( valueObj );
       text = malloc(SQ_MAX_VALUE_LENGTH);
       SNPRINTF_FUNCTION ( text, SQ_MAX_VALUE_LENGTH, "%f", current );
       return text;
   }
   return SQ_STRDUP_FUNCTION("");
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

int windows_actions ( SQByteArray * _pointer )
{
   long count = 0;
   Accessible * accessibleObject = windows_to_accessible(_pointer);
   if ( Accessible_isAction(accessibleObject) )
   {
       AccessibleAction * actionObj = Accessible_getAction(accessibleObject);
       count = (int) AccessibleAction_getNActions ( actionObj );
   }
   return count;
}

char * windows_action_name ( SQByteArray * _pointer, int _actionIndex )
{
   Accessible * accessibleObject = windows_to_accessible(_pointer);
   if ( Accessible_isAction(accessibleObject) )
   {
       AccessibleAction * actionObj = Accessible_getAction(accessibleObject);
       char * name = AccessibleAction_getName ( actionObj, _actionIndex );
       return windows_string ( name );
   }
   return SQ_STRDUP_FUNCTION("UNKNOWN");
}

void windows_action_do ( SQByteArray * _pointer, int _actionIndex )
{
   Accessible * accessibleObject = windows_to_accessible(_pointer);
   if ( Accessible_isAction(accessibleObject) )
   {
       AccessibleAction * actionObj = Accessible_getAction(accessibleObject);
       AccessibleAction_doAction ( actionObj, _actionIndex );
   }
}
