#include <sequanto/QtStringProperty.h>
#include <sequanto/QtWrapper.h>

using namespace sequanto::automation;

const std::string & QtStringProperty::QtPropertyName () const
{
   return GetName();
}

QtStringProperty::QtStringProperty ( const std::string & _name, QObject * _object )
   : PropertyNode ( _name ),
     m_object ( _object ),
     m_notifyAdapter ( NULL )
{
   m_notifyAdapter = QtPropertyChangedNotificationAdapter::ConnectIfPossible ( _object, QtPropertyName(), this );
}

const NodeInfo & QtStringProperty::Info () const
{
   return GetStringNodeInfo();
}

void QtStringProperty::PropertyChanged ()
{
   SendUpdate ();
}

void QtStringProperty::HandleGet ( SQValue & _outputValue )
{
   std::string value ( QtWrapper::ToString(QtWrapper::GetPropertyValue ( m_object, QtPropertyName() ).toString()) );
   sq_value_string_copy ( &_outputValue, value.c_str() );
}

void QtStringProperty::HandleSet ( const SQValue * const _value )
{
   QVariant variantValue ( _value->Value.m_stringValue );
   m_object->setProperty(QtPropertyName().c_str(), variantValue );
}

QtStringProperty::~QtStringProperty()
{
   if ( m_notifyAdapter )
   {
      delete m_notifyAdapter;
      m_notifyAdapter = NULL;
   }
}

QtStringPropertyWithAlternateName::QtStringPropertyWithAlternateName ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName )
   : QtStringProperty(_propertyName, _object)
{
   init ( _propertyName, _object, _qtPropertyName, _qtPropertyName );
}

QtStringPropertyWithAlternateName::QtStringPropertyWithAlternateName ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName, const std::string & _qtPropertyToUseForNotification  )
   : QtStringProperty(_propertyName, _object)
{
   init ( _propertyName, _object, _qtPropertyName, _qtPropertyToUseForNotification );
}

const std::string & QtStringPropertyWithAlternateName::QtPropertyName() const
{
   return m_qtPropertyName;
}

void QtStringPropertyWithAlternateName::init ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName, const std::string & _qtPropertyToUseForNotification )
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

QtStringPropertyWithAlternateName::~QtStringPropertyWithAlternateName ()
{
}
