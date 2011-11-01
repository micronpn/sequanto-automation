#include <QtGui>
#include <sequanto/automation.h>
#include <sequanto/QtWrapper.h>
#include "cxx_root.h"
#include "click_handler.moc"

using namespace std;
using namespace sequanto::automation;

QMessageBox * windowA = NULL;
MyDialog * windowB = NULL;

void ClickHandler::showWindowA ()
{
        if ( windowA == NULL )
        {
            windowA = new QMessageBox ( QMessageBox::Information, "A Window", "This the A window", QMessageBox::Close );
        }
        windowA->exec();
}

void ClickHandler::showWindowB ()
{
    if ( windowB == NULL )
    {
        windowB = new MyDialog ( QApplication::activeWindow() );
    }
    windowB->exec();
}

void ClickHandler::showWindowC ()
{
    delete windowB;
    windowB = new MyDialog ( QApplication::activeWindow() );
    windowB->exec();
}

MyDialog::MyDialog (QWidget * _parent)
    : QDialog(_parent)
{
    QVBoxLayout * vbox = new QVBoxLayout(this);
    QLabel * label = new QLabel ("MyDialog Window (B)");
    label->setObjectName ( "label" );
    QPushButton * button = new QPushButton ("OK");
    button->setObjectName ( "button" );
    vbox->addWidget ( label );
    vbox->addWidget ( button );
    connect ( button, SIGNAL(pressed()), this, SLOT(accept()) );
}

int main ( int argc, char * argv[] )
{
   SQServer server;

   sq_init();
   sq_server_init ( &server, 4321 );
   
   ListNode * ui_root = new ListNode ( "ui" );
   sq_get_cxx_root()->AddChild ( ui_root );
   
   QApplication application ( argc, argv );
   
   ClickHandler handler;
   QMainWindow mainWindow;
   QWidget * widget = new QWidget();
   QLayout * vbox = new QHBoxLayout();
   widget->setLayout ( vbox );
   
   // Button A
   QPushButton * button = new QPushButton("A");
   button->setObjectName ( "A" );
   QObject::connect ( button, SIGNAL(clicked()),
                      &handler, SLOT(showWindowA()) );
   vbox->addWidget ( button );

   // Button A
   button = new QPushButton("B");
   button->setObjectName ( "B" );
   QObject::connect ( button, SIGNAL(clicked()),
                      &handler, SLOT(showWindowB()) );
   vbox->addWidget ( button );

   // Button C
   button = new QPushButton("C");
   button->setObjectName ( "C" );
   QObject::connect ( button, SIGNAL(clicked()),
                      &handler, SLOT(showWindowC()) );
   vbox->addWidget ( button );
   
   mainWindow.setCentralWidget ( widget );
   mainWindow.show();
   
   QtWrapper::WrapApplication ( ui_root );
   
   sq_server_poll ( &server );
   
   atexit(sq_shutdown);
   
   return application.exec();
}
