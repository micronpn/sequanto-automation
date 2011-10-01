#include <sequanto/QtGlobalPositionProperty.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/ui.h>
#include <stdexcept>

using namespace sequanto::automation;

QtGlobalPositionProperty::QtGlobalPositionProperty ( const std::string & _name, QWidget * _object )
   : IntegerPropertyNode ( _name ),
     m_object ( _object )
{
}
   
const NodeInfo & QtGlobalPositionProperty::Info () const
{
   return this->GetReadOnlyIntegerNodeInfo();
}

int QtGlobalPositionProperty::GetValue ()
{
   return GetValue ( QtWrapper::GetPropertyValue ( m_object, QtWrapper::global_pos() ).toPoint() );
}

void QtGlobalPositionProperty::SetValue ( int _newValue )
{
   SQ_UNUSED_PARAMETER(_newValue);
   
   throw std::runtime_error ( "Can not set the global widget position." );
}

QtGlobalPositionProperty::~QtGlobalPositionProperty()
{
   m_object = NULL;
}

QtGlobalXProperty::QtGlobalXProperty ( QWidget * _object )
   : QtGlobalPositionProperty(SQ_UI_WIDGET_X, _object)
{
}

int QtGlobalXProperty::GetValue ( const QPoint & _value )
{
   return _value.x();
}

QtGlobalYProperty::QtGlobalYProperty ( QWidget * _object )
   : QtGlobalPositionProperty ( SQ_UI_WIDGET_Y, _object )
{
}

int QtGlobalYProperty::GetValue ( const QPoint & _value )
{
   return _value.y();
}
