#include <sequanto/QtTabsTabPositionProperty.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>

using namespace sequanto::automation;

QtTabsTabPositionProperty::QtTabsTabPositionProperty ()
   : ReadOnlyStringPropertyNode ( SQ_UI_TABS_TAB_POSITION )
{
}

const std::string & QtTabsTabPositionProperty::GetValue ()
{
   static std::string EMPTY;
   static std::string NORTH("North");
   static std::string SOUTH("South");
   
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*> ( GetParent () );
   if ( widgetNode != NULL )
   {
      QTabWidget * tabWidget = qobject_cast<QTabWidget*> ( widgetNode->widget() );
      if ( tabWidget != NULL )
      {
         switch ( tabWidget->tabPosition() )
         {
         case QTabWidget::North:
            return NORTH;
               
         case QTabWidget::South:
            return SOUTH;
            
         default:
            break;
         }
      }
   }
   return EMPTY;
}

QtTabsTabPositionProperty::~QtTabsTabPositionProperty()
{
}
