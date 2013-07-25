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

static QRect get_qtablewidget_rect ( const QTableWidget * tableWidget, const QByteArray & extra )
{
   const QTableWidgetItem * item = NULL;
   switch ( extra.length() )
   {
   case 1:
      item = tableWidget->item(extra[0], 0);
      break;

   case 2:
      item = tableWidget->item(extra[0], extra[1]);
      break;

   default:
      break;
   }
   if ( item != NULL )
   {
      QRect rect = tableWidget->visualItemRect(item);
      if ( extra.length() == 1 )
      {
         rect.setWidth ( tableWidget->width() );
      }
      return rect;
   }
   else
   {
      return QRect();
   }
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
               if ( event->normal() )
               {
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
               }
               else
               {
                  QByteArray extra = event->extra();
                  switch ( extra.length() )
                  {
                  case 1:
                     event->done ( QString("Row%1").arg((int)extra[0]) );
                     break;

                  case 2:
                     event->done ( QString("Column%1").arg((int)extra[1]) );
                     break;

                  default:
                     event->done ( QString("ERROR: Bad numner of extra indices: %1").arg(extra.length()) );
                     break;
                  }
               }
               break;

            case QtMachineAutomationEvent::ROLE:
               if ( event->normal() )
               {
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
                  else if ( qobject_cast<QTableView*>(widget) != NULL )
                  {
                     event->done("table");
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
               }
               else
               {
                  if ( event->extra().length() == 1 )
                  {
                     event->done("row");
                  }
                  else
                  {
                     event->done("cell");
                  }
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
               else if ( qobject_cast<QTableWidget*>(widget) != NULL )
               {
                  QTableWidget * table = qobject_cast<QTableWidget*>(widget);
                  if ( event->normal() )
                  {
                     event->done ( QString("%1x%2").arg( table->rowCount() ).arg ( table->columnCount() ) );
                  }
                  else
                  {
                     QByteArray extra ( event->extra() );
                     if ( extra.length() == 2 )
                     {
                        QTableWidgetItem * item = table->item(extra[0], extra[1]);
                        if ( item == NULL )
                        {
                           event->done(QString());
                        }
                        else
                        {
                           event->done ( item->text() );
                        }
                     }
                     else
                     {
                        event->done ( QString() );
                     }
                  }
               }
               else
               {
                  event->done ( "" );
               }
               break;

            case QtMachineAutomationEvent::X:
            case QtMachineAutomationEvent::Y:
               {
                  QPoint pos ( -1, -1 );
                  if ( event->normal() )
                  {
                     pos = widget->mapToGlobal(QPoint(0,0));
                  }
                  else if ( qobject_cast<QTableWidget*>(widget) != NULL )
                  {
                     QTableWidget * tableWidget = qobject_cast<QTableWidget*>(widget);
                     QByteArray extra = event->extra();
                     QRect rect = get_qtablewidget_rect(tableWidget, extra);
                     if ( !rect.isNull() )
                     {
                        pos = rect.topLeft();
                     }
                  }
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
               if ( event->normal() )
               {
                  event->done ( widget->width() );
               }
               else if ( qobject_cast<QTableWidget*>(widget) != NULL )
               {
                  QTableWidget * tableWidget = qobject_cast<QTableWidget*>(widget);
                  QByteArray extra = event->extra();
                  QRect rect = get_qtablewidget_rect(tableWidget, extra);
                  event->done( rect.width() );
               }
               break;

            case QtMachineAutomationEvent::HEIGHT:
               if ( event->normal() )
               {
                  event->done ( widget->height() );
               }
               else if ( qobject_cast<QTableWidget*>(widget) != NULL )
               {
                  QTableWidget * tableWidget = qobject_cast<QTableWidget*>(widget);
                  QByteArray extra = event->extra();
                  QRect rect = get_qtablewidget_rect(tableWidget, extra);
                  event->done( rect.height() );
               }
               break;

            case QtMachineAutomationEvent::CHILD:
            case QtMachineAutomationEvent::CHILDREN:
               {
                  if ( qobject_cast<QTableWidget*>(widget) != NULL )
                  {
                     QTableWidget* tableWidget = qobject_cast<QTableWidget*>(widget);
                     if ( event->command() == QtMachineAutomationEvent::CHILDREN )
                     {
                        QByteArray extra ( event->extra() );
                        switch ( extra.length() )
                        {
                        case 0:
                           event->done(tableWidget->rowCount());
                           break;

                        case 1:
                           event->done(tableWidget->columnCount());
                           break;

                        default:
                           event->done(0);
                           break;
                        }
                     }
                     else
                     {
                        QByteArray obj ( event->object() );
                        obj.append ( (char) event->index() );
                        event->done ( obj );
                     }
                  }
                  else
                  {
                     QWidgetList list;
                     if ( widget == QApplication::desktop() )
                     {
                        QWidgetList topLevel ( QApplication::topLevelWidgets() );
                        for ( QWidgetList::const_iterator it = topLevel.constBegin(); it != topLevel.constEnd(); it++ )
                        {
                           if ( (*it)->isVisible() )
                           {
                              list.append ( *it );
                           }
                        }
                     }
                     else
                     {
                        for ( QObjectList::const_iterator it = widget->children().constBegin(); it != widget->children().constEnd(); it++ )
                        {
                           if ( (*it)->isWidgetType() )
                           {
                              QWidget * childWidget = qobject_cast<QWidget*>(*it);
                              if ( childWidget->isVisible() )
                              {
                                 list.append ( childWidget );
                              }
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
                        if ( index != -1 )
                        {
                           event->done ( QVariant() );
                        }
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
