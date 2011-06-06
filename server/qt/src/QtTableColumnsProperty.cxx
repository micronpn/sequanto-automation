#include <sequanto/QtTableColumnsProperty.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>

using namespace sequanto::automation;

QtTableColumnsProperty::QtTableColumnsProperty ()
   : ReadOnlyIntegerPropertyNode ( SQ_UI_NODE_COLUMNS )
{
}

void QtTableColumnsProperty::ParentResolved()
{
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*> ( GetParent () );
   if ( widgetNode != NULL )
   {
      QTableWidget * tableWidget = qobject_cast<QTableWidget*> ( widgetNode->widget() );
      if ( tableWidget != NULL )
      {
         m_propertyChangedAdapter = QtPropertyChangedNotificationAdapter::ConnectIfPossible ( tableWidget, "columnCount", this );
      }
   }
}

void QtTableColumnsProperty::PropertyChanged ()
{
   this->SendUpdate ();
}

int QtTableColumnsProperty::GetValue ()
{
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*> ( GetParent () );
   if ( widgetNode != NULL )
   {
      QTableWidget * tableWidget = qobject_cast<QTableWidget*> ( widgetNode->widget() );
      if ( tableWidget != NULL )
      {
         int columnCount = tableWidget->columnCount();
         for ( int i = 0; i < tableWidget->columnCount(); i++ )
         {
            if ( tableWidget->isColumnHidden(i) )
            {
               columnCount --;
            }
         }
         return columnCount;
      }
   }
   return 0;
}

int QtTableColumnsProperty::TranslateColumn ( QTableWidget * _tableWidget, int _column )
{
   //qDebug() << "TranslateColumn: " << _column;
   
   int columnCount = _tableWidget->columnCount();
   for ( int i = 0; i < columnCount; i++ )
   {
      //qDebug() << "Column " << i << " hidden? " << _tableWidget->isColumnHidden(i);
      if ( _tableWidget->isColumnHidden(i) )
      {
         _column ++;
      }
      if ( i >= _column )
      {
         //qDebug() << "returning " << _column;
         return _column;
      }
      //qDebug() << "    _column is now " << _column;
   }
   //qDebug() << "returning " << _column;
   return _column;
}

QtTableColumnsProperty::~QtTableColumnsProperty()
{
   if ( m_propertyChangedAdapter )
   {
      delete m_propertyChangedAdapter;
      m_propertyChangedAdapter = NULL;
   }
}
