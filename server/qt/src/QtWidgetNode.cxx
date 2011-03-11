#include <sequanto/QtWidgetNode.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/QtUnnamedObjectStore.h>
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

void QtWidgetNode::SendChildrenUpdate ()
{
   ListNode * childrenNode = dynamic_cast<ListNode*>(this->FindChild ( SQ_UI_NODE_CHILDREN ) );
   if ( childrenNode != NULL )
   {
      childrenNode->SendUpdate ();
   }
}

bool QtWidgetNode::AddChildWidget ( QWidget * _child )
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
            return true;
         }
      }
   }
   return false;
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
