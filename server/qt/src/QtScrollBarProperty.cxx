#include <sequanto/QtScrollBarProperty.h>

using namespace sequanto::automation;

QtScrollBarProperty::QtScrollBarProperty ( const std::string & _name, QObject * _object )
   : QtIntegerProperty(_name, _object)
{
   m_associatedScrollArea = NULL;
   m_hasSearchedForAssociatedScrollArea = false;
}
   
void QtScrollBarProperty::PropertyChanged ()
{
   QtIntegerProperty::SendUpdate ();

   if ( !m_hasSearchedForAssociatedScrollArea )
   {
      // Get my widget (which is my parent, since i am a property
      Node * parent = GetParent();
      if ( parent != NULL )
      {
         // My widget's parent (the parent widget's children list)
         parent = parent->GetParent();
         if ( parent != NULL )
         {
            // My actual parent (the QtWidgetNode instance)
            // For scrollbars on a QScrollArea this will be qt_scrollarea_vcontainer or qt_scrollarea_hcontainer
            parent = parent->GetParent();
            if ( parent != NULL )
            {
               // Get widget list of actual parent (hopefully this is a QScrollArea).
               parent = parent->GetParent();
               if ( parent != NULL )
               {
                  // Get actual parent
                  parent = parent->GetParent();
                  if ( parent != NULL )
                  {
                     QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(parent);
                     if ( widgetNode != NULL )
                     {
                        if ( widgetNode->type() == SQ_WIDGET_TYPE_SCROLL_AREA )
                        {
                           m_associatedScrollArea = widgetNode;
                        }
                     }
                  }
               }
            }
         }
      }         
      m_hasSearchedForAssociatedScrollArea = true;
   }
      
   if ( m_associatedScrollArea != NULL )
   {
      m_associatedScrollArea->SendPositionUpdateForAllChildren();
   }
}

QtScrollBarProperty::~QtScrollBarProperty()
{
}
