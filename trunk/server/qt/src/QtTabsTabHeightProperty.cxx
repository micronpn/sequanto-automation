#include <sequanto/QtTabsTabHeightProperty.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>

using namespace sequanto::automation;

QtTabsTabHeightProperty::QtTabsTabHeightProperty ()
   : ReadOnlyIntegerPropertyNode ( SQ_UI_TABS_TAB_HEIGHT )
{
}

int QtTabsTabHeightProperty::GetValue ()
{
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*> ( GetParent () );
   if ( widgetNode != NULL )
   {
      QTabWidget * tabWidget = qobject_cast<QTabWidget*> ( widgetNode->widget() );
      if ( tabWidget != NULL )
      {
         return 14;
      }
   }
   return 0;
}

QtTabsTabHeightProperty::~QtTabsTabHeightProperty()
{
}
