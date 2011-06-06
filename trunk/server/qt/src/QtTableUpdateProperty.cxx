#include <sequanto/QtTableUpdateProperty.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>

using namespace sequanto::automation;

QtTableUpdateProperty::QtTableUpdateProperty ()
   : ReadOnlyIntegerPropertyNode ( SQ_UI_NODE_UPDATE ),
     m_update ( 0 )
{
}

void QtTableUpdateProperty::ParentResolved()
{
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*> ( GetParent () );
   if ( widgetNode != NULL )
   {
      QTableWidget * tableWidget = qobject_cast<QTableWidget*> ( widgetNode->widget() );
      if ( tableWidget != NULL )
      {
         m_itemChangedAdapter = new QtPropertyChangedNotificationAdapter ( this, tableWidget, "cellChanged(int,int)" );
      }
   }
}

void QtTableUpdateProperty::PropertyChanged ()
{
   m_update ++;
   this->SendUpdate ();
}

int QtTableUpdateProperty::GetValue ()
{
   return m_update;
}

QtTableUpdateProperty::~QtTableUpdateProperty()
{
   if ( m_itemChangedAdapter )
   {
      delete m_itemChangedAdapter;
      m_itemChangedAdapter = NULL;
   }
}
