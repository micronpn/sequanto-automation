#include <sequanto/methodinfo.h>
#include <sequanto/QtDebugging.h>
#include <sequanto/QtWrapper.h>

using namespace sequanto::automation;

QtDebuggingCallMethod::QtDebuggingCallMethod(Method _method)
   : Node(MethodName(_method)),
     m_method(_method)
{
}

const std::string & QtDebuggingCallMethod::MethodName(Method _method)
{
   static std::string verifyIntegrity ( "verifyIntegrity" );
   static std::string unknown ( "unknown" );
    
   switch ( _method )
   {
   case VERIFY_INTEGRITY:
      return verifyIntegrity;

   default:
      return unknown;
   }
}

const NodeInfo & QtDebuggingCallMethod::Info () const
{
   return MethodInfo::voidVoid();
}

void QtDebuggingCallMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   SQ_UNUSED_PARAMETER(_numberOfValues);
   SQ_UNUSED_PARAMETER(_inputValues);
   SQ_UNUSED_PARAMETER(_output);

   QApplication::postEvent ( QApplication::instance(), new QtDebuggingEvent(m_method) );
}

void QtDebuggingCallMethod::Handle ( Method _method )
{
   switch ( _method )
   {
   case VERIFY_INTEGRITY:
      {
         ListNode * root = QtWrapper::ApplicationRoot();
         ListNode * windows = dynamic_cast<ListNode*>(root->FindChild ( SQ_UI_ROOT_WINDOWS ));
         QWidgetList allWidgets ( QApplication::allWidgets() );
         Q_FOREACH ( QWidget * widget, allWidgets )
         {
            if ( widget->isWindow() && widget == widget->window() &&
                 ( widget->inherits(QMainWindow::staticMetaObject.className()) ||
                   widget->inherits(QDialog::staticMetaObject.className()) ) )
            {
               std::string name = QtWrapper::GetObjectName( widget );
               QtWidgetNode * node = dynamic_cast<QtWidgetNode*>(windows->FindChild(name));
               if ( node == NULL )
               {
                  QtWrapper::Log(QString("ERR: QT has a window called %1, but sequanto-automation does not know about it (called %2 by sequanto automation).").arg(widget->objectName()).arg(name.c_str()) );
               }
               else
               {
                  if ( node->widget() != widget )
                  {
                     QtWrapper::Log ( QString("ERR: The window know as %1 by sequanto-automation does not point to the same QWidget* as QT.").arg(name.c_str()) );
                  }
               }
            }
         }
      }
   }
}

QtDebuggingCallMethod::~QtDebuggingCallMethod()
{
}

const int QtDebuggingEvent::ID = QEvent::registerEventType();

QtDebuggingEvent::QtDebuggingEvent ( QtDebuggingCallMethod::Method _method )
   : QEvent( (QEvent::Type) ID),
     m_method(_method)
{
}

void QtDebuggingEvent::Handle ()
{
   QtDebuggingCallMethod::Handle ( m_method );
}

QtDebuggingEvent::~QtDebuggingEvent()
{
}
