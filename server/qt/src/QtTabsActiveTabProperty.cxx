#include <sequanto/QtTabsActiveTabProperty.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>

using namespace sequanto::automation;

QtTabsActiveTabProperty::QtTabsActiveTabProperty ()
   : ReadOnlyStringPropertyNode ( SQ_UI_TABS_ACTIVE_TAB )
{
}

void QtTabsActiveTabProperty::ParentResolved()
{
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*> ( GetParent () );
   if ( widgetNode != NULL )
   {
      QTabWidget * tabWidget = qobject_cast<QTabWidget*> ( widgetNode->widget() );
      if ( tabWidget != NULL )
      {
         m_itemChangedAdapter = new QtPropertyChangedNotificationAdapter ( this, tabWidget, "currentChanged(int)" );
      }
   }
}

void QtTabsActiveTabProperty::PropertyChanged ()
{
   bool foundTabName = false;
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*> ( GetParent () );
   if ( widgetNode != NULL )
   {
      QTabWidget * tabWidget = qobject_cast<QTabWidget*> ( widgetNode->widget() );
      if ( tabWidget != NULL )
      {
         int index = tabWidget->currentIndex();
         if ( index != -1 )
         {
            m_activeTab = QtWrapper::ToString( tabWidget->tabText ( index ) );
            foundTabName = true;
         }
      }
   }

   if ( !foundTabName )
   {
      m_activeTab = "";
   }

   this->SendUpdate ();
}

const std::string & QtTabsActiveTabProperty::GetValue ()
{
   return m_activeTab;
}

QtTabsActiveTabProperty::~QtTabsActiveTabProperty()
{
   if ( m_itemChangedAdapter )
   {
      delete m_itemChangedAdapter;
      m_itemChangedAdapter = NULL;
   }
}
