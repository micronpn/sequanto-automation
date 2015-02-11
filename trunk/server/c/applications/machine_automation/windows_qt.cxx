#include <sequanto/automation.h>
#include <memory.h>
#include <QtGui>
#include "QtApplicationMachineAutomationEventFilter.h"
#include "QtMachineAutomationEvent.h"
#include "config.h"

using namespace sequanto::automation;

bool initialized = false;
QWidget * screen = NULL;
QDesktopWidget * desktop = NULL;

char * ToString ( const QString & _string )
{
   QByteArray value ( _string.toUtf8() );
   return SQ_STRDUP_FUNCTION(value.data());
}

SQByteArray * ToByteArray ( const QByteArray & _byteArray )
{
   // assert ( _byteArray.length() == sizeof(QWidget*) );

   SQByteArray * ret = sq_byte_array_create_prealloc ( _byteArray.length() );
   memcpy ( ret->m_start, _byteArray.constData(), _byteArray.length() );
   return ret;
}

extern "C" void windows_init_if_not_already ()
{
   if ( !initialized )
   {
      QApplication::instance()->installEventFilter ( new QtApplicationMachineAutomationEventFilter(QApplication::instance()) );

      initialized = true;
   }
}

QByteArray FromByteArray ( const SQByteArray * _pointer )
{
   QByteArray ret;
   ret.setRawData ( (const char*) _pointer->m_start, _pointer->m_length );
   return ret;
}

extern "C"
{

   int windows_desktops ()
   {
      return 1;
   }

   SQByteArray * windows_desktop ( int _desktop )
   {
      SQ_UNUSED_PARAMETER(_desktop);

      QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::DESKTOP0, NULL, -1);
      QVariant value = event->wait(QApplication::instance());

      return ToByteArray (value.toByteArray() );
   }

   void windows_ref ( SQByteArray * _pointer )
   {
      SQ_UNUSED_PARAMETER(_pointer);
   }

   void windows_unref ( SQByteArray * _pointer )
   {
      SQ_UNUSED_PARAMETER(_pointer);
   }

   char * windows_name ( SQByteArray * _pointer )
   {
      QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::NAME, _pointer, -1);
      QVariant value = event->wait(QApplication::instance());

      if ( value.isNull() )
      {
         return SQ_STRDUP_FUNCTION("");
      }
      else
      {
         return ToString(value.toString());
      }
   }

   int windows_process_id ( SQByteArray * _pointer )
   {
      SQ_UNUSED_PARAMETER(_pointer);

      return QApplication::applicationPid();
   }

   int windows_x ( SQByteArray * _pointer )
   {
      QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::X, _pointer, -1);
      QVariant value = event->wait(QApplication::instance());

      if ( value.isNull() )
      {
         return -1;
      }
      else
      {
         return value.toInt();
      }
   }

   int windows_y ( SQByteArray * _pointer )
   {
      QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::Y, _pointer, -1);
      QVariant value = event->wait(QApplication::instance());

      if ( value.isNull() )
      {
         return -1;
      }
      else
      {
         return value.toInt();
      }
   }

   int windows_width ( SQByteArray * _pointer )
   {
      QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::WIDTH, _pointer, -1);
      QVariant value = event->wait(QApplication::instance());

      if ( value.isNull() )
      {
         return -1;
      }
      else
      {
         return value.toInt();
      }
   }

   int windows_height ( SQByteArray * _pointer )
   {
      QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::HEIGHT, _pointer, -1);
      QVariant value = event->wait(QApplication::instance());

      if ( value.isNull() )
      {
         return -1;
      }
      else
      {
         return value.toInt();
      }
   }

   char * windows_role ( SQByteArray * _pointer )
   {
      QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::ROLE, _pointer, -1);
      QVariant value = event->wait(QApplication::instance());

      if ( value.isNull() )
      {
         return SQ_STRDUP_FUNCTION("");
      }
      else
      {
         return ToString(value.toString());
      }
   }

   char * windows_text ( SQByteArray * _pointer )
   {
      QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::TEXT, _pointer, -1);
      QVariant value = event->wait(QApplication::instance());

      if ( value.isNull() )
      {
         return SQ_STRDUP_FUNCTION("");
      }
      else
      {
         return ToString(value.toString());
      }
   }

   int windows_actions ( SQByteArray * _pointer )
   {
      QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::ACTIONS, _pointer, -1);
      QVariant value = event->wait(QApplication::instance());

      if ( value.isNull() )
      {
         return -1;
      }
      else
      {
         return value.toInt();
      }
   }

   char * windows_action_name ( SQByteArray * _pointer, int _actionIndex )
   {
      QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::ACTION_NAME, _pointer, _actionIndex);
      QVariant value = event->wait(QApplication::instance());

      if ( value.isNull() )
      {
         return SQ_STRDUP_FUNCTION("");;
      }
      else
      {
         return ToString(value.toString());
      }
   }

   void windows_action_do ( SQByteArray * _pointer, int _actionIndex )
   {
      QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::ACTION_DO, _pointer, _actionIndex);
      event->wait(QApplication::instance());
   }

   long windows_children ( SQByteArray * _pointer )
   {
      QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::CHILDREN, _pointer, -1);
      QVariant value = event->wait(QApplication::instance());

      if ( value.isNull() )
      {
         return 0;
      }
      else
      {
         return value.toInt();
      }
   }

   SQByteArray * windows_child ( SQByteArray * _parent, long _index )
   {
      QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::CHILD, _parent, _index);
      QVariant value = event->wait(QApplication::instance());

      if ( value.isNull() )
      {
         return sq_byte_array_create_prealloc(0);
      }
      else
      {
         return ToByteArray ( value.toByteArray() );
      }
   }

   void mouse_click_at ( int _x, int _y, int _button )
   {
      QtMachineAutomationMouseEvent * event = new QtMachineAutomationMouseEvent(QtMachineAutomationMouseEvent::CLICK_AT, _x, _y, _button);
      QApplication::postEvent ( QApplication::instance(), event );
   }

   void mouse_hold_at ( int _x, int _y, int _button )
   {
      QtMachineAutomationMouseEvent * event = new QtMachineAutomationMouseEvent(QtMachineAutomationMouseEvent::HOLD_AT, _x, _y, _button);
      QApplication::postEvent ( QApplication::instance(), event );
   }

   void mouse_release_at ( int _x, int _y, int _button )
   {
      QtMachineAutomationMouseEvent * event = new QtMachineAutomationMouseEvent(QtMachineAutomationMouseEvent::RELEASE_AT, _x, _y, _button);
      QApplication::postEvent ( QApplication::instance(), event );
   }

   SQByteArray * windows_capture_screen ( int _desktop )
   {
      QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::CAPTURE_SCREEN, NULL, _desktop);
      QVariant value = event->wait(QApplication::instance());

      if ( value.isNull() )
      {
         return sq_byte_array_create_prealloc(0);
      }
      else
      {
         return ToByteArray ( value.toByteArray() );
      }
   }
}
