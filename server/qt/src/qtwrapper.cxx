#include <sequanto/ui.h>
#include <sequanto/qtwrapper.h>
#include <sequanto/qtautomationeventfilter.h>
#include <cassert>
#include <vector>

using namespace sequanto::automation;

QtWrapper::QtWrapper()
{
}

class QtStringProperty : public PropertyNode
{
private:
   QObject * m_object;
   std::string m_cached;

public:
   QtStringProperty ( const std::string & _name, QObject * _object )
      : PropertyNode ( _name ),
        m_object ( _object )
   {
   }

   virtual const NodeInfo & Info () const
   {
      return GetStringNodeInfo();
   }

   virtual void HandleGet ( SQValue & _outputValue )
   {
      std::string value ( QtWrapper::ToString(m_object->property(GetName().c_str()).toString() ) );
      sq_value_string_copy ( &_outputValue, value.c_str() );
   }

   virtual void HandleSet ( const SQValue * const _value )
   {
      QVariant variantValue ( _value->Value.m_stringValue );
      m_object->setProperty(GetName().c_str(), variantValue );
   }

   virtual ~QtStringProperty()
   {
   }
};

class QtIntProperty : public IntegerPropertyNode
{
private:
   QObject * m_object;

public:
   QtIntProperty ( const std::string & _name, QObject * _object )
      : IntegerPropertyNode ( _name ),
        m_object ( _object )
   {
   }

   virtual int GetValue ()
   {
      return m_object->property(GetName().c_str()).toInt();
   }

   virtual void SetValue ( int _newValue )
   {
      QVariant variantValue ( _newValue );
      m_object->setProperty(GetName().c_str(), variantValue );
   }

   virtual ~QtIntProperty()
   {
   }
};

class QtBoolProperty : public BooleanPropertyNode
{
private:
   QObject * m_object;

public:
   QtBoolProperty ( const std::string & _name, QObject * _object )
      : BooleanPropertyNode ( _name ),
        m_object ( _object )
   {
   }

   virtual bool GetValue ()
   {
      return m_object->property(GetName().c_str()).toBool();
   }

   virtual void SetValue ( bool _newValue )
   {
      QVariant variantValue ( _newValue );
      m_object->setProperty(GetName().c_str(), variantValue );
   }

   virtual ~QtBoolProperty()
   {
   }
};

class QtMethod2Parameters : public Node
{
private:
   QObject * m_object;
   MethodInfo m_methodInfo;
   int m_methodIndex;

public:
   QtMethod2Parameters ( const std::string & _name, QObject * _object, int _methodIndex )
      : Node(_name),
        m_object ( _object ),
        m_methodIndex ( _methodIndex )
   {
      QMetaMethod method = _object->metaObject()->method(_methodIndex);

      assert ( method.methodType() == QMetaMethod::Method );

      QList<QByteArray> parameterTypes ( method.parameterTypes() );
      for ( int i = 0; i < parameterTypes.size(); i++ )
      {
         m_methodInfo.AddParameter ( ConvertType(parameterTypes[i]) );
      }
   }

   virtual const NodeInfo & Info () const
   {
      return m_methodInfo;
   }

   static bool Valid ( const QMetaMethod & _method )
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

   static SQValueType ConvertType ( QByteArray _typeByteArray )
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
};

std::string QtWrapper::ToString ( const QString & _string )
{
   QByteArray value ( _string.toUtf8() );
   return std::string ( value.constData(), value.length() );
}

void QtWrapper::Wrap ( ListNode * _root, QObject * _object )
{
   std::string name ( ToString(_object->objectName()) );
   _root->AddChild ( new ConstantStringNode("name", name ) );
   _root->AddChild ( new ConstantStringNode("className", _object->metaObject()->className() ) );

   for ( int i = 0; i < _object->metaObject()->propertyCount(); i++ )
   {
      if ( _object->metaObject()->property(i).hasNotifySignal() )
      {
         std::string text ( "The " );
         text += _object->metaObject()->property(i).name();
         text += "property has a notify signal";
      }
      switch ( _object->metaObject()->property(i).type() )
      {
      case QVariant::String:
         _root->AddChild ( new QtStringProperty(_object->metaObject()->property(i).name(), _object ) );
         break;

      case QVariant::Int:
         _root->AddChild ( new QtIntProperty(_object->metaObject()->property(i).name(), _object ) );
         break;

      case QVariant::Bool:
         _root->AddChild ( new QtBoolProperty(_object->metaObject()->property(i).name(), _object ) );
         break;

      }
   }

   for ( int i = 0; i < _object->metaObject()->methodCount(); i++ )
   {
      int methodIndex = _object->metaObject()->methodOffset() + i;
      QMetaMethod method ( _object->metaObject()->method( methodIndex ) );
      if ( method.methodType() == QMetaMethod::Method )
      {
         if ( method.signature() != 0 )
         {
            if ( QtMethod2Parameters::Valid(method) )
            {
               if ( method.parameterTypes().size() == 2 )
               {
                  std::string methodName ( method.signature() );
                  size_t i = methodName.find("(");
                  methodName = methodName.substr(0, i);
                  if ( !_root->HasChild(methodName) )
                  {
                     _root->AddChild ( new QtMethod2Parameters(methodName, _object, methodIndex) );
                  }
               }
            }
         }
      }
   }

   QObjectList list ( _object->children() );
   if ( !list.empty() )
   {
      ListNode * children = new ListNode ( "children" );

      for ( int i = 0; i < list.count(); i++ )
      {
         QObject * childObject = list.at ( i );

         std::string childName (ToString(childObject->objectName()) );
         if ( !childName.empty() )
         {
            ListNode * child = new ListNode ( childName );
            Wrap ( child, childObject );
            children->AddChild ( child );
         }
      }
      _root->AddChild ( children );
   }
}

class QtMoveMethod : public Node
{
private:
   QWidget * m_widget;

public:
   QtMoveMethod( QWidget * _widget )
      : Node(SQ_UI_NODE_MOVE),
        m_widget ( _widget )
   {
   }

   virtual const NodeInfo & Info () const
   {
      static MethodInfo info ( VALUE_TYPE_BOOLEAN );
      if ( info.GetNumberOfParameters() == 0 )
      {
         info.AddParameter ( VALUE_TYPE_INTEGER );
         info.AddParameter ( VALUE_TYPE_INTEGER );
      }
      return info;
   }

   virtual void HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
   {
      int x = _inputValues[0].Value.m_integerValue;
      int y = _inputValues[1].Value.m_integerValue;

      QCoreApplication::postEvent ( m_widget, new QtAutomationMoveEvent(x, y) );

      sq_value_boolean ( &_output, SQ_TRUE );
   }

   virtual ~QtMoveMethod()
   {
   }
};

class QtResizeMethod : public Node
{
private:
   QWidget * m_widget;

public:
   QtResizeMethod( QWidget * _widget )
      : Node(SQ_UI_NODE_RESIZE),
        m_widget ( _widget )
   {
   }

   virtual const NodeInfo & Info () const
   {
      static MethodInfo info ( VALUE_TYPE_BOOLEAN );
      if ( info.GetNumberOfParameters() == 0 )
      {
         info.AddParameter ( VALUE_TYPE_INTEGER );
         info.AddParameter ( VALUE_TYPE_INTEGER );
      }
      return info;
   }

   virtual void HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
   {
      int width = _inputValues[0].Value.m_integerValue;
      int height = _inputValues[1].Value.m_integerValue;

      QCoreApplication::postEvent ( m_widget, new QtAutomationResizeEvent(width, height) );

      sq_value_boolean ( &_output, SQ_TRUE );
   }

   virtual ~QtResizeMethod()
   {
   }
};

void QtWrapper::WrapUi ( ListNode * _root, QWidget * _widget )
{
   if ( _widget->inherits ( QAbstractButton::staticMetaObject.className() ) )
   {
      _root->AddChild ( new ConstantStringNode(SQ_UI_NODE_TYPE, SQ_WIDGET_TYPE_BUTTON_STRING) );
   }
   else if ( _widget->inherits ( QDialog::staticMetaObject.className() ) || _widget->inherits ( QMainWindow::staticMetaObject.className() ) )
   {
      _root->AddChild ( new ConstantStringNode(SQ_UI_NODE_TYPE, SQ_WIDGET_TYPE_WINDOW_STRING) );
   }
   else if ( _widget->inherits ( QMenuBar::staticMetaObject.className() ) )
   {
      _root->AddChild ( new ConstantStringNode(SQ_UI_NODE_TYPE, SQ_WIDGET_TYPE_MENU_BAR_STRING) );
   }
   else if ( _widget->inherits ( QMenu::staticMetaObject.className() ) )
   {
      _root->AddChild ( new ConstantStringNode(SQ_UI_NODE_TYPE, SQ_WIDGET_TYPE_MENU_ITEM_STRING) );
   }
   else if ( _widget->inherits ( QLayout::staticMetaObject.className() ) )
   {
      _root->AddChild ( new ConstantStringNode(SQ_UI_NODE_TYPE, SQ_WIDGET_TYPE_CONTAINER_STRING) );
   }
   else if ( _widget->inherits ( QLabel::staticMetaObject.className() ) )
   {
      _root->AddChild ( new ConstantStringNode(SQ_UI_NODE_TYPE, SQ_WIDGET_TYPE_LABEL_STRING) );
   }
   else
   {
      _root->AddChild ( new ConstantStringNode(SQ_UI_NODE_TYPE, SQ_WIDGET_TYPE_WIDGET_STRING) );
   }
   _root->AddChild ( new ConstantStringNode ( SQ_UI_NODE_NATIVE_TYPE, _widget->metaObject()->className() ) );

   _root->AddChild ( new QtIntProperty( SQ_UI_NODE_X, _widget ) );
   _root->AddChild ( new QtIntProperty( SQ_UI_NODE_Y, _widget ) );
   _root->AddChild ( new QtIntProperty( SQ_UI_NODE_WIDTH, _widget ) );
   _root->AddChild ( new QtIntProperty( SQ_UI_NODE_HEIGHT, _widget ) );

   _root->AddChild ( new QtMoveMethod(_widget ) );
   _root->AddChild ( new QtResizeMethod(_widget ) );

   _widget->installEventFilter( new QtAutomationEventFilter(_root, _widget) );

   QObjectList list ( _widget->children() );
   if ( !list.empty() )
   {
      ListNode * children = new ListNode ( SQ_UI_NODE_CHILDREN );

      int unnamedObjects = 0;
      for ( int i = 0; i < list.count(); i++ )
      {
         QObject * childObject = list.at ( i );

         if ( childObject->isWidgetType() )
         {
            std::string childName (ToString(childObject->objectName()) );
            if ( !childName.empty() )
            {
               ListNode * child = new ListNode ( childName );
               WrapUi ( child, qobject_cast<QWidget*>(childObject) );
               children->AddChild ( child );
            }
            else
            {
               unnamedObjects ++;
            }
         }
      }
      if ( unnamedObjects > 0)
      {
         _root->AddChild ( new ConstantIntegerNode(SQ_UI_NODE_UNNAMED_OBJECTS, unnamedObjects) );
      }
      _root->AddChild ( children );
   }
}

QtWrapper::~QtWrapper()
{
}
