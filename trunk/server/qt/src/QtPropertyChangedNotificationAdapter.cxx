#include <sequanto/QtPropertyChangedNotificationAdapter.h>
#include <QtCore>
#include <iostream>

using namespace sequanto::automation;

QtPropertyChangedNotificationAdapter::QtPropertyChangedNotificationAdapter ( IQtPropertyChangedReceiver * _receiver, QObject * _object, const std::string & _methodName )
   : QObject(),
     m_object ( _object ),
     m_receiver ( _receiver )
{
   m_methodName = "2";
   m_methodName += _methodName;
   
   QObject::connect ( m_object, m_methodName.c_str(),
                      this, SLOT(propertyChanged()) );
}

void QtPropertyChangedNotificationAdapter::propertyChanged ()
{
   m_receiver->PropertyChanged ();
}

QtPropertyChangedNotificationAdapter::~QtPropertyChangedNotificationAdapter()
{
   QObject::disconnect ( m_object, m_methodName.c_str(),
                        this, SLOT(propertyChanged()) );
}

QtPropertyChangedNotificationAdapter * QtPropertyChangedNotificationAdapter::ConnectIfPossible ( QObject * _object, const std::string & _propertyName, IQtPropertyChangedReceiver * _receiver )
{
#if QT_VERSION >= 0x040500
      int propertyIndex = _object->metaObject()->indexOfProperty(_propertyName.c_str());
      QMetaProperty property ( _object->metaObject()->property(propertyIndex) );
      if ( property.isValid() && property.hasNotifySignal() )
      {
         return new QtPropertyChangedNotificationAdapter(_receiver, _object, property.notifySignal().signature());
      }
      else
      {
         return NULL;
      }
#endif
}

