#include <sequanto/ui.h>
#include <sequanto/log.h>
#include <sequanto/QtWidgetNode.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/QtWidgetAutomationEventFilter.h>
#include <sequanto/QtApplicationAutomationEventFilter.h>
#include <sequanto/QtAutomationGetPropertyEvent.h>
#include <sequanto/QtAutomationMoveEvent.h>
#include <sequanto/QtAutomationResizeEvent.h>
#include <sequanto/QtAutomationMouseMoveEvent.h>
#include <sequanto/QtAutomationMouseClickEvent.h>
#include <sequanto/QtPropertyChangedNotificationAdapter.h>
#include <sequanto/QtUnnamedObjectStore.h>
#include <sequanto/QtUiTypeProperty.h>
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
       
       QPoint pos = widget->mapToGlobal(QPoint(0,0));
       QPoint windowTopLeft ( window->geometry().topLeft() );
	
       pos -= windowTopLeft;

       return pos;
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

class QtStringProperty : public PropertyNode, public virtual IQtPropertyChangedReceiver
{
private:
   QObject * m_object;

protected:
   QtPropertyChangedNotificationAdapter * m_notifyAdapter;

   virtual const std::string & QtPropertyName () const
   {
      return GetName();
   }

public:
   QtStringProperty ( const std::string & _name, QObject * _object )
      : PropertyNode ( _name ),
        m_object ( _object ),
        m_notifyAdapter ( NULL )
   {
      m_notifyAdapter = QtPropertyChangedNotificationAdapter::ConnectIfPossible ( _object, QtPropertyName(), this );
   }

   virtual const NodeInfo & Info () const
   {
      return GetStringNodeInfo();
   }

   virtual void PropertyChanged ()
   {
      SendUpdate ();
   }

   virtual void HandleGet ( SQValue & _outputValue )
   {
     std::string value ( QtWrapper::ToString(QtWrapper::GetPropertyValue ( m_object, QtPropertyName() ).toString()) );
      sq_value_string_copy ( &_outputValue, value.c_str() );
   }

   virtual void HandleSet ( const SQValue * const _value )
   {
      QVariant variantValue ( _value->Value.m_stringValue );
      m_object->setProperty(QtPropertyName().c_str(), variantValue );
   }

   virtual ~QtStringProperty()
   {
      if ( m_notifyAdapter )
      {
         delete m_notifyAdapter;
         m_notifyAdapter = NULL;
      }
   }
};

class QtStringPropertyWithAlternateName : public QtStringProperty
{
private:
   std::string m_qtPropertyName;

protected:
   virtual const std::string & QtPropertyName() const
   {
      return m_qtPropertyName;
   }

public:
   QtStringPropertyWithAlternateName ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName )
      : QtStringProperty(_propertyName, _object)
   {
      init ( _propertyName, _object, _qtPropertyName, _qtPropertyName );
   }

   QtStringPropertyWithAlternateName ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName, const std::string & _qtPropertyToUseForNotification  )
      : QtStringProperty(_propertyName, _object)
   {
      init ( _propertyName, _object, _qtPropertyName, _qtPropertyToUseForNotification );
   }

   void init ( const std::string & _propertyName, QObject * _object, const std::string & _qtPropertyName, const std::string & _qtPropertyToUseForNotification )
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

   virtual ~QtStringPropertyWithAlternateName ()
   {
   }
};

class QtIntProperty : public IntegerPropertyNode, IQtPropertyChangedReceiver
{
private:
   QObject * m_object;
   QtPropertyChangedNotificationAdapter * m_notifyAdapter;

public:
   QtIntProperty ( const std::string & _name, QObject * _object )
      : IntegerPropertyNode ( _name ),
        m_object ( _object ),
        m_notifyAdapter ( NULL )
   {
      m_notifyAdapter = QtPropertyChangedNotificationAdapter::ConnectIfPossible ( _object, _name, this );
   }

   virtual void PropertyChanged ()
   {
      this->SendUpdate ();
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
       qDebug() << "    value is NOT valid.";
       
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
      if ( m_notifyAdapter )
      {
         delete m_notifyAdapter;
         m_notifyAdapter = NULL;
      }
   }
};

class QtScrollBarProperty : public QtIntProperty
{
private:
   QtWidgetNode * m_associatedScrollArea;
   bool m_hasSearchedForAssociatedScrollArea;
   
public:
   QtScrollBarProperty ( const std::string & _name, QObject * _object )
	   : QtIntProperty(_name, _object)
   {
      m_associatedScrollArea = NULL;
      m_hasSearchedForAssociatedScrollArea = false;
   }
   
   virtual void PropertyChanged ()
   {
      QtIntProperty::SendUpdate ();

      if ( !m_hasSearchedForAssociatedScrollArea )
      {
		  // Get my widget (which is my parent, since i am a property
         Node * parent = GetParent();
         if ( parent != NULL )
         {
            // My widget's parent (the parent widget's children list)
            parent = parent->GetParent();
            if ( parent != NULL )
            {
               // My actual parent (the QtWidgetNode instance)
               // For scrollbars on a QScrollArea this will be qt_scrollarea_vcontainer or qt_scrollarea_hcontainer
               parent = parent->GetParent();
               if ( parent != NULL )
               {
                  // Get widget list of actual parent (hopefully this is a QScrollArea).
                  parent = parent->GetParent();
                  if ( parent != NULL )
                  {
                     // Get actual parent
                     parent = parent->GetParent();
                     if ( parent != NULL )
                     {
                        QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(parent);
                        if ( widgetNode != NULL )
                        {
                           if ( widgetNode->type() == SQ_WIDGET_TYPE_SCROLL_AREA )
                           {
                              m_associatedScrollArea = widgetNode;
                           }
                        }
                     }
                  }
               }
            }
         }         
         m_hasSearchedForAssociatedScrollArea = true;
      }
      
      if ( m_associatedScrollArea != NULL )
      {
         m_associatedScrollArea->SendPositionUpdateForAllChildren();
      }
   }
};

class QtBooleanProperty : public BooleanPropertyNode, IQtPropertyChangedReceiver
{
private:
   QObject * m_object;
   QtPropertyChangedNotificationAdapter * m_notifyAdapter;
   
public:
   QtBooleanProperty ( const std::string & _name, QObject * _object )
      : BooleanPropertyNode ( _name ),
        m_object ( _object ),
        m_notifyAdapter ( NULL )
   {
      m_notifyAdapter = QtPropertyChangedNotificationAdapter::ConnectIfPossible ( _object, _name, this );
   }

   virtual void PropertyChanged ()
   {
      this->SendUpdate ();
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

   virtual ~QtBooleanProperty()
   {
      if ( m_notifyAdapter )
      {
         delete m_notifyAdapter;
         m_notifyAdapter = NULL;
      }
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
      SQ_UNUSED_PARAMETER(_newValue);
      
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
      SQ_UNUSED_PARAMETER(_newValue);
      
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
      SQ_UNUSED_PARAMETER(_newValue);
      
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
      SQ_UNUSED_PARAMETER(_newValue);
      
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

std::string QtWrapper::GetObjectName ( QObject * _object )
{
   QString objectName ( _object->objectName() );
   if ( objectName.isEmpty() )
	{
      return QtUnnamedObjectStore::GetName ( _object );
	}
   QByteArray value ( _object->objectName().toUtf8() );
   if ( !Node::IsValidName(value.constData(), value.length()) )
   {
      return QtUnnamedObjectStore::GetName ( _object );
   }
	else
	{
      return std::string(value.constData(), value.length() );
	}
}

void QtWrapper::Wrap ( ListNode * _root, QObject * _object )
{
   std::string name ( GetObjectName(_object) );
   _root->AddChild ( new ConstantStringNode("name", name ) );
   _root->AddChild ( new ConstantStringNode("className", _object->metaObject()->className() ) );

   for ( int i = 0; i < _object->metaObject()->propertyCount(); i++ )
   {
#if QT_VERSION >= 0x040500
      if ( _object->metaObject()->property(i).hasNotifySignal() )
      {
         std::string text ( "The " );
         text += _object->metaObject()->property(i).name();
         text += "property has a notify signal";
      }
#endif
      if ( !_root->HasChild ( _object->metaObject()->property(i).name() ) )
      {
         switch ( _object->metaObject()->property(i).type() )
         {
         case QVariant::String:
            _root->AddChild ( new QtStringProperty(_object->metaObject()->property(i).name(), _object ) );
            break;

         case QVariant::Int:
            _root->AddChild ( new QtIntProperty(_object->metaObject()->property(i).name(), _object ) );
            break;

         case QVariant::Bool:
            _root->AddChild ( new QtBooleanProperty(_object->metaObject()->property(i).name(), _object ) );
            break;

         default:
            break;
         }
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

         std::string childName ( GetObjectName(childObject) );
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
       assert ( _numberOfValues == 2 );
       
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
       assert ( _numberOfValues == 2 );
       
      int width = _inputValues[0].Value.m_integerValue;
      int height = _inputValues[1].Value.m_integerValue;

      QCoreApplication::postEvent ( m_widget, new QtAutomationResizeEvent(width, height) );

      sq_value_boolean ( &_output, SQ_TRUE );
   }

   virtual ~QtResizeMethod()
   {
   }
};

class QtInputMethod : public Node
{
public:
   QtInputMethod()
      : Node(SQ_UI_NODE_INPUT)
   {
   }

   virtual const NodeInfo & Info () const
   {
      static MethodInfo info ( VALUE_TYPE_VOID );
      if ( info.GetNumberOfParameters() == 0 )
      {
         info.AddParameter ( VALUE_TYPE_STRING );
      }
      return info;
   }
   
   virtual void HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
   {
      SQ_UNUSED_PARAMETER(_output);
      
       assert ( _numberOfValues == 1 );

       std::string text ( _inputValues[0].Value.m_stringValue );
       
       QtWidgetNode * parent = dynamic_cast<QtWidgetNode*> ( this->GetParent() );
       if ( parent != NULL )
       {
           QWidget * receiver = parent->widget();
           
           QApplication::postEvent ( receiver, new QKeyEvent( QEvent::KeyPress, Qt::Key_unknown, Qt::NoModifier, text.c_str(), false, text.length() ) );
           QApplication::postEvent ( receiver, new QKeyEvent( QEvent::KeyRelease, Qt::Key_unknown, Qt::NoModifier, text.c_str(), false, text.length() ) );
       }
   }

   virtual ~QtInputMethod()
   {
   }
};

class QtAutoGeneratedNameProperty : public ReadOnlyBooleanPropertyNode
{
public:
   QtAutoGeneratedNameProperty ()
      : ReadOnlyBooleanPropertyNode(SQ_UI_NODE_AUTO_GENERATED_NAME)
   {
   }
   
   virtual bool GetValue ()
   {
      QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(this->GetParent());
      if ( widgetNode != NULL )
      {
         QWidget * widget = widgetNode->widget();
         if ( widget != NULL )
         {
            return QtUnnamedObjectStore::IsKnown(widget);
         }
      }
      return false;
   }
};

class QtNativeIdProperty : public ReadOnlyPropertyNode
{
public:
   QtNativeIdProperty ()
      : ReadOnlyPropertyNode(SQ_UI_NODE_NATIVE_ID)
   {
   }
   
   virtual const NodeInfo & Info () const 
   {
      return GetReadOnlyByteArrayNodeInfo();
   }

   virtual void HandleGet ( SQValue & _value )
   {
      QtWidgetNode * widgetNode = dynamic_cast<QtWidgetNode*>(this->GetParent());
      if ( widgetNode != NULL )
      {
         widgetNode->HandleGetNativeId ( _value );
         return;
      }
      sq_value_const_byte_array ( &_value, NULL, 0 );
   }
};

void QtWrapper::WrapUi ( QtWidgetNode * _root, QWidget * _widget )
{
   if ( QtWrapper::IsWindow(_widget) )
   {
      assert ( _widget == _widget->window() );
      
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_WINDOW) );
      _root->AddChild ( new QtScreenXProperty( _widget ) );
      _root->AddChild ( new QtScreenYProperty( _widget ) );
   }
   else if ( _widget->inherits ( QCheckBox::staticMetaObject.className() ) )
   {
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_CHECK_BOX) );
      _root->AddChild ( new QtBooleanProperty(SQ_UI_NODE_CHECKED, _widget) );
      _root->AddChild ( new QtStringProperty(SQ_UI_NODE_TEXT, _widget) );
   }
   else if ( _widget->inherits ( QRadioButton::staticMetaObject.className() ) )
   {
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_RADIO_BUTTON) );
      _root->AddChild ( new QtBooleanProperty(SQ_UI_NODE_CHECKED, _widget) );
      _root->AddChild ( new QtStringProperty(SQ_UI_NODE_TEXT, _widget) );
   }
   else if ( _widget->inherits ( QLineEdit::staticMetaObject.className() ) )
   {
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_TEXT_BOX) );
      _root->AddChild ( new QtStringProperty(SQ_UI_NODE_TEXT, _widget) );
   }
   else if ( _widget->inherits ( QPlainTextEdit::staticMetaObject.className() ) )
   {
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_TEXT_BOX) );
      _root->AddChild ( new QtStringPropertyWithAlternateName(SQ_UI_NODE_TEXT, _widget, "plainText") );
   }
   else if ( _widget->inherits ( QTextEdit::staticMetaObject.className() ) )
   {
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_TEXT_BOX) );
      _root->AddChild ( new QtStringPropertyWithAlternateName(SQ_UI_NODE_TEXT, _widget, "plainText", "html") );
   }
   else if ( _widget->inherits ( QAbstractButton::staticMetaObject.className() ) )
   {
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_BUTTON) );
      _root->AddChild ( new QtStringProperty(SQ_UI_NODE_TEXT, _widget) );
   }
   else if ( _widget->inherits ( QMenuBar::staticMetaObject.className() ) )
   {
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_MENU_BAR) );
   }
   else if ( _widget->inherits ( QMenu::staticMetaObject.className() ) )
   {
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_MENU_ITEM) );
   }
   else if ( _widget->inherits ( QStatusBar::staticMetaObject.className() ) )
   {
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_STATUS_BAR) );
   }
   else if ( _widget->inherits ( QLayout::staticMetaObject.className() ) )
   {
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_CONTAINER) );
   }
   else if ( _widget->inherits ( QLabel::staticMetaObject.className() ) )
   {
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_LABEL) );
      _root->AddChild ( new QtStringProperty(SQ_UI_NODE_TEXT, _widget) );
   }
   else if ( _widget->inherits( QAbstractSlider::staticMetaObject.className() ) )
   {
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_SLIDER) );
      _root->AddChild ( new QtScrollBarProperty(SQ_UI_NODE_VALUE, _widget) );
      _root->AddChild ( new QtIntProperty(SQ_UI_NODE_MINIMUM, _widget) );
      _root->AddChild ( new QtIntProperty(SQ_UI_NODE_MAXIMUM, _widget) );
   }
   else if ( _widget->inherits ( QScrollArea::staticMetaObject.className() ) )
   {
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_SCROLL_AREA) );
   }
   else
   {
      _root->AddChild ( new QtUiTypeProperty(SQ_WIDGET_TYPE_WIDGET) );
   }
   _root->AddChild ( new ConstantStringNode ( SQ_UI_NODE_NATIVE_TYPE, _widget->metaObject()->className() ) );
   _root->AddChild ( new QtAutoGeneratedNameProperty() );
   _root->AddChild ( new QtNativeIdProperty() );
   
   _root->AddChild ( new QtGlobalXProperty( _widget ) );
   _root->AddChild ( new QtGlobalYProperty( _widget ) );
   
   _root->AddChild ( new QtIntProperty( SQ_UI_NODE_WIDTH, _widget ) );
   _root->AddChild ( new QtIntProperty( SQ_UI_NODE_HEIGHT, _widget ) );

   _root->AddChild ( new QtBooleanProperty( SQ_UI_NODE_ENABLED, _widget ) );
   _root->AddChild ( new QtBooleanProperty( SQ_UI_NODE_VISIBLE, _widget ) );

   _root->AddChild ( new QtMoveMethod(_widget ) );
   _root->AddChild ( new QtResizeMethod(_widget ) );
   
   _root->AddChild ( new QtInputMethod() );
   
   _root->AddChild ( new ListNode(SQ_UI_NODE_CHILDREN) );

   QObjectList list ( _widget->children() );
   if ( !list.empty() )
   {
      for ( int i = 0; i < list.count(); i++ )
      {
         QObject * childObject = list.at ( i );
         if ( childObject->isWidgetType() )
         {
			 QWidget * childWidget = qobject_cast<QWidget*>(childObject);
			 if ( !IsWindow(childWidget) )
			 {
				 _root->AddChildWidget ( childWidget );
			 }
         }
      }
   }
}

class QtActiveWindowProperty : public PropertyNode, public virtual IQtActiveWindowProperty
{
private:
	std::string m_previousActiveWindow;

	void InternalGet ( SQValue * _outputValue, bool _sendUpdateIfNeeded )
	{
		static std::string NO_ACTIVE_WINDOW ( "" );

		std::string newActiveWindow;
		QWidget * activeWindow = QApplication::activeWindow();
		if ( activeWindow == NULL )
		{
			newActiveWindow = NO_ACTIVE_WINDOW;
		}
		else
		{
			newActiveWindow = QtWrapper::GetObjectName(activeWindow);
		}

		if ( _outputValue != NULL )
		{
			sq_value_string_copy ( _outputValue, newActiveWindow.c_str() );
		}

		if ( newActiveWindow != m_previousActiveWindow )
		{
			m_previousActiveWindow = newActiveWindow;

         if ( _sendUpdateIfNeeded )
         {
            // Re-use _outputValue
            if( _outputValue != NULL )
            {
               PropertyNode::SendUpdate ( *_outputValue );
            }
            else
            {
               SQValue value;
               sq_value_init ( &value );
               sq_value_const_string ( &value, m_previousActiveWindow.c_str() );
               PropertyNode::SendUpdate ( value );
               sq_value_free ( &value );
            }
         }
		}
	}

public:
   QtActiveWindowProperty ()
      : PropertyNode ( SQ_UI_NODE_ACTIVE_WINDOW ),
	    m_previousActiveWindow ( "<NULL>" )
   {
   }

   virtual const NodeInfo & Info () const
   {
      return this->GetReadOnlyStringNodeInfo();
   }

   virtual void HandleGet ( SQValue & _outputValue )
   {
	   InternalGet ( &_outputValue, false );
   }

   virtual void HandleSet ( const SQValue * const _value )
   {
      SQ_UNUSED_PARAMETER(_value);
      
      throw std::runtime_error ( "Can not set the active window" );
   }

   virtual void TrySendUpdate ()
   {
	   InternalGet ( NULL, true );
   }
};

class QtMouseMoveMethod : public Node
{
public:
   QtMouseMoveMethod()
      : Node(SQ_UI_NODE_MOVE)
   {
   }

   virtual const NodeInfo & Info () const
   {
      static MethodInfo info ( VALUE_TYPE_VOID );
      if ( info.GetNumberOfParameters() == 0 )
      {
         info.AddParameter ( VALUE_TYPE_INTEGER );
         info.AddParameter ( VALUE_TYPE_INTEGER );
      }
      return info;
   }
   
   virtual void HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
   {
      SQ_UNUSED_PARAMETER(_output);
      
       assert ( _numberOfValues == 2 );
       
      int x = _inputValues[0].Value.m_integerValue;
      int y = _inputValues[1].Value.m_integerValue;

      QWidget * window = QApplication::activeWindow();
      if ( window != NULL )
      {
          QApplication::postEvent ( window, new QtAutomationMouseMoveEvent(x, y) );
      }
   }

   virtual ~QtMouseMoveMethod()
   {
   }
};

class QtMouseClickMethod : public Node
{
public:
   QtMouseClickMethod()
      : Node(SQ_UI_NODE_CLICK)
   {
   }

   virtual const NodeInfo & Info () const
   {
      static MethodInfo info ( VALUE_TYPE_VOID );
      if ( info.GetNumberOfParameters() == 0 )
      {
         info.AddParameter ( VALUE_TYPE_INTEGER );
         info.AddParameter ( VALUE_TYPE_INTEGER );
         info.AddParameter ( VALUE_TYPE_INTEGER );
      }
      return info;
   }
   
   virtual void HandleCall ( size_t _numberOfValues, const SQValue * const _inputValues, SQValue & _output )
   {
      SQ_UNUSED_PARAMETER(_output);
      
       assert ( _numberOfValues == 3 );
       
      int x = _inputValues[0].Value.m_integerValue;
      int y = _inputValues[1].Value.m_integerValue;
      int mouseButton = _inputValues[2].Value.m_integerValue;
      
      Qt::MouseButton button = Qt::NoButton;
      switch ( mouseButton )
      {
      case 0:
         button = Qt::LeftButton;
         break;

      case 1:
         button = Qt::RightButton;
         break;

      case 2:
         button = Qt::MidButton;
         break;
      }

      QWidget * window = QApplication::activeWindow();
      if ( window != NULL )
      {
          QApplication::postEvent ( window, new QtAutomationMouseClickEvent(x, y, button) );
      }
   }

   virtual ~QtMouseClickMethod()
   {
   }
};

ListNode * s_applicationRoot;

void QtWrapper::WrapApplication ( ListNode * _root )
{
    s_applicationRoot = _root;
    
   ListNode * windows = new ListNode ( SQ_UI_NODE_WINDOWS );
   _root->AddChild ( windows );
   QtActiveWindowProperty * activeWindow = new QtActiveWindowProperty();
   _root->AddChild ( activeWindow );

   UpdateWindows ( windows, activeWindow );

   QDesktopWidget * desktop = QApplication::desktop();
   ListNode * screen = new ListNode ( SQ_UI_NODE_SCREEN );
   _root->AddChild ( screen );
   screen->AddChild ( new ConstantIntegerNode(SQ_UI_NODE_WIDTH, desktop->screenGeometry ().width() ) );
   screen->AddChild ( new ConstantIntegerNode(SQ_UI_NODE_HEIGHT, desktop->screenGeometry ().height() ) );
   
   ListNode * mouse = new ListNode ( SQ_UI_NODE_MOUSE );
   _root->AddChild ( mouse );
   mouse->AddChild ( new QtMouseMoveMethod() );
   mouse->AddChild ( new QtMouseClickMethod() );

   QApplication::instance()->installEventFilter ( new QtApplicationAutomationEventFilter(windows, activeWindow, QApplication::instance()) );
}

bool QtWrapper::IsWindow ( QWidget * _widget )
{
   if ( _widget->isWindow() && _widget == _widget->window() ) //|| _widget->inherits(QMainWindow::staticMetaObject.className()) || _widget->inherits(QDialog::staticMetaObject.className()) )
   {
	  return true;
   }
   else
   {
      return false;
   }
}

bool QtWrapper::UpdateWindows()
{
   ListNode * windowsNode = dynamic_cast<ListNode*>(s_applicationRoot->FindChild ( SQ_UI_NODE_WINDOWS ));
   IQtActiveWindowProperty * activeWindowNode = dynamic_cast<IQtActiveWindowProperty*>(s_applicationRoot->FindChild ( SQ_UI_NODE_ACTIVE_WINDOW ));
   if ( windowsNode != NULL && activeWindowNode != NULL )
   {
      return UpdateWindows ( windowsNode, activeWindowNode );
   }
   else
   {
      return false;
   }
}

bool QtWrapper::UpdateWindows( ListNode * _windows, IQtActiveWindowProperty * _activeWindowNode )
{
   bool changed = false;
   foreach ( QWidget * widget, QApplication::allWidgets() )
   {
      if ( IsWindow(widget) )
      {
         std::string objectName ( GetObjectName(widget) );
         if ( !_windows->HasChild ( objectName ))
         {
            if ( QtUnnamedObjectStore::IsKnown ( widget ) )
            {
               std::string unnamedObjectName ( QtUnnamedObjectStore::GetName(widget) );
               if ( _windows->HasChild(unnamedObjectName) )
               {
                  _windows->RemoveChild ( unnamedObjectName );
                  QtUnnamedObjectStore::Deleted ( widget );
               }
            }
            QtWidgetNode * newWindow = new QtWidgetNode ( widget );
            WrapUi ( newWindow, widget );
            _windows->AddChild ( newWindow );

            changed = true;
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
         if ( GetObjectName(widget) == it->GetCurrent()->GetName() )
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
      changed = true;
   }

   if ( changed )
   {
	   _activeWindowNode->TrySendUpdate ();
   }
   return changed;
}

void QtWrapper::Log ( const QString & _message )
{
   std::string message ( ToString(_message) );
   sq_log ( message.c_str() );
}

QtWrapper::~QtWrapper()
{
}
