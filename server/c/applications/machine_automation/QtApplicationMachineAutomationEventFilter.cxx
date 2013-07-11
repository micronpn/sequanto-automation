#include "QtApplicationMachineAutomationEventFilter.h"
#include "QtMachineAutomationEvent.h"

using namespace sequanto::automation;

QtApplicationMachineAutomationEventFilter::QtApplicationMachineAutomationEventFilter ( QObject * _parent )
   : QObject(_parent)
{
}

QByteArray ToByteArray ( QWidget * _widget )
{
   return QByteArray ( (const char*) &_widget, sizeof(QWidget*) );
}

bool QtApplicationMachineAutomationEventFilter::eventFilter ( QObject * _object, QEvent * _event )
{
   if ( _event->type() == QtMachineAutomationEvent::ID )
   {
      QtMachineAutomationEvent * event = dynamic_cast<QtMachineAutomationEvent*>(_event);
      event->received();

      if ( event->command() == QtMachineAutomationEvent::DESKTOP0 )
      {
         event->done ( ToByteArray(QApplication::desktop()) );
      }
      else
      {
         QWidget * widget = event->widget();  
         if ( widget != NULL )
         {
            switch ( event->command() )
            {
            case QtMachineAutomationEvent::NAME:
#ifdef QT_NO_ACCESSIBILITY
               event->done ( widget->objectName() );
#else
               if ( widget->accessibleName().isEmpty() )
               {
                  event->done ( widget->objectName() );
               }
               else
               {
                  event->done ( widget->accessibleName() );
               }
#endif
               break;

            case QtMachineAutomationEvent::ROLE:
               if ( widget->isWindow() )
               {
                  event->done ( "window" );
               }
               else if ( qobject_cast<QCheckBox*>(widget) != NULL )
               {
                  event->done ( "checkbox" );
               }
               else if ( qobject_cast<QRadioButton*>(widget) != NULL )
               {
                  event->done ( "radiobutton" );
               }
               else if ( qobject_cast<QAbstractButton*>(widget) != NULL )
               {
                  event->done ( "button" );
               }
               else if ( qobject_cast<QAbstractSlider*>(widget) != NULL )
               {
                  event->done ( "slider" );
               }
               else if ( qobject_cast<QComboBox*>(widget) != NULL )
               {
                  event->done ( "combobox" );
               }
               else if ( qobject_cast<QSpinBox*>(widget) != NULL )
               {
                  event->done ( "spinbox" );
               }
               else if ( qobject_cast<QCalendarWidget*>(widget) != NULL )
               {
                  event->done ( "calendar" );
               }
               else if ( qobject_cast<QDialog*>(widget) != NULL )
               {
                  event->done ( "dialog" );
               }
               else if ( qobject_cast<QDialogButtonBox*>(widget) != NULL ||
                  qobject_cast<QGroupBox*>(widget) != NULL )
               {
                  event->done ( "group" );
               }
               else if ( qobject_cast<QLabel*>(widget) != NULL )
               {
                  event->done ( "label" );
               }
               else if ( qobject_cast<QSplitter*>(widget) != NULL )
               {
                  event->done ( "splitter" );
               }
               else if ( qobject_cast<QLineEdit*>(widget) != NULL )
               {
                  event->done ( "text" );
               }
               else if ( qobject_cast<QMdiSubWindow*>(widget) != NULL )
               {
                  event->done ( "window" );
               }
               else if ( qobject_cast<QMenu*>(widget) != NULL )
               {
                  event->done ( "menu" );
               }
               else if ( qobject_cast<QMenuBar*>(widget) != NULL )
               {
                  event->done ( "menubar" );
               }
               else if ( qobject_cast<QProgressBar*>(widget) != NULL )
               {
                  event->done ( "progress" );
               }
               else if ( qobject_cast<QStatusBar*>(widget) != NULL )
               {
                  event->done ( "statusbar" );
               }
               else if ( qobject_cast<QTabBar*>(widget) != NULL )
               {
                  event->done ( "tabs" );
               }
               else if ( qobject_cast<QTabWidget*>(widget) != NULL )
               {
                  event->done ( "tabs" );
               }
               else if ( qobject_cast<QToolBar*>(widget) != NULL )
               {
                  event->done ( "toolbar" );
               }
               /*
               else if ( qobject_cast<*>(widget) != NULL )
               {
               event->done ( "" );
               }
               */
               else
               {
                  event->done ( widget->metaObject()->className() );
               }
               break;

            case QtMachineAutomationEvent::TEXT:
               if ( widget->isWindow() )
               {
                  event->done ( widget->windowTitle() );
               }
               else if ( qobject_cast<QAbstractButton*>(widget) != NULL )
               {
                  event->done( qobject_cast<QAbstractButton*>(widget)->text() );
               }
               else if ( qobject_cast<QAbstractSlider*>(widget) != NULL )
               {
                  event->done ( QString(qobject_cast<QAbstractSlider*>(widget)->value()) );
               }
               else if ( qobject_cast<QComboBox*>(widget) != NULL )
               {
                  event->done ( qobject_cast<QComboBox*>(widget)->currentText() );
               }
               else if ( qobject_cast<QSpinBox*>(widget) != NULL )
               {
                  event->done ( QString(qobject_cast<QSpinBox*>(widget)->value()) );
               }
               else if ( qobject_cast<QCalendarWidget*>(widget) != NULL )
               {
                  event->done ( qobject_cast<QCalendarWidget*>(widget)->selectedDate().toString() );  
               }
               else if ( qobject_cast<QDialog*>(widget) != NULL )
               {
                  event->done( qobject_cast<QDialog*>(widget)->windowTitle() );
               }
               else if ( qobject_cast<QGroupBox*>(widget) != NULL )
               {
                  event->done ( qobject_cast<QGroupBox*>(widget)->title() );
               }
               else if ( qobject_cast<QLabel*>(widget) != NULL )
               {
                  event->done ( qobject_cast<QLabel*>(widget)->text() );
               }
               else if ( qobject_cast<QSplitter*>(widget) != NULL )
               {
                  event->done ( QString(qobject_cast<QSplitter*>(widget)->orientation()) );
               }
               else if ( qobject_cast<QLineEdit*>(widget) != NULL )
               {
                  event->done ( qobject_cast<QLineEdit*>(widget)->text() );
               }
               else if ( qobject_cast<QMdiSubWindow*>(widget) != NULL )
               {
                  event->done( qobject_cast<QMdiSubWindow*>(widget)->windowTitle() );
               }
               else if ( qobject_cast<QMenu*>(widget) != NULL )
               {
                  event->done ( qobject_cast<QMenu*>(widget)->title() );
               }
               else if ( qobject_cast<QProgressBar*>(widget) != NULL )
               {
                  QProgressBar * progressBar = qobject_cast<QProgressBar*>(widget);
                  event->done ( progressBar->text() + progressBar->value() );
               }
               else if ( qobject_cast<QStatusBar*>(widget) != NULL )
               {
                  event->done ( qobject_cast<QStatusBar*>(widget)->currentMessage() );
               }
               else if ( qobject_cast<QTabBar*>(widget) != NULL )
               {
                  event->done ( QString(qobject_cast<QTabBar*>(widget)->count()) );
               }
               else if ( qobject_cast<QTabWidget*>(widget) != NULL )
               {
                  event->done ( QString(qobject_cast<QTabWidget*>(widget)->count()) );
               }
               else
               {
                  event->done ( "" );
               }
               break;

            case QtMachineAutomationEvent::X:
            case QtMachineAutomationEvent::Y:
               {
                  QPoint pos = widget->mapToGlobal(QPoint(0,0));
                  /*
                  QWidget * window = widget->window();
                  QPoint windowTopLeft ( window->geometry().topLeft() );
                  pos -= windowTopLeft;
                  */

                  if ( event->command() == QtMachineAutomationEvent::X )
                  {
                     event->done ( pos.x() );
                  }
                  else
                  {
                     event->done ( pos.y() );
                  }
               }
               break;

            case QtMachineAutomationEvent::WIDTH:
               event->done ( widget->width() );
               break;

            case QtMachineAutomationEvent::HEIGHT:
               event->done ( widget->height() );
               break;

            case QtMachineAutomationEvent::CHILD:
            case QtMachineAutomationEvent::CHILDREN:
               {
                  QWidgetList list;
                  if ( widget == QApplication::desktop() )
                  {
                     list = QApplication::topLevelWidgets();
                  }
                  else
                  {
                     for ( QObjectList::const_iterator it = widget->children().constBegin(); it != widget->children().constEnd(); it++ )
                     {
                        if ( (*it)->isWidgetType() )
                        {
                           list.append ( qobject_cast<QWidget*>(*it) );
                        }
                     }
                  }
                  if ( event->command() == QtMachineAutomationEvent::CHILDREN)
                  {
                     event->done(list.count());
                  }
                  else
                  {
                     int index = event->index();
                     for ( int i = 0; i < list.count(); i++ )
                     {
                        if ( i == index )
                        {
                           event->done ( ToByteArray(list.at(i)) );
                           index = -1;
                        }
                     }
                     if ( index == -1 )
                     {
                        event->done ( QVariant() );
                     }
                  }
                  break;
               }
            case QtMachineAutomationEvent::ACTIONS:
               event->done ( widget->actions().length() );
               break;

            case QtMachineAutomationEvent::ACTION_NAME:
               if ( event->index() < widget->actions().length() )
               {
                  event->done ( widget->actions().at(event->index())->text());
               }
               else
               {
                  event->done ( QVariant() );
               }
               break;

            case QtMachineAutomationEvent::ACTION_DO:
               if ( event->index() < widget->actions().length() )
               {
                  widget->actions().at(event->index())->trigger();
               }
               event->done ( QVariant() );
               break;
               
            default:
                event->done ( QVariant() );
                break;
            }
         }
         else
         {
            event->done ( QVariant() );
         }
      }
      return true;
   }
   else if ( _event->type() == QtMachineAutomationMouseEvent::ID )
   {
      QtMachineAutomationMouseEvent * event = dynamic_cast<QtMachineAutomationMouseEvent*>(_event);
      QWidget * receiver = QApplication::widgetAt(event->position());
      if ( receiver != NULL )
      {
         QPoint widgetPos = receiver->mapFromGlobal ( event->position() );
         
         QApplication::postEvent ( receiver, new QMouseEvent( QEvent::MouseButtonPress, widgetPos, event->position(), event->button(), event->button(), Qt::NoModifier ) );
         QApplication::postEvent ( receiver, new QMouseEvent( QEvent::MouseButtonRelease, widgetPos, event->position(), event->button(), event->button(), Qt::NoModifier ) );
      }
      return true;
   }
   return QObject::eventFilter(_object, _event );
}

QtApplicationMachineAutomationEventFilter::~QtApplicationMachineAutomationEventFilter ()
{
}
