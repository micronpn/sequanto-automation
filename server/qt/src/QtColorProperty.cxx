#include <sequanto/QtColorProperty.h>
#include <sequanto/QtWrapper.h>

using namespace sequanto::automation;

QtColorProperty::QtColorProperty ( const std::string & _name, QWidget * _widget, QPalette::ColorRole _role )
   : PropertyNode ( _name ),
     m_widget ( _widget ),
     m_role ( _role )
{
}

const NodeInfo & QtColorProperty::Info () const
{
   return GetStringNodeInfo();
}

void QtColorProperty::PropertyChanged ()
{
   SendUpdate ();
}

void QtColorProperty::HandleGet ( SQValue & _outputValue )
{
   QPalette palette = m_widget->palette();
   QBrush brush ( palette.brush(m_role) );
   QColor color ( Qt::magenta );
   
   switch ( brush.style() )
   {
   case Qt::NoBrush:
   case Qt::SolidPattern:
   case Qt::Dense1Pattern:
   case Qt::Dense2Pattern:
   case Qt::Dense3Pattern:
   case Qt::Dense4Pattern:
   case Qt::Dense5Pattern:
   case Qt::Dense6Pattern:
   case Qt::Dense7Pattern:
   case Qt::HorPattern:
   case Qt::VerPattern:
   case Qt::CrossPattern:
   case Qt::BDiagPattern:
   case Qt::FDiagPattern:
   case Qt::DiagCrossPattern:
      color = brush.color();
      break;
      
   case Qt::LinearGradientPattern:
   case Qt::ConicalGradientPattern:
   case Qt::RadialGradientPattern:
      {
         QGradientStops stops ( brush.gradient()->stops() );
         if ( stops.size() > 0 )
         {
            color = stops[0].second;
         }
      }
      break;
      
   case Qt::TexturePattern:
      break;
   }
   std::string stringValue ( QtWrapper::ToString(color.name()) );
   sq_value_string_copy ( &_outputValue, stringValue.c_str() );
}

void QtColorProperty::HandleSet ( const SQValue * const _value )
{
   QString stringValue ( _value->Value.m_stringValue );
   if ( QColor::isValidColor(stringValue) )
   {
      QColor value ( stringValue );
      QPalette palette = m_widget->palette();
      palette.setColor ( m_role, value );
      m_widget->setPalette ( palette );
   }
}

QtColorProperty::~QtColorProperty()
{
}
