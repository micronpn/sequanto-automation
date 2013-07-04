#include <sequanto/automation.h>
#include <memory.h>
#include <QtGui>
#include "QtApplicationMachineAutomationEventFilter.h"
#include "QtMachineAutomationEvent.h"

using namespace sequanto::automation;

bool initialized = false;
QWidget * screen = NULL;
QDesktopWidget * desktop = NULL;

char * ToString ( const QString & _string )
{
   QByteArray value ( _string.toUtf8() );
   return strdup(value.data());
}

SQByteArray * ToByteArray ( const QByteArray & _byteArray )
{
   //assert ( _byteArray.length() == sizeof(QWidget*) );

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

QWidget * windows_to_widget ( const SQByteArray * _pointer )
{
   QWidget * widget = NULL;
   memcpy ( &widget, _pointer->m_start, sizeof(QWidget*) );
   return widget;
}

extern "C"
{

int windows_desktops ()
{
   return 1;
}

SQByteArray * windows_desktop ( int _desktop )
{
   QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::DESKTOP0, NULL, -1);
   QVariant value = event->wait(QApplication::instance());
   
   return ToByteArray (value.toByteArray() );
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
   QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::NAME, widget, -1);
   QVariant value = event->wait(QApplication::instance());
   
   if ( value.isNull() )
   {
      return strdup("");
   }
   else
   {
      return ToString(value.toString());
   }
}

int windows_process_id ( SQByteArray * _pointer )
{
   return QApplication::applicationPid();
}

int windows_x ( SQByteArray * _pointer )
{
   QWidget * widget = windows_to_widget(_pointer);
   QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::X, widget, -1);
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
   QWidget * widget = windows_to_widget(_pointer);
   QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::Y, widget, -1);
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
   QWidget * widget = windows_to_widget(_pointer);
   QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::WIDTH, widget, -1);
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
   QWidget * widget = windows_to_widget(_pointer);
   QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::HEIGHT, widget, -1);
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
   QWidget * widget = windows_to_widget(_pointer);
   QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::ROLE, widget, -1);
   QVariant value = event->wait(QApplication::instance());
   
   if ( value.isNull() )
   {
      return strdup("");
   }
   else
   {
      return ToString(value.toString());
   }
}

char * windows_text ( SQByteArray * _pointer )
{  
   QWidget * widget = windows_to_widget(_pointer);
   QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::TEXT, widget, -1);
   QVariant value = event->wait(QApplication::instance());
   
   if ( value.isNull() )
   {
      return strdup("");
   }
   else
   {
      return ToString(value.toString());
   }
}

int windows_actions ( SQByteArray * _pointer )
{
   QWidget * widget = windows_to_widget(_pointer);
   QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::ACTIONS, widget, -1);
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
   QWidget * widget = windows_to_widget(_pointer);
   QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::ACTION_NAME, widget, _actionIndex);
   QVariant value = event->wait(QApplication::instance());
   
   if ( value.isNull() )
   {
      return strdup("");;
   }
   else
   {
      return ToString(value.toString());
   }
}

void windows_action_do ( SQByteArray * _pointer, int _actionIndex )
{
   QWidget * widget = windows_to_widget(_pointer);
   QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::ACTION_DO, widget, _actionIndex);
   event->wait(QApplication::instance());
}

long windows_children ( SQByteArray * _pointer )
{
   QWidget * widget = windows_to_widget(_pointer);
   QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::CHILDREN, widget, -1);
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
   QWidget * widget = windows_to_widget(_parent);
   QtMachineAutomationEvent * event = new QtMachineAutomationEvent(QtMachineAutomationEvent::CHILD, widget, _index);
   QVariant value = event->wait(QApplication::instance());
   
   if ( value.isNull() )
   {
      return NULL;
   }
   else
   {
      return ToByteArray (value.toByteArray() );
   }
}
}
