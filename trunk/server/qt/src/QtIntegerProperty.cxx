#include <sequanto/QtIntegerProperty.h>
#include <sequanto/QtWrapper.h>
#include <cassert>

using namespace sequanto::automation;

QtIntegerProperty::QtIntegerProperty ( const std::string & _name, QObject * _object )
   : IntegerPropertyNode ( _name ),
     m_object ( _object ),
     m_notifyAdapter ( NULL )
{
   m_notifyAdapter = QtPropertyChangedNotificationAdapter::ConnectIfPossible ( _object, _name, this );
}

void QtIntegerProperty::PropertyChanged ()
{
   this->SendUpdate ();
}

int QtIntegerProperty::GetValue ()
{
   assert ( m_object != NULL );
     
   QVariant value = QtWrapper::GetPropertyValue ( m_object, GetName() );
   if ( value.isValid() )
   {
      return value.toInt();
   }
   else
   {
      qDebug() << "    value is NOT valid.";
       
      return -1;
   }
}

void QtIntegerProperty::SetValue ( int _newValue )
{
   QVariant variantValue ( _newValue );
   m_object->setProperty(GetName().c_str(), variantValue );
}

QtIntegerProperty::~QtIntegerProperty()
{
   if ( m_notifyAdapter )
   {
      delete m_notifyAdapter;
      m_notifyAdapter = NULL;
   }
}
