#include <sequanto/qtwidgetnode.h>
#include <sequanto/qtwrapper.h>

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
   dynamic_cast<ListNode*>(this->GetParent())->RemoveChild ( GetName() );
}

QtWidgetNode::~QtWidgetNode()
{
   m_widget->removeEventFilter ( m_eventFilter );
   //delete m_eventFilter;
   m_widget = NULL;
}
