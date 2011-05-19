#include <sequanto/QtWidgetAutomationEventFilter.h>
#include <sequanto/QtAutomationGetPropertyEvent.h>
#include <sequanto/QtAutomationMoveEvent.h>
#include <sequanto/QtAutomationResizeEvent.h>
#include <sequanto/QtAutomationMouseMoveEvent.h>
#include <sequanto/QtAutomationMouseClickEvent.h>
#include <sequanto/QtAutomationChildAddedEvent.h>
#include <sequanto/QtWidgetNode.h>
#include <sequanto/ui.h>
#include <cassert>

#include <sequanto/QtWrapper.h>

using namespace sequanto::automation;

QtWidgetAutomationEventFilter::QtWidgetAutomationEventFilter ( QtWidgetNode * _node )
    : QObject(_node->widget()),
      m_node ( _node )
{
}

bool QtWidgetAutomationEventFilter::eventFilter ( QObject * _object, QEvent * _event )
{
    switch ( _event->type() )
    {
    case QEvent::ChildAdded:
       {
          QChildEvent * childEvent = dynamic_cast<QChildEvent*>(_event );
          QObject * child = childEvent->child();
          if ( child->isWidgetType () )
          {
             QWidget * childWidget = qobject_cast<QWidget*>(child);
             QApplication::postEvent ( m_node->widget(), new QtAutomationChildAddedEvent(childWidget) );
          }
       }
       break;

/*
    case QEvent::ChildRemoved:
       {
          QChildEvent * childEvent = dynamic_cast<QChildEvent*>(_event );
          QObject * child = childEvent->child();
          if ( child->isWidgetType () )
          {
             QtWrapper::Log ( QString("%1: Removing child %2").arg(m_node->GetFullName().c_str()).arg(QtWrapper::GetObjectName(child).c_str()) );
             if ( m_node->RemoveChildWidget ( qobject_cast<QWidget*>(child) ) )
             {
                m_node->SendChildrenUpdate();
             }
          }
       }
       break;
*/

    case QEvent::Move:
       if ( _object->isWidgetType() && QtWrapper::IsWindow(qobject_cast<QWidget*>(_object)) )
        {
           Node * screenX = m_node->FindChild(SQ_UI_WINDOW_SCREEN_X);
           Node * screenY = m_node->FindChild(SQ_UI_WINDOW_SCREEN_Y);
           if ( screenX != NULL )
           {
              dynamic_cast<IntegerPropertyNode*> ( screenX )->SendUpdate();
           }
           if ( screenY != NULL )
           {
              dynamic_cast<IntegerPropertyNode*> ( screenY )->SendUpdate();
           }
        }
        else
        {
            dynamic_cast<IntegerPropertyNode*> ( m_node->FindChild(SQ_UI_NODE_X) )->SendUpdate();
            dynamic_cast<IntegerPropertyNode*> ( m_node->FindChild(SQ_UI_NODE_Y) )->SendUpdate();
        }
        break;
      
    case QEvent::Resize:
        dynamic_cast<IntegerPropertyNode*> ( m_node->FindChild(SQ_UI_NODE_WIDTH) )->SendUpdate();
        dynamic_cast<IntegerPropertyNode*> ( m_node->FindChild(SQ_UI_NODE_HEIGHT) )->SendUpdate();
        break;

    case QEvent::EnabledChange:
       dynamic_cast<BooleanPropertyNode*> ( m_node->FindChild(SQ_UI_NODE_ENABLED) )->SendUpdate();
       break;

    case QEvent::Show:
    case QEvent::Hide:
       dynamic_cast<BooleanPropertyNode*> ( m_node->FindChild(SQ_UI_NODE_VISIBLE) )->SendUpdate();
       break;

    default:
       break;
    }

    if ( _event->type() == QtAutomationMoveEvent::ID )
    {
        QPoint position = dynamic_cast<QtAutomationMoveEvent*>(_event)->position();
        qobject_cast<QWidget*> ( _object )->move ( position );
        return true;
    }
    else if ( _event->type() == QtAutomationResizeEvent::ID )
    {
        QSize size = dynamic_cast<QtAutomationResizeEvent*>(_event)->size();
        qobject_cast<QWidget*> ( _object )->resize ( size );
        return true;
    }
    else if ( _event->type() == QtAutomationGetPropertyEvent::ID )
    {
        QtAutomationGetPropertyEvent * event = dynamic_cast<QtAutomationGetPropertyEvent*>(_event);
     
        if ( event->propertyName() == QtWrapper::screen_pos() )
        {
            QWidget * widget = qobject_cast<QWidget*>(_object);
            event->done ( widget->geometry().topLeft() );
        }
        else if ( event->propertyName() == QtWrapper::global_pos() )
        {
            QWidget * widget = qobject_cast<QWidget*>(_object);
            QWidget * window = widget->window();
       
            QPoint pos = widget->mapToGlobal(QPoint(0,0));
            QPoint windowTopLeft ( window->geometry().topLeft() );
	
            pos -= windowTopLeft;

            //QPoint pos = widget->mapTo ( window, widget->pos() );
       
            //qDebug() << "Filtered: " << pos;
       
            event->done ( pos );
        }
        else
        {
            event->done ( qobject_cast<QWidget*> ( _object )->property ( event->propertyName() ) );
        }
        return true;
    }
    else if ( _event->type() == QtAutomationMouseMoveEvent::ID )
    {
       QtAutomationMouseMoveEvent * event = dynamic_cast<QtAutomationMouseMoveEvent*>(_event);
       QApplication::postEvent ( this, new QMouseEvent( QEvent::MouseMove, event->position(), Qt::NoButton, Qt::NoButton, Qt::NoModifier ) );
       return true;
    }
    else if ( _event->type() == QtAutomationMouseClickEvent::ID )
    {
       QtAutomationMouseClickEvent * event = dynamic_cast<QtAutomationMouseClickEvent*>(_event);

       QWidget * receiver = QApplication::widgetAt(event->position());
       
       if ( receiver != 0 )
       {
          QPoint widgetPos = receiver->mapFromGlobal ( event->position() );
          
          QApplication::postEvent ( receiver, new QMouseEvent( QEvent::MouseButtonPress, widgetPos, event->position(), event->button(), event->button(), Qt::NoModifier ) );
          QApplication::postEvent ( receiver, new QMouseEvent( QEvent::MouseButtonRelease, widgetPos, event->position(), event->button(), event->button(), Qt::NoModifier ) );
       }
       return true;
    }
    else if ( _event->type() == QtAutomationChildAddedEvent::ID )
    {
       QtAutomationChildAddedEvent * event = dynamic_cast<QtAutomationChildAddedEvent*>(_event);
       QWidget * childWidget = event->child();
       if ( childWidget != NULL )
       {
          if ( QtWrapper::IsWindow(childWidget) )
          {
             QtWrapper::UpdateWindows();
          }
          else
          {
             switch ( m_node->AddChildWidget ( childWidget ) )
             {
             case QtWidgetNode::ADDED:
                m_node->SendChildrenUpdate();
                break;
                
             case QtWidgetNode::ALREADY_EXISTS:
                if ( event->iteration() < 2 )
                {
                   QtWrapper::Log ( QString("WARN: Child was not added the first time around, trying again.") );
                   QApplication::postEvent ( _object, new QtAutomationChildAddedEvent( childWidget, event->iteration() + 1 ) );
                }
                break;

             case QtWidgetNode::NOT_ADDED:
                break;
             }
          }
       }
       return true;
    }
    else
    {
        return QObject::eventFilter(_object, _event );
    }
}

QtWidgetAutomationEventFilter::~QtWidgetAutomationEventFilter ()
{
   m_node->WidgetDestroyed();
}
