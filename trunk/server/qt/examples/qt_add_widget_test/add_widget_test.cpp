#include <QtGui>
#include <sequanto/automation.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/QtUnnamedObjectStore.h>

using namespace std;
using namespace sequanto::automation;

static int CREATE_WINDOW_EVENT = QEvent::registerEventType();
static int REMOVE_WINDOW_EVENT = QEvent::registerEventType();
static int ADD_WIDGET_EVENT = QEvent::registerEventType();
static int REMOVE_WIDGET_EVENT = QEvent::registerEventType();
static int OPEN_DIALOG_EVENT = QEvent::registerEventType();
static int OPEN_CHILD_DIALOG_EVENT = QEvent::registerEventType();

class Commander : public QObject
{
private:
   int m_nextWindowNumber;
   int m_nextButtonNumber;

public:
   Commander ( QObject * _parent )
      : QObject(_parent)
   {
      m_nextWindowNumber = 1;
      m_nextButtonNumber = 1;
   }

   virtual bool event ( QEvent * _event )
   {
      if ( _event->type() == CREATE_WINDOW_EVENT )
      {
         QMainWindow * window = new QMainWindow();
         QString name = QString("Window_%1").arg(m_nextWindowNumber);
         //window->setObjectName ( name );
         window->setWindowTitle ( name );
         window->show();
         QWidget * centralWidget = new QWidget();
         //centralWidget->setObjectName ( "centralWidget" );
         QVBoxLayout * layout = new QVBoxLayout ();
         //layout->setObjectName ( "layout" );
         centralWidget->setLayout ( layout );
         window->setCentralWidget ( centralWidget );
         m_nextWindowNumber ++;
         return true;
      }
      else if ( _event->type() == REMOVE_WINDOW_EVENT )
      {
         QWidgetList topLevelWidgets ( QApplication::topLevelWidgets() );
         if ( topLevelWidgets.size() > 0 )
         {
            QMainWindow * window = qobject_cast<QMainWindow*>(topLevelWidgets[0]);
            window->deleteLater();
         }
         return true;
      }
      else if ( _event->type() == ADD_WIDGET_EVENT )
      {
         QWidgetList topLevelWidgets ( QApplication::topLevelWidgets() );
         if ( topLevelWidgets.size() > 0 )
         {
            QMainWindow * window = qobject_cast<QMainWindow*>(topLevelWidgets[0]);
            QPushButton * button = new QPushButton( QString("button_%1").arg(m_nextButtonNumber) );
            //button->setObjectName ( QString::QString("button_%1").arg(m_nextButtonNumber) );
            window->centralWidget()->layout()->addWidget ( button );
            m_nextButtonNumber ++;
         }
         return true;
      }
      else if ( _event->type() == REMOVE_WIDGET_EVENT )
      {
         QWidgetList topLevelWidgets ( QApplication::topLevelWidgets() );
         if ( topLevelWidgets.size() > 0 )
         {
            QMainWindow * window = qobject_cast<QMainWindow*>(topLevelWidgets[0]);
            if ( window->centralWidget()->layout()->count() > 0 )
            {
               QLayoutItem * item = window->centralWidget()->layout()->itemAt ( 0 );
               window->centralWidget()->layout()->removeItem ( item );
               delete item->widget();
               delete item;
            }
         }
         return true;
      }
      else if ( _event->type() == OPEN_DIALOG_EVENT )
      {
         QDialog * dialog = new QDialog();
         QString name ( QString("Dialog_%1").arg(m_nextWindowNumber) );
         dialog->setObjectName ( name );
         dialog->setWindowTitle ( name );
         dialog->show();
         m_nextWindowNumber ++;
         return true;
      }
      else if ( _event->type() == OPEN_CHILD_DIALOG_EVENT )
      {
         QWidgetList topLevelWidgets ( QApplication::topLevelWidgets() );
         if ( topLevelWidgets.size() > 0 )
         {
            QMainWindow * window = qobject_cast<QMainWindow*>(topLevelWidgets[0]);
            QDialog * dialog = new QDialog(window);
            QString name ( QString("ChildDialog_%1").arg(m_nextWindowNumber) );
            dialog->setObjectName ( name );
            dialog->setWindowTitle ( name );
            dialog->show();
            m_nextWindowNumber ++;
         }
         return true;
      }
      else
      {
         return false;
      }
   }
};

static Commander * s_commander;

void create_window_using_commander ()
{
   QApplication::postEvent ( s_commander, new QEvent( (QEvent::Type) CREATE_WINDOW_EVENT ) );
}

void remove_window_using_commander ()
{
   QApplication::postEvent ( s_commander, new QEvent ( (QEvent::Type) REMOVE_WINDOW_EVENT ) );
}

void add_widget_using_commander ()
{
   QApplication::postEvent ( s_commander, new QEvent( (QEvent::Type) ADD_WIDGET_EVENT ) );
}

void remove_widget_using_commander ()
{
   QApplication::postEvent ( s_commander, new QEvent ( (QEvent::Type) REMOVE_WIDGET_EVENT ) );
}

void open_dialog_using_commander ()
{
   QApplication::postEvent ( s_commander, new QEvent( (QEvent::Type) OPEN_DIALOG_EVENT ) );
}

void open_child_dialog_using_commander ()
{
   QApplication::postEvent ( s_commander, new QEvent( (QEvent::Type) OPEN_CHILD_DIALOG_EVENT ) );
}

void quit_using_commander ()
{
   QApplication::quit();
}

static Tree ui_tree;

extern "C"
{
   SQBool sq_handle_branch_info_ui ( SQStream * _stream, const char * _objectPath )
   {
      return ui_tree.HandleInfo ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_list_ui ( SQStream * _stream, const char * _objectPath )
   {
      return ui_tree.HandleList ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_get_ui ( SQStream * _stream, const char * _objectPath )
   {
      return ui_tree.HandleGet ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_set_ui ( SQStream * _stream, const char * _objectPath, const SQValue * const _value )
   {
      return ui_tree.HandleSet ( _stream, _objectPath, _value );
   }

   SQBool sq_handle_branch_disable_ui ( SQStream * _stream, const char * _objectPath )
   {
      return ui_tree.HandleDisable ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_enable_ui ( SQStream * _stream, const char * _objectPath )
   {
      return ui_tree.HandleEnable ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_call_ui ( SQStream * _stream, const char * _objectPath, const SQValue * const _values, int _numberOfValues )
   {
      return ui_tree.HandleCall ( _stream, _objectPath, _values, _numberOfValues );
   }

   SQBool sq_handle_branch_dump_ui ( SQStream * _stream, const char * _objectPath )
   {
      return ui_tree.HandleDump ( _stream, _objectPath );
   }

   void create_window ()
   {
      create_window_using_commander ();
   }

   void remove_window ()
   {
      remove_window_using_commander ();
   }

   void add_widget ()
   {
      add_widget_using_commander ();
   }

   void remove_widget ()
   {
      remove_widget_using_commander ();
   }

   void open_dialog ()
   {
      open_dialog_using_commander ();
   }

   void open_child_dialog ()
   {
      open_child_dialog_using_commander ();
   }
   
   void quit ()
   {
      quit_using_commander();
   }

   int number_of_unnamed_objects ()
   {
      return (int) QtUnnamedObjectStore::CachedObjects();
   }
}

int main ( int argc, char * argv[] )
{
   SQServer server;

   sq_init();
   sq_server_init ( &server, 4321 );
   
   ListNode * ui_root = new ListNode ( "ui" );
   ui_tree.SetRoot ( ui_root );
   
   QApplication application ( argc, argv );

   s_commander = new Commander ( &application );

   QtWrapper::WrapApplication ( ui_root );
   
   sq_server_poll ( &server );
   
   atexit(sq_shutdown);
   
   return application.exec();
}
