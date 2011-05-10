#include <sequanto/QtScreenPositionProperty.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/ui.h>
#include <stdexcept>

using namespace sequanto::automation;

QtScreenPositionProperty::QtScreenPositionProperty ( const std::string & _name, QWidget * _object )
   : IntegerPropertyNode ( _name ),
     m_object ( _object )
{
}
   
const NodeInfo & QtScreenPositionProperty::Info () const
{
   return this->GetReadOnlyIntegerNodeInfo();
}

int QtScreenPositionProperty::GetValue ()
{
   return GetValue ( QtWrapper::GetPropertyValue ( m_object, QtWrapper::screen_pos() ).toPoint() );
}

void QtScreenPositionProperty::SetValue ( int _newValue )
{
   SQ_UNUSED_PARAMETER(_newValue);
   
   throw std::runtime_error ( "Can not set the screen position." );
}

QtScreenPositionProperty::~QtScreenPositionProperty()
{
   m_object = NULL;
}

QtScreenXProperty::QtScreenXProperty ( QWidget * _object )
   : QtScreenPositionProperty(SQ_UI_WINDOW_SCREEN_X, _object)
{
}

int QtScreenXProperty::GetValue ( const QPoint & _value )
{
   return _value.x();
}

QtScreenYProperty::QtScreenYProperty ( QWidget * _object )
   : QtScreenPositionProperty(SQ_UI_WINDOW_SCREEN_Y, _object)
{
}

int QtScreenYProperty::GetValue ( const QPoint & _value )
{
   return _value.y();
}
