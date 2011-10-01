#include <sequanto/QtListUpdateProperty.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>

using namespace sequanto::automation;

QtListUpdateProperty::QtListUpdateProperty ()
   : ReadOnlyIntegerPropertyNode ( SQ_UI_LIST_UPDATE ),
     m_update ( 0 )
{
}

void QtListUpdateProperty::ParentResolved()
{
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*> ( GetParent () );
   if ( widgetNode != NULL )
   {
      QListWidget * listWidget = qobject_cast<QListWidget*> ( widgetNode->widget() );
      if ( listWidget != NULL )
      {
         m_itemChangedAdapter = new QtPropertyChangedNotificationAdapter ( this, listWidget, "itemChanged(QListWidgetItem*)" );
      }
   }
}

void QtListUpdateProperty::PropertyChanged ()
{
   m_update ++;
   this->SendUpdate ();
}

int QtListUpdateProperty::GetValue ()
{
   return m_update;
}

QtListUpdateProperty::~QtListUpdateProperty()
{
   if ( m_itemChangedAdapter )
   {
      delete m_itemChangedAdapter;
      m_itemChangedAdapter = NULL;
   }
}
