#include <sequanto/qtautomationeventfilter.h>
#include <sequanto/ui.h>
#include <cassert>

#include <sequanto/qtwrapper.h>

using namespace sequanto::automation;

const int QtAutomationMoveEvent::ID = QEvent::registerEventType();

QtAutomationMoveEvent::QtAutomationMoveEvent ( int _x, int _y )
   : QEvent( (QEvent::Type) ID),
     m_position(_x, _y)
{
}
 
const QPoint & QtAutomationMoveEvent::position()
{
   return m_position;
}

QtAutomationMoveEvent::~QtAutomationMoveEvent()
{
}

const int QtAutomationResizeEvent::ID = QEvent::registerEventType();

QtAutomationResizeEvent::QtAutomationResizeEvent ( int _width, int _height )
   : QEvent( (QEvent::Type) ID),
     m_size(_width, _height)
{
}

const QSize & QtAutomationResizeEvent::size()
{
   return m_size;
}

QtAutomationResizeEvent::~QtAutomationResizeEvent()
{
}


const int QtAutomationGetPropertyEvent::ID = QEvent::registerEventType();

QtAutomationGetPropertyEvent::QtAutomationGetPropertyEvent ( const char * const _propertyName )
   : QEvent( (QEvent::Type) ID),
     m_propertyName(_propertyName)
{
}
 
const QVariant & QtAutomationGetPropertyEvent::value() const
{
   return m_value;
}

const char * QtAutomationGetPropertyEvent::propertyName() const
{
  return m_propertyName;
}

void QtAutomationGetPropertyEvent::done( const QVariant & _value )
{
   m_lock.lock();

  m_value = _value;
  
  m_waitCondition.wakeAll ();

  m_lock.unlock();

  m_doneLock.lock ();

  m_doneLock.unlock ();
}

QVariant QtAutomationGetPropertyEvent::wait(QObject * _objectToPostEventTo )
{
   QVariant ret;

   m_doneLock.lock();
   m_lock.lock();

   QCoreApplication::postEvent ( _objectToPostEventTo, this );
   
   m_waitCondition.wait ( &m_lock );

   ret = m_value;

   m_lock.unlock();
   m_doneLock.unlock();

   return ret;
}

QtAutomationGetPropertyEvent::~QtAutomationGetPropertyEvent()
{
}

QtAutomationEventFilter::QtAutomationEventFilter ( ListNode * _node, QObject * _parent )
      : QObject(_parent),
        m_node ( _node )
{
}

bool QtAutomationEventFilter::eventFilter ( QObject * _object, QEvent * _event )
{
   switch ( _event->type() )
   {
   case QEvent::ChildAdded:
   case QEvent::ChildRemoved:
      m_node->SendUpdate();
      break;

   case QEvent::Move:
     
      if ( qobject_cast<QMainWindow*>(_object) != NULL || qobject_cast<QDialog*>(_object) != NULL )
      {
	dynamic_cast<IntegerPropertyNode*> ( m_node->FindChild(SQ_UI_WINDOW_SCREEN_X) )->SendUpdate();
	dynamic_cast<IntegerPropertyNode*> ( m_node->FindChild(SQ_UI_WINDOW_SCREEN_Y) )->SendUpdate();
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
	
       QPoint pos = widget->mapTo ( window, widget->pos() );
       qDebug() << "Filtered: " << pos;
       
       event->done ( pos );
       
	//qDebug() << widget->objectName() << " (in event handler>";
	//qDebug() << "   Global: " << widget->mapTo(widget->window(), widget->pos());
	//event->done ( widget->mapTo(widget->window(), widget->pos()) );
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

QtAutomationEventFilter::~QtAutomationEventFilter ()
{
}

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
