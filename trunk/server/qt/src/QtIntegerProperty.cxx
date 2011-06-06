#include <sequanto/QtIntegerProperty.h>
#include <sequanto/QtWrapper.h>
#include <cassert>

using namespace sequanto::automation;

QtIntegerProperty::QtIntegerProperty ( const std::string & _name, QObject * _object )
   : IntegerPropertyNode ( _name ),
     m_object ( _object ),
     m_notifyAdapter ( NULL )
{
   m_notifyAdapter = QtPropertyChangedNotificationAdapter::ConnectIfPossible ( _object, QtPropertyName(), this );
}

const std::string & QtIntegerProperty::QtPropertyName () const
{
   return GetName();
}


void QtIntegerProperty::PropertyChanged ()
{
   this->SendUpdate ();
}

int QtIntegerProperty::GetValue ()
{
   assert ( m_object != NULL );
     
   QVariant value = QtWrapper::GetPropertyValue ( m_object, QtPropertyName() );
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
   m_object->setProperty(QtPropertyName().c_str(), variantValue );
}

QtIntegerProperty::~QtIntegerProperty()
{
   if ( m_notifyAdapter )
   {
      delete m_notifyAdapter;
      m_notifyAdapter = NULL;
   }
}

QtIntegerPropertyWithAlternateName::QtIntegerPropertyWithAlternateName ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName )
   : QtIntegerProperty(_propertyName, _object)
{
   init ( _propertyName, _object, _qtPropertyName, _qtPropertyName );
}

QtIntegerPropertyWithAlternateName::QtIntegerPropertyWithAlternateName ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName, const std::string & _qtPropertyToUseForNotification  )
   : QtIntegerProperty(_propertyName, _object)
{
   init ( _propertyName, _object, _qtPropertyName, _qtPropertyToUseForNotification );
}

const std::string & QtIntegerPropertyWithAlternateName::QtPropertyName() const
{
   return m_qtPropertyName;
}

void QtIntegerPropertyWithAlternateName::init ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName, const std::string & _qtPropertyToUseForNotification )
{
   SQ_UNUSED_PARAMETER(_propertyName);
   
   m_qtPropertyName = _qtPropertyName;
   // We need to subscribe to the notification again, because the base constructor does not use our QtPropertyName() implementation when connecting.
   if ( m_notifyAdapter != NULL )
   {
      delete m_notifyAdapter;
   }
   m_notifyAdapter = QtPropertyChangedNotificationAdapter::ConnectIfPossible ( _object, _qtPropertyToUseForNotification, this );
}

QtIntegerPropertyWithAlternateName::~QtIntegerPropertyWithAlternateName ()
{
}
