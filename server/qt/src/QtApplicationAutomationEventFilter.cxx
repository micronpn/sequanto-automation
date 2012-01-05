#include <sequanto/QtApplicationAutomationEventFilter.h>
#include <sequanto/QtAutomationGetPropertyEvent.h>
#include <sequanto/QtAutomationMoveEvent.h>
#include <sequanto/QtAutomationResizeEvent.h>
#include <sequanto/QtAutomationWidgetCreatedEvent.h>
#include <sequanto/QtAutomationStealFocusEvent.h>
#include <sequanto/QtWidgetNode.h>
#include <sequanto/QtActiveWindowProperty.h>
#include <sequanto/QtAutomationMouseClickEvent.h>
#include <sequanto/QtLogging.h>
#include <sequanto/QtDebugging.h>
#include <sequanto/ui.h>
#include <cassert>

#include <sequanto/QtWrapper.h>

using namespace sequanto::automation;

QtApplicationAutomationEventFilter::QtApplicationAutomationEventFilter ( ListNode * _windowsNode, QtActiveWindowProperty * _activeWindowNode, QObject * _parent )
    : QObject(_parent),
      m_windowsNode ( _windowsNode ),
      m_activeWindowNode ( _activeWindowNode )
{
}

bool QtApplicationAutomationEventFilter::eventFilter ( QObject * _object, QEvent * _event )
{
    switch ( _event->type() )
    {
    case QEvent::WindowActivate:
		m_activeWindowNode->TrySendUpdate ();
        break;

	case QEvent::WindowDeactivate:
		m_activeWindowNode->TrySendUpdate ();
		break;

    case QEvent::Create:
       {
          QWidget * widget = qobject_cast<QWidget*> ( _object );
          if ( widget != NULL )
          {
             QApplication::postEvent ( this, new QtAutomationWidgetCreatedEvent(widget) );
          }
       }
       break;

    case QEvent::Close:
       {
          QWidget * widget = qobject_cast<QWidget*> ( _object );
          if ( QtWrapper::IsWindow(widget) )
          {
             std::string name ( QtWrapper::GetObjectName(widget) );
             if ( m_windowsNode->HasChild(name ) )
             {
                m_windowsNode->RemoveChild ( name );
                m_windowsNode->SendUpdate();
             }
          }
       }
       break;

    default:
       if ( _event->type() == QtAutomationWidgetCreatedEvent::ID )
       {
          QtAutomationWidgetCreatedEvent * event = dynamic_cast<QtAutomationWidgetCreatedEvent*>(_event);
          if ( event->widget() != NULL )
          {
             if ( QtWrapper::IsWindow(event->widget()) )
             {
                if ( QtWrapper::UpdateWindows ( m_windowsNode, m_activeWindowNode ) )
                {
                   m_windowsNode->SendUpdate();
                }
             }
          }
          return true;
       }
       else if ( _event->type() == QtAutomationMouseClickEvent::ID )
       {
          dynamic_cast<QtAutomationMouseClickEvent*>(_event)->done();
       }
       else if ( _event->type() == QtDebuggingEvent::ID )
       {
           dynamic_cast<QtDebuggingEvent*>(_event)->Handle();
           return true;
       }
       else if ( _event->type() == QtAutomationStealFocusEvent::ID )
       {
          QtAutomationStealFocusEvent * focusEvent = dynamic_cast<QtAutomationStealFocusEvent*>(_event);
          focusEvent->stealFocus();
          focusEvent->done();
          return true;
       }
       break;
    }
    return QObject::eventFilter(_object, _event );
}

QtApplicationAutomationEventFilter::~QtApplicationAutomationEventFilter ()
{
}
