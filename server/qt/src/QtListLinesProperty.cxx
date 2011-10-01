#include <sequanto/QtListLinesProperty.h>
#include <sequanto/ui.h>
#include <sequanto/QtWidgetNode.h>

using namespace sequanto::automation;

QtListLinesProperty::QtListLinesProperty ()
   : ReadOnlyIntegerPropertyNode ( SQ_UI_LIST_LINES )
{
}

void QtListLinesProperty::ParentResolved()
{
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*> ( GetParent () );
   if ( widgetNode != NULL )
   {
      QListWidget * listWidget = qobject_cast<QListWidget*> ( widgetNode->widget() );
      if ( listWidget != NULL )
      {
         m_propertyChangedAdapter = QtPropertyChangedNotificationAdapter::ConnectIfPossible ( listWidget, "lines", this );
      }
   }
}

void QtListLinesProperty::PropertyChanged ()
{
   this->SendUpdate ();
}

int QtListLinesProperty::GetValue ()
{
   QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*> ( GetParent () );
   if ( widgetNode != NULL )
   {
      QListWidget * listWidget = qobject_cast<QListWidget*> ( widgetNode->widget() );
      if ( listWidget != NULL )
      {
         int count = listWidget->count();
         for ( int i = 0; i < listWidget->count(); i++ )
         {
            if ( listWidget->isRowHidden(i) )
            {
               count --;
            }
         }
         return count;
      }
   }
   return 0;
}

int QtListLinesProperty::TranslateLine ( QListWidget * _listWidget, int _line )
{
   int lines = _listWidget->count();
   for ( int i = 0; i < lines; i++ )
   {
      if ( _listWidget->isRowHidden(i) )
      {
         _line ++;
      }
      
      if ( i >= _line )
      {
         return _line;
      }
   }
   return _line;
}

QtListLinesProperty::~QtListLinesProperty()
{
   if ( m_propertyChangedAdapter )
   {
      delete m_propertyChangedAdapter;
      m_propertyChangedAdapter = NULL;
   }
}
