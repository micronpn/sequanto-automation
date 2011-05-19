#include <sequanto/QtWidgetNode.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/QtUnnamedObjectStore.h>
#include <sequanto/QtUiTypeProperty.h>
#include <sequanto/ui.h>
#include <cassert>

using namespace sequanto::automation;

QtWidgetNode::QtWidgetNode( QWidget * _widget )
    : ListNode(QtWrapper::GetObjectName(_widget) ),
      m_widget(_widget)
{
   m_eventFilter = new QtWidgetAutomationEventFilter(this);
   m_widget->installEventFilter( m_eventFilter );
}

SQWidgetType QtWidgetNode::type () const
{
   QtUiTypeProperty * typeProperty = dynamic_cast<QtUiTypeProperty*>(FindChild(SQ_UI_NODE_TYPE));
   if ( typeProperty != NULL )
   {
      return typeProperty->type();
   }
   else
   {
      return SQ_WIDGET_TYPE_UNKNOWN;
   }
}

void QtWidgetNode::SendChildrenUpdate ()
{
   ListNode * childrenNode = dynamic_cast<ListNode*>(this->FindChild ( SQ_UI_NODE_CHILDREN ) );
   if ( childrenNode != NULL )
   {
      childrenNode->SendUpdate ();
   }
}

void QtWidgetNode::SendPositionUpdateForAllChildren()
{
   PropertyNode * xNode = dynamic_cast<PropertyNode*>( this->FindChild ( SQ_UI_NODE_X ) );
   if( xNode != NULL )
   {
	   xNode->SendUpdate();
   }
   
   PropertyNode * yNode = dynamic_cast<PropertyNode*>( this->FindChild ( SQ_UI_NODE_Y ) );
   if( yNode != NULL )
   {
	   yNode->SendUpdate();
   }
   
   ListNode * childrenNode = dynamic_cast<ListNode*>( this->FindChild ( SQ_UI_NODE_CHILDREN ) );
   if ( childrenNode != NULL )
   {
	   ListNode::Iterator * it = childrenNode->ListChildren();
	   for ( ; it->HasNext(); it->Next() )
	   {
		   QtWidgetNode * node = dynamic_cast<QtWidgetNode*>(it->GetCurrent());
		   if ( node != NULL )
		   {
			   node->SendPositionUpdateForAllChildren();
		   }
	   }
	   delete it;
   }
}

QtWidgetNode::AddChildWidgetResult QtWidgetNode::AddChildWidget ( QWidget * _child )
{
   ListNode * childrenNode = dynamic_cast<ListNode*>( this->FindChild ( SQ_UI_NODE_CHILDREN ) );
   if ( childrenNode != NULL )
   {
      std::string childName ( QtWrapper::GetObjectName(_child) );
      if ( !childName.empty() )
      {
         if ( !childrenNode->HasChild(childName) )
         {
            if ( QtUnnamedObjectStore::IsKnown ( _child ) )
            {
               std::string unnamedObjectName ( QtUnnamedObjectStore::GetName(_child) );
               if ( childrenNode->HasChild(unnamedObjectName) )
               {
                  childrenNode->RemoveChild ( unnamedObjectName );
                  QtUnnamedObjectStore::Deleted ( _child );
               }
            }
            QtWidgetNode * child = new QtWidgetNode ( _child );
            QtWrapper::WrapUi ( child, qobject_cast<QWidget*>(_child) );
            childrenNode->AddChild ( child );
            return QtWidgetNode::ADDED;
         }
		 else
		 {
			 QtWrapper::Log ( QString("ERROR: Tried to add the child %1 to the widget %2 but it already contains a child with the given name.").arg(childName.c_str()).arg(GetFullName().c_str()) );
          return QtWidgetNode::ALREADY_EXISTS;
		 }
      }
   }
   return QtWidgetNode::NOT_ADDED;
}

/*
bool QtWidgetNode::RemoveChildWidget ( QWidget * _child )
{
   ListNode * childrenNode = dynamic_cast<ListNode*>( this->FindChild ( SQ_UI_NODE_CHILDREN ) );
   if ( childrenNode != NULL )
   {
      std::string childName ( QtWrapper::GetObjectName(_child) );
      if ( !childrenNode->HasChild(childName) )
      {
         childrenNode->RemoveChild ( childName );
         return true;
      }
   }
   return false;
}
*/

void QtWidgetNode::HandleGetNativeId ( SQValue & _value )
{
   sq_value_const_byte_array ( &_value, (SQByte*) (&m_widget), sizeof(QWidget*) );
}

void QtWidgetNode::WidgetDestroyed()
{
   // If m_widget is NULL we are in the process of being destroyed (~QtWidgetNode is in progress).
   if ( m_widget != NULL )
   {
      QtUnnamedObjectStore::Deleted ( m_widget );
      m_widget = NULL;
      ListNode * parent = dynamic_cast<ListNode*>(this->GetParent());
      if ( parent != NULL )
      {
         parent->RemoveChild ( GetName() );
         parent->SendUpdate ();
      }
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

      QtUnnamedObjectStore::Deleted ( widget );
   }
}
