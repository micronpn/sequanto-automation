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
      m_activeWindowNode ( _activeWindowNode )
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
            m_activeWindowNode->SendUpdate ( value );
            sq_value_free ( &value );
        }
        break;

    case QEvent::Create:
        if ( _object->isWidgetType() )
        {
            QWidget * widget = qobject_cast<QWidget*> ( _object );
            if ( widget->isWindow() )
            {
                QtWrapper::UpdateWindows ( m_windowsNode );
                m_windowsNode->SendUpdate();
            }
        }
        break;

    case QEvent::Destroy:
        if ( _object->isWidgetType() )
        {
            QWidget * widget = qobject_cast<QWidget*> ( _object );
            if ( widget->isWindow() )
            {
                QtWrapper::UpdateWindows ( m_windowsNode );
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
