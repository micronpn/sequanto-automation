#include <sequanto/ui.h>
#include <sequanto/qtwrapper.h>
#include <sequanto/qtautomationeventfilter.h>
#include <cassert>
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace sequanto::automation;

QtWrapper::QtWrapper()
{
}

QVariant QtWrapper::GetPropertyValue ( QObject * _object, const std::string & _propertyName )
{
  if ( _object->thread() == QThread::currentThread() )
  {
    if ( _propertyName == QtWrapper::screen_pos() )
    {
       QWidget * widget = qobject_cast<QWidget*>(_object);
       return widget->geometry().topLeft();
    }
    else if ( _propertyName == QtWrapper::global_pos() )
    {
       QWidget * widget = qobject_cast<QWidget*>(_object);
       QWidget * window = widget->window();
       //QPoint pos = window->geometry().topLeft();

       QPoint pos = widget->mapTo ( window, widget->pos() );
       qDebug() << "Direct: " << pos;
       return pos;
       /*
       qDebug() << widget->objectName();
       qDebug() << "   Global: " << widget->mapToGlobal(widget->pos());
       qDebug() << "   Window topleft: " << pos;
       pos = widget->mapToGlobal(widget->pos()) - pos;
       qDebug() << "   Global - Window: " << pos;
       */
       
       //return pos;
    }
    else
    {
      return _object->property(_propertyName.c_str());
    }
  }
  else
  {
     QtAutomationGetPropertyEvent * event = new QtAutomationGetPropertyEvent(_propertyName.c_str());
     QVariant value = event->wait ( _object );
     return value;
  }
}

const std::string & QtWrapper::screen_pos ()
{
  static std::string SCREEN_POS ( "__sq_screen" );
  return SCREEN_POS;
}

const std::string & QtWrapper::global_pos ()
{
  static std::string GLOBAL_POS ( "__sq_global" );
  return GLOBAL_POS;
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
     std::string value ( QtWrapper::ToString(QtWrapper::GetPropertyValue ( m_object, GetName() ).toString()) );
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
     assert ( m_object != NULL );
     
     QVariant value = QtWrapper::GetPropertyValue ( m_object, GetName() );
     if ( value.isValid() )
     {
       return value.toInt();
     }
     else
     {
       qDebug() << "    valid is NOT valid.";
       
       return -1;
     }
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
      return QtWrapper::GetPropertyValue ( m_object, GetName() ).toBool();
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


class QtScreenXProperty : public IntegerPropertyNode
{
private:
   QWidget * m_object;

public:
   QtScreenXProperty ( QWidget * _object )
      : IntegerPropertyNode ( SQ_UI_WINDOW_SCREEN_X ),
        m_object ( _object )
   {
   }

   virtual const NodeInfo & Info () const
   {
      return this->GetReadOnlyIntegerNodeInfo();
   }

   virtual int GetValue ()
   {
     return QtWrapper::GetPropertyValue ( m_object, QtWrapper::screen_pos() ).toPoint().x();
   }

   virtual void SetValue ( int _newValue )
   {
      throw std::runtime_error ( "Can not set the screenX value." );
   }

   virtual ~QtScreenXProperty()
   {
   }
};

class QtScreenYProperty : public IntegerPropertyNode
{
private:
   QWidget * m_object;

public:
   QtScreenYProperty ( QWidget * _object )
      : IntegerPropertyNode ( SQ_UI_WINDOW_SCREEN_Y ),
        m_object ( _object )
   {
   }

   virtual const NodeInfo & Info () const
   {
      return this->GetReadOnlyIntegerNodeInfo();
   }

   virtual int GetValue ()
   {
     return QtWrapper::GetPropertyValue ( m_object, QtWrapper::screen_pos() ).toPoint().y();
   }

   virtual void SetValue ( int _newValue )
   {
      throw std::runtime_error ( "Can not set the screenY value." );
   }

   virtual ~QtScreenYProperty()
   {
   }
};

class QtGlobalXProperty : public IntegerPropertyNode
{
private:
   QWidget * m_object;

public:
   QtGlobalXProperty ( QWidget * _object )
      : IntegerPropertyNode ( SQ_UI_NODE_X ),
        m_object ( _object )
   {
   }

   virtual const NodeInfo & Info () const
   {
      return this->GetReadOnlyIntegerNodeInfo();
   }

   virtual int GetValue ()
   {
     return QtWrapper::GetPropertyValue ( m_object, QtWrapper::global_pos() ).toPoint().x();
   }

   virtual void SetValue ( int _newValue )
   {
      throw std::runtime_error ( "Can not set the X value." );
   }

   virtual ~QtGlobalXProperty()
   {
   }
};

class QtGlobalYProperty : public IntegerPropertyNode
{
private:
   QWidget * m_object;

public:
   QtGlobalYProperty ( QWidget * _object )
      : IntegerPropertyNode ( SQ_UI_NODE_Y ),
        m_object ( _object )
   {
   }

   virtual const NodeInfo & Info () const
   {
      return this->GetReadOnlyIntegerNodeInfo();
   }

   virtual int GetValue ()
   {
     return QtWrapper::GetPropertyValue ( m_object, QtWrapper::global_pos() ).toPoint().y();
   }

   virtual void SetValue ( int _newValue )
   {
      throw std::runtime_error ( "Can not set the Y value." );
   }

   virtual ~QtGlobalYProperty()
   {
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
      _root->AddChild ( new QtScreenXProperty( _widget ) );
      _root->AddChild ( new QtScreenYProperty( _widget ) );
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
   
   /*
   if ( _widget->inherits ( QDialog::staticMetaObject.className() ) || _widget->inherits ( QMainWindow::staticMetaObject.className() ) )
   {
     _root->AddChild ( new ConstantIntegerNode(SQ_UI_NODE_X, 0 ) );
     _root->AddChild ( new ConstantIntegerNode(SQ_UI_NODE_Y, 0 ) );
   }
   else
   {
     _root->AddChild ( new QtIntProperty( SQ_UI_NODE_X, _widget ) );
     _root->AddChild ( new QtIntProperty( SQ_UI_NODE_Y, _widget ) );
   }
   */
   _root->AddChild ( new QtGlobalXProperty( _widget ) );
   _root->AddChild ( new QtGlobalYProperty( _widget ) );
   
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

class QtActiveWindowProperty : public PropertyNode
{
public:
   QtActiveWindowProperty ()
      : PropertyNode ( SQ_UI_NODE_ACTIVE_WINDOW )
   {
   }

   virtual const NodeInfo & Info () const
   {
      return this->GetReadOnlyStringNodeInfo();
   }

   virtual void HandleGet ( SQValue & _outputValue )
   {
      static std::string NO_ACTIVE_WINDOW ( "" );

      QWidget * activeWindow = QApplication::activeWindow();
      if ( activeWindow == NULL )
      {
         sq_value_string_copy ( &_outputValue, NO_ACTIVE_WINDOW.c_str() );
      }
      else
      {
         std::string value ( QtWrapper::ToString(activeWindow->objectName()) );
         sq_value_string_copy ( &_outputValue, value.c_str() );
      }
   }

   virtual void HandleSet ( const SQValue * const _value )
   {
      throw std::runtime_error ( "Can not set the active window" );
   }
};

void QtWrapper::WrapApplication ( ListNode * _root )
{
   ListNode * windows = new ListNode ( SQ_UI_NODE_WINDOWS );
   _root->AddChild ( windows );
   QtActiveWindowProperty * activeWindow = new QtActiveWindowProperty();
   _root->AddChild ( activeWindow );

   UpdateWindows ( windows );

   QApplication::instance()->installEventFilter ( new QtApplicationAutomationEventFilter(windows, activeWindow, QApplication::instance()) );
}

void QtWrapper::UpdateWindows( ListNode * _windows )
{
   foreach ( QWidget * widget, QApplication::topLevelWidgets() )
   {
      if ( !widget->objectName().isEmpty() )
      {
         if ( !_windows->HasChild ( ToString(widget->objectName()) ))
         {
            ListNode * newWindow = new ListNode ( ToString(widget->objectName()) );
            WrapUi ( newWindow, widget );
            _windows->AddChild ( newWindow );
         }
      }
   }
   
   ListNode::Iterator * it = _windows->ListChildren();
   std::vector<std::string> toBeRemoved;
   for ( ; it->HasNext(); it->Next() )
   {
      bool found = false;
      foreach ( QWidget * widget, QApplication::topLevelWidgets() )
      {
         if ( ToString(widget->objectName()) == it->GetCurrent()->GetName() )
         {
            found = true;
            break;
         }
      }
      if ( !found )
      {
         toBeRemoved.push_back ( it->GetCurrent()->GetName() );
      }
   }
   delete it;
   for ( std::vector<std::string>::const_iterator removeIt = toBeRemoved.begin(); removeIt != toBeRemoved.end(); removeIt++ )
   {
      _windows->RemoveChild ( *removeIt );
   }
}

QtWrapper::~QtWrapper()
{
}
