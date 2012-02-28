#include <sequanto/QtTableColumnHeaderHeightProperty.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>

using namespace sequanto::automation;

QtTableColumnHeaderHeightProperty::QtTableColumnHeaderHeightProperty()
   : ReadOnlyIntegerPropertyNode(SQ_UI_TABLE_COLUMN_HEADER_HEIGHT)
{
}

int QtTableColumnHeaderHeightProperty::GetValue()
{
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(GetParent());
   Q_ASSERT ( widgetNode != NULL );
   
   QTableWidget * tableWidget = qobject_cast<QTableWidget*>(widgetNode->widget());
   Q_ASSERT ( tableWidget != NULL );
   
   QHeaderView * header = tableWidget->horizontalHeader();
   if ( header == NULL || !header->isVisible() )
   {
      return 0;
   }
   else
   {
      return header->height();
   }
}
