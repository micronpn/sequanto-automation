#include <sequanto/QtCachedProperty.h>

using namespace sequanto::automation;

QtCachedStringProperty::QtCachedStringProperty ( const std::string & _name, QWidget * _object, QtCacheItem::Property _cachedProperty )
   : QtStringProperty(_name, _object),
     m_cacheProperty(_cachedProperty)
{
}

void QtCachedStringProperty::HandleGet ( SQValue & _outputValue )
{
   QtCache::Instance().HandleGet ( qobject_cast<QWidget*>(m_object), m_cacheProperty, _outputValue );
}

QtCachedStringProperty::~QtCachedStringProperty()
{
}

QtCachedIntegerProperty::QtCachedIntegerProperty ( const std::string & _name, QWidget * _object, QtCacheItem::Property _cachedProperty )
   : QtIntegerProperty(_name, _object),
     m_cacheProperty(_cachedProperty)
{
}

void QtCachedIntegerProperty::HandleGet ( SQValue & _outputValue )
{
   QtCache::Instance().HandleGet ( qobject_cast<QWidget*>(m_object), m_cacheProperty, _outputValue );
}

QtCachedIntegerProperty::~QtCachedIntegerProperty()
{
}

QtCachedBooleanProperty::QtCachedBooleanProperty ( const std::string & _name, QWidget * _object, QtCacheItem::Property _cachedProperty )
   : QtBooleanProperty(_name, _object),
     m_cacheProperty(_cachedProperty)
{
}

void QtCachedBooleanProperty::HandleGet ( SQValue & _outputValue )
{
   QtCache::Instance().HandleGet ( qobject_cast<QWidget*>(m_object), m_cacheProperty, _outputValue );
}

QtCachedBooleanProperty::~QtCachedBooleanProperty()
{
}
