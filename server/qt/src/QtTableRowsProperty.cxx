#include <sequanto/QtTableRowsProperty.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>

using namespace sequanto::automation;

QtTableRowsProperty::QtTableRowsProperty ()
   : ReadOnlyIntegerPropertyNode ( SQ_UI_NODE_ROWS )
{
}

void QtTableRowsProperty::ParentResolved()
{
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*> ( GetParent () );
   if ( widgetNode != NULL )
   {
      QTableWidget * tableWidget = qobject_cast<QTableWidget*> ( widgetNode->widget() );
      if ( tableWidget != NULL )
      {
         m_propertyChangedAdapter = QtPropertyChangedNotificationAdapter::ConnectIfPossible ( tableWidget, "rowCount", this );
      }
   }
}

void QtTableRowsProperty::PropertyChanged ()
{
   this->SendUpdate ();
}

int QtTableRowsProperty::GetValue ()
{
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*> ( GetParent () );
   if ( widgetNode != NULL )
   {
      QTableWidget * tableWidget = qobject_cast<QTableWidget*> ( widgetNode->widget() );
      if ( tableWidget != NULL )
      {
         int rowCount = tableWidget->rowCount();
         for ( int i = 0; i < tableWidget->rowCount(); i++ )
         {
            if ( tableWidget->isRowHidden(i) )
            {
               rowCount --;
            }
         }
         return rowCount;
      }
   }
   return 0;
}

int QtTableRowsProperty::TranslateRow ( QTableWidget * _tableWidget, int _row )
{
   int rowCount = _tableWidget->rowCount();
   for ( int i = 0; i < rowCount; i++ )
   {
      if ( _tableWidget->isRowHidden(i) )
      {
         _row ++;
      }
      
      if ( i >= _row )
      {
         return _row;
      }
   }
   return _row;
}

QtTableRowsProperty::~QtTableRowsProperty()
{
   if ( m_propertyChangedAdapter )
   {
      delete m_propertyChangedAdapter;
      m_propertyChangedAdapter = NULL;
   }
}
