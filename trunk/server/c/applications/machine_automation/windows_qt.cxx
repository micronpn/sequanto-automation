#include <sequanto/automation.h>
#include <memory.h>
#include <QtGui>

QWidget * screen;

char * ToString ( const QString & _string )
{
   QByteArray value ( _string.toUtf8() );
   return strdup(value.data());
}

extern "C"
{

void windows_init_subsystem ()
{
   screen = QApplication::desktop()->screen();
}

void windows_deinit_subsystem ()
{
}

QWidget * windows_to_widget ( const SQByteArray * _pointer )
{
   QWidget * widget = NULL;
   memcpy ( &widget, _pointer->m_start, sizeof(QWidget*) );
   
   if ( QApplication::allWidgets().contains(widget) )
   {
      return widget;
   }
   else
   {
      return NULL;
   }
}

SQByteArray * windows_from_widget ( const QWidget * _widget )
{
   SQByteArray * ret = sq_byte_array_create_prealloc ( sizeof(QWidget*) );
   memcpy ( ret->m_start, &_widget, sizeof(QWidget*) );
   return ret;
}

int windows_desktops ()
{
   return 1;
}

SQByteArray * windows_desktop ( int _desktop )
{
   if ( screen == NULL )
   {
      screen = QApplication::desktop()->screen();
   }
   return windows_from_widget ( screen );
}

void windows_ref ( SQByteArray * _pointer )
{
}

void windows_unref ( SQByteArray * _pointer )
{
}

char * windows_name ( SQByteArray * _pointer )
{
   QWidget * widget = windows_to_widget(_pointer);
   if ( widget == NULL )
   {
      return strdup("");
   }
   else
   {
#if(QT_NO_ACCESSIBILITY)
      return ToString( widget->objectName() );
#else
      return ToString( widget->accessibleName() );
#endif
   }
}

int windows_process_id ( SQByteArray * _pointer )
{
   return QApplication::applicationPid();
}

int windows_x ( SQByteArray * _pointer )
{
   QWidget * widget = windows_to_widget(_pointer);
   if ( widget == NULL )
   {
      return -1;
   }
   else
   {
      return widget->x();
   }
}

int windows_y ( SQByteArray * _pointer )
{
   QWidget * widget = windows_to_widget(_pointer);
   if ( widget == NULL )
   {
      return -1;
   }
   else
   {
      return widget->y();
   }
}

int windows_width ( SQByteArray * _pointer )
{
   QWidget * widget = windows_to_widget(_pointer);
   if ( widget == NULL )
   {
      return -1;
   }
   else
   {
      return widget->width();
   }
}

int windows_height ( SQByteArray * _pointer )
{
   QWidget * widget = windows_to_widget(_pointer);
   if ( widget == NULL )
   {
      return -1;
   }
   else
   {
      return widget->height();
   }
}

char * windows_role ( SQByteArray * _pointer )
{  
   QWidget * widget = windows_to_widget(_pointer);
   if ( widget == NULL )
   {
      return strdup("");     
   }
   else
   {
      return ToString(widget->windowRole());
   }
}

char * windows_text ( SQByteArray * _pointer )
{  
   QWidget * widget = windows_to_widget(_pointer);
   if ( widget == NULL )
   {
      return strdup("");     
   }
   else
   {
      return strdup("");
   }
}

int windows_actions ( SQByteArray * _pointer )
{
   QWidget * widget = windows_to_widget(_pointer);
   if ( widget == NULL )
   {
      return 0;     
   }
   else
   {
      return widget->actions().length();
   }
}

char * windows_action_name ( SQByteArray * _pointer, int _actionIndex )
{
   QWidget * widget = windows_to_widget(_pointer);
   if ( widget != NULL )
   {
      if ( _actionIndex < widget->actions().length() )
      {
         return ToString(widget->actions().at(_actionIndex)->text());
      }
   }
   return strdup("UNKNOWN");
}

void windows_action_do ( SQByteArray * _pointer, int _actionIndex )
{
   QWidget * widget = windows_to_widget(_pointer);
   if ( widget != NULL )
   {
      if ( _actionIndex < widget->actions().length() )
      {
         widget->actions().at(_actionIndex)->trigger();
      }
   }
}

long windows_children ( SQByteArray * _pointer )
{
   QWidget * widget = windows_to_widget(_pointer);
   if ( widget == NULL )
   {
      return 0;
   }
   else
   {
      int ret = 0;
      QObjectList list ( widget->children() );
      for ( int i = 0; i < list.count(); i++ )
      {
         QObject * childObject = list.at ( i );
         if ( childObject->isWidgetType() )
         {
            ret++;
            QWidget * childWidget = qobject_cast<QWidget*>(childObject);
         }
      }
      return ret;
   }
}

SQByteArray * windows_child ( SQByteArray * _parent, long _index )
{
   QWidget * widget = windows_to_widget(_parent);
   if ( widget != NULL )
   {
      int ret = 0;
      QObjectList list ( widget->children() );
      for ( int i = 0; i < list.count(); i++ )
      {
         QObject * childObject = list.at ( i );
         if ( childObject->isWidgetType() )
         {
            ret++;
            if ( ret == _index )
            {
               return windows_from_widget(qobject_cast<QWidget*>(childObject));
            }
         }
      }
   }
   return NULL;
}
}
