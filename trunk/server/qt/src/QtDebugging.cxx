#include <sequanto/methodinfo.h>
#include <sequanto/QtDebugging.h>
#include <sequanto/QtWrapper.h>

using namespace sequanto::automation;

QtDebuggingVerifyIntegrityMethod::QtDebuggingVerifyIntegrityMethod()
   : Node("verifyIntegrity")
{
}

const NodeInfo & QtDebuggingVerifyIntegrityMethod::Info () const
{
   return MethodInfo::voidVoid();
}

void QtDebuggingVerifyIntegrityMethod::HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
{
   ListNode * root = QtWrapper::ApplicationRoot();
   ListNode * windows = dynamic_cast<ListNode*>(root->FindChild ( SQ_UI_ROOT_WINDOWS ));
   QWidgetList allWidgets ( QApplication::allWidgets() );
   Q_FOREACH ( QWidget * widget, allWidgets )
   {
      if ( widget->isWindow() )
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

QtDebuggingVerifyIntegrityMethod::~QtDebuggingVerifyIntegrityMethod()
{
}
