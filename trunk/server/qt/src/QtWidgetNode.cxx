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
   QtUiTypeProperty * typeProperty = dynamic_cast<QtUiTypeProperty*>(FindChild(SQ_UI_WIDGET_TYPE));
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
   ListNode * childrenNode = dynamic_cast<ListNode*>(this->FindChild ( SQ_UI_COMMON_BASE_CHILDREN ) );
   if ( childrenNode != NULL )
   {
      childrenNode->SendUpdate ();
   }
}

void QtWidgetNode::SendPositionUpdateForAllChildren()
{
   PropertyNode * xNode = dynamic_cast<PropertyNode*>( this->FindChild ( SQ_UI_WIDGET_X ) );
   if( xNode != NULL )
   {
      xNode->SendUpdate();
   }
   
   PropertyNode * yNode = dynamic_cast<PropertyNode*>( this->FindChild ( SQ_UI_WIDGET_Y ) );
   if( yNode != NULL )
   {
      yNode->SendUpdate();
   }
   
   ListNode * childrenNode = dynamic_cast<ListNode*>( this->FindChild ( SQ_UI_COMMON_BASE_CHILDREN ) );
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

void QtWidgetNode::SendUpdateForAllChildren()
{
   qDebug() << "SendUpdateForAllChildren " << this->GetFullName().c_str();
   
   ListNode::Iterator * it = ListChildren();
   for ( ; it->HasNext(); it->Next() )
   {
       PropertyNode * node = dynamic_cast<PropertyNode*>( it->GetCurrent() );
       if ( node != NULL )
       {
           node->SendUpdate();
       }
       else if ( it->GetCurrent()->GetName() == SQ_UI_COMMON_BASE_CHILDREN )
       {
           ListNode * childrenNode = dynamic_cast<ListNode*>( it->GetCurrent() );
           if ( childrenNode != NULL )
           {
               ListNode::Iterator * childIt = childrenNode->ListChildren();
               for ( ; childIt->HasNext(); childIt->Next() )
               {
                   QtWidgetNode * node = dynamic_cast<QtWidgetNode*>(childIt->GetCurrent());
                   if ( node != NULL )
                   {
                       node->SendUpdateForAllChildren();
                   }
               }
               delete childIt;
               childrenNode->SendUpdate();
           }
       }
   }
   delete it;
   it = NULL;
}

void QtWidgetNode::SendUpdateForAllImmediateChildren ()
{
   ListNode::Iterator * it = ListChildren();
   for ( ; it->HasNext(); it->Next() )
   {
      PropertyNode * propertyNode = dynamic_cast<PropertyNode*>(it->GetCurrent());
      if ( propertyNode != NULL )
      {
         propertyNode->SendUpdate();
      }
      else
      {
         ListNode * listNode = dynamic_cast<ListNode*>(it->GetCurrent());
         if ( listNode != NULL )
         {
            listNode->SendUpdate();
         }
      }
   }
   delete it;
}

QtWidgetNode::AddChildWidgetResult QtWidgetNode::AddChildWidget ( QWidget * _child, bool _sendAdd )
{
   ListNode * childrenNode = dynamic_cast<ListNode*>( this->FindChild ( SQ_UI_COMMON_BASE_CHILDREN ) );
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
            if ( _sendAdd )
            {
               child->SendAdd ();
            }
            return QtWidgetNode::ADDED;
         }
         else
         {
            QtWrapper::Log ( QString("WARN: Tried to add the child %1 to the widget %2 but it already contains a child with the given name.").arg(childName.c_str()).arg(GetFullName().c_str()) );
            QtWidgetNode * alreadyExistingNode = dynamic_cast<QtWidgetNode*>(childrenNode->FindChild(childName));
            if ( alreadyExistingNode != NULL )
            {
               if ( alreadyExistingNode->widget() == _child )
               {
                  QtWrapper::Log ( QString("WARN: The child %1 has been added to %2 previously.").arg(childName.c_str()).arg(GetFullName().c_str()) );
                  return QtWidgetNode::PREVIOUSLY_ADDED;
               }
               else if ( !alreadyExistingNode->widget()->isVisible() )
               {
                  QtWrapper::Log ( QString("WARN: The already existing child is not visible, removing that in favour of the new one.") );
                  childrenNode->RemoveChild ( childName );
                  return QtWidgetNode::ALREADY_EXISTS_BUT_REMOVED_SINCE_IT_IS_NOT_VISIBLE;
               }
            }
            QWidget * parent = _child->parentWidget();
            if ( parent != NULL )
            {
               int count = 0;
               for ( QObjectList::const_iterator it = parent->children().begin(); it != parent->children().end(); it++ )
               {
                  if ( QtWrapper::GetObjectName(*it) == childName )
                  {
                     count ++;
                  }
               }
               if ( count > 1 )
               {
                  QtWrapper::Log ( QString("WARN: %1 contains %2 children with the name %3, you might have a bug in your code.").arg(GetFullName().c_str()).arg(count).arg(childName.c_str()) );
               }
            }
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

QtWidgetNode * QtWidgetNode::FindNodeForWidget ( QWidget * _child )
{
   ListNode * childrenNode = dynamic_cast<ListNode*>( this->FindChild ( SQ_UI_COMMON_BASE_CHILDREN ) );
   if ( childrenNode != NULL )
   {
      ListNode::Iterator * it = childrenNode->ListChildren();
      for ( ; it->HasNext(); it->Next() )
      {
         QtWidgetNode * node = dynamic_cast<QtWidgetNode*>(it->GetCurrent());
         if ( node != NULL )
         {
            if ( node->widget() == _child )
            {
               delete it;
               return node;
            }
         }
      }
      delete it;
   }
   return NULL;
}

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
