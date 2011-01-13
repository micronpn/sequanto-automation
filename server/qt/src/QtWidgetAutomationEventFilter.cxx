#include <sequanto/QtWidgetAutomationEventFilter.h>
#include <sequanto/QtAutomationGetPropertyEvent.h>
#include <sequanto/QtAutomationMoveEvent.h>
#include <sequanto/QtAutomationResizeEvent.h>
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
             QtWrapper::Log ( QString("%1: Adding child %2").arg(m_node->GetFullName().c_str()).arg(QtWrapper::GetObjectName(child).c_str()) );
             if ( QtWrapper::AddChild ( m_node, qobject_cast<QWidget*>(child) ) )
             {
                m_node->SendUpdate();
             }
          }
       }
        break;

    case QEvent::ChildRemoved:
       break;

    case QEvent::Move:
     
        if ( qobject_cast<QMainWindow*>(_object) != NULL || qobject_cast<QDialog*>(_object) != NULL )
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
      
    case QEvent::Destroy:
    case QEvent::DeferredDelete:
        m_node->WidgetDestroyed ();
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
    else
    {
        return QObject::eventFilter(_object, _event );
    }
}

QtWidgetAutomationEventFilter::~QtWidgetAutomationEventFilter ()
{
}
