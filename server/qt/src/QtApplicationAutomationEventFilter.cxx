#include <sequanto/QtApplicationAutomationEventFilter.h>
#include <sequanto/QtAutomationGetPropertyEvent.h>
#include <sequanto/QtAutomationMoveEvent.h>
#include <sequanto/QtAutomationResizeEvent.h>
#include <sequanto/QtWidgetNode.h>
#include <sequanto/ui.h>
#include <cassert>

#include <sequanto/QtWrapper.h>

using namespace sequanto::automation;

QtApplicationAutomationEventFilter::QtApplicationAutomationEventFilter ( ListNode * _windowsNode, PropertyNode * _activeWindowNode, QObject * _parent )
    : QObject(_parent),
      m_windowsNode ( _windowsNode ),
      m_activeWindowNode ( _activeWindowNode ),
      m_previousActiveWindow ( "<NULL>" )
{
}

bool QtApplicationAutomationEventFilter::eventFilter ( QObject * _object, QEvent * _event )
{
    switch ( _event->type() )
    {
    case QEvent::WindowActivate:
        {
            SQValue value;
            sq_value_init ( &value );
            m_activeWindowNode->HandleGet ( value );
            if ( m_previousActiveWindow != value.Value.m_stringValue )
            {
               m_previousActiveWindow = value.Value.m_stringValue;
               m_activeWindowNode->SendUpdate ( value );
            }
            sq_value_free ( &value );
        }
        break;

    case QEvent::Create:
       {
          QWidget * widget = qobject_cast<QWidget*> ( _object );
          if ( widget->isWindow() )
          {
             if ( QtWrapper::UpdateWindows ( m_windowsNode ) )
             {
                m_windowsNode->SendUpdate();
             }
          }
       }
       break;

    case QEvent::Close:
       QWidget * widget = qobject_cast<QWidget*> ( _object );
       if ( widget->isWindow() )
       {
          std::string name ( QtWrapper::GetObjectName(widget) );
          if ( m_windowsNode->HasChild(name ) )
          {
             m_windowsNode->RemoveChild ( name );
             m_windowsNode->SendUpdate();
          }
       }
       break;
    }
    return QObject::eventFilter(_object, _event );
}

QtApplicationAutomationEventFilter::~QtApplicationAutomationEventFilter ()
{
}
