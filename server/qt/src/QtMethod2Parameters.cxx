#include <sequanto/QtMethod2Parameters.h>

using namespace sequanto::automation;

QtMethod2Parameters::QtMethod2Parameters ( const std::string & _name, QObject * _object, int _methodIndex )
   : Node(_name),
     m_object ( _object ),
     m_methodIndex ( _methodIndex )
{
   QMetaMethod method = _object->metaObject()->method(_methodIndex);

   Q_ASSERT ( method.methodType() == QMetaMethod::Method );

   QList<QByteArray> parameterTypes ( method.parameterTypes() );
   for ( int i = 0; i < parameterTypes.size(); i++ )
   {
      m_methodInfo.AddParameter ( ConvertType(parameterTypes[i]) );
   }
}

const NodeInfo & QtMethod2Parameters::Info () const
{
   return m_methodInfo;
}

bool QtMethod2Parameters::Valid ( const QMetaMethod & _method )
{
   QList<QByteArray> parameterTypes ( _method.parameterTypes() );
   for ( int i = 0; i < parameterTypes.size(); i++ )
   {
      if ( ConvertType(parameterTypes[i]) == VALUE_TYPE_NO_VALUE )
      {
         return false;
      }
   }
   return true;
}

SQValueType QtMethod2Parameters::ConvertType ( QByteArray _typeByteArray )
{
   static QByteArray intType = QMetaObject::normalizedType("int");
   static QByteArray boolType = QMetaObject::normalizedType("bool");
   static QByteArray stringType = QMetaObject::normalizedType("QString");

   if ( _typeByteArray == intType )
   {
      return VALUE_TYPE_INTEGER;
   }
   else if ( _typeByteArray == boolType )
   {
      return VALUE_TYPE_BOOLEAN;
   }
   else if ( _typeByteArray == stringType )
   {
      return VALUE_TYPE_STRING;
   }
   else
   {
      return VALUE_TYPE_NO_VALUE;
   }
}
