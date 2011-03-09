#include <sequanto/QtPropertyChangedNotificationAdapter.h>
#include <QtCore>
#include <iostream>

using namespace sequanto::automation;

QtPropertyChangedNotificationAdapter::QtPropertyChangedNotificationAdapter ( IQtPropertyChangedReceiver * _receiver )
   : QObject(),
     m_receiver ( _receiver )
{
}

void QtPropertyChangedNotificationAdapter::propertyChanged ()
{
   m_receiver->PropertyChanged ();
}

QtPropertyChangedNotificationAdapter::~QtPropertyChangedNotificationAdapter()
{
}

QtPropertyChangedNotificationAdapter * QtPropertyChangedNotificationAdapter::ConnectIfPossible ( QObject * _object, const std::string & _propertyName, IQtPropertyChangedReceiver * _receiver )
{
#if QT_VERSION >= 0x040500
      int propertyIndex = _object->metaObject()->indexOfProperty(_propertyName.c_str());
      QMetaProperty property ( _object->metaObject()->property(propertyIndex) );
      if ( property.isValid() && property.hasNotifySignal() )
      {
         QtPropertyChangedNotificationAdapter * ret = new QtPropertyChangedNotificationAdapter(_receiver);
         std::string methodName ( "2" );
         methodName += property.notifySignal().signature();

         QObject::connect ( _object, methodName.c_str(),
                            ret, SLOT(propertyChanged()) );
         return ret;
      }
      else
      {
         return NULL;
      }
#endif
}
