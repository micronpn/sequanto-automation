#include <sequanto/QtTableRowHeaderHeightProperty.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>
#include <cassert>

using namespace sequanto::automation;

QtTableRowHeaderHeightProperty::QtTableRowHeaderHeightProperty()
   : ReadOnlyIntegerPropertyNode(SQ_UI_TABLE_ROW_HEADER_HEIGHT)
{
}

int QtTableRowHeaderHeightProperty::GetValue()
{
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(GetParent());
   assert ( widgetNode != NULL );
   
   QTableWidget * tableWidget = qobject_cast<QTableWidget*>(widgetNode->widget());
   assert ( tableWidget != NULL );
   
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
