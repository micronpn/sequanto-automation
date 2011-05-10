#include <sequanto/QtBooleanProperty.h>
#include <sequanto/QtWrapper.h>

using namespace sequanto::automation;

QtBooleanProperty::QtBooleanProperty ( const std::string & _name, QObject * _object )
   : BooleanPropertyNode ( _name ),
     m_object ( _object ),
     m_notifyAdapter ( NULL )
{
   m_notifyAdapter = QtPropertyChangedNotificationAdapter::ConnectIfPossible ( _object, _name, this );
}

void QtBooleanProperty::PropertyChanged ()
{
   this->SendUpdate ();
}

bool QtBooleanProperty::GetValue ()
{
   return QtWrapper::GetPropertyValue ( m_object, GetName() ).toBool();
}

void QtBooleanProperty::SetValue ( bool _newValue )
{
   QVariant variantValue ( _newValue );
   m_object->setProperty(GetName().c_str(), variantValue );
}

QtBooleanProperty::~QtBooleanProperty()
{
   if ( m_notifyAdapter )
   {
      delete m_notifyAdapter;
      m_notifyAdapter = NULL;
   }
}
