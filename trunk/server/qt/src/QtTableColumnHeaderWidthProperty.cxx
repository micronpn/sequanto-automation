#include <sequanto/QtTableColumnHeaderWidthProperty.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>
#include <cassert>

using namespace sequanto::automation;

QtTableColumnHeaderWidthProperty::QtTableColumnHeaderWidthProperty()
   : ReadOnlyIntegerPropertyNode(SQ_UI_NODE_COLUMN_HEADER_WIDTH)
{
}

int QtTableColumnHeaderWidthProperty::GetValue()
{
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(GetParent());
   assert ( widgetNode != NULL );
   
   QTableWidget * tableWidget = qobject_cast<QTableWidget*>(widgetNode->widget());
   assert ( tableWidget != NULL );
   
   QHeaderView * header = tableWidget->verticalHeader();
   if ( header == NULL || !header->isVisible() )
   {
      return 0;
   }
   else
   {
      return header->width();
   }
}
