#include <sequanto/QtWidgetNode.h>
#include <sequanto/QtWrapper.h>
#include <cassert>

using namespace sequanto::automation;

QtWidgetNode::QtWidgetNode( QWidget * _widget )
    : ListNode(QtWrapper::GetObjectName(_widget) ),
      m_widget(_widget)
{
   m_eventFilter = new QtWidgetAutomationEventFilter(this);
   m_widget->installEventFilter( m_eventFilter );
}

void QtWidgetNode::WidgetDestroyed()
{
   // If m_widget is NULL we are in the process of being destroyed (~QtWidgetNode is in progress).
   if ( m_widget != NULL )
   {
      m_widget = NULL;
      dynamic_cast<ListNode*>(this->GetParent())->RemoveChild ( GetName() );
   }
}

QtWidgetNode::~QtWidgetNode()
{
   // If m_widget is NULL our widget has already been destroyed and we do not need to do anything to un-register ourselves from the widget.
   if ( m_widget != NULL )
   {
      QWidget * widget = m_widget;
      m_widget = NULL;

      widget->removeEventFilter ( m_eventFilter );
      delete m_eventFilter;
   }
}
