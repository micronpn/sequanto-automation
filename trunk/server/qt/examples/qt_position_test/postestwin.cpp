#include "postestwin.h"

#include <iostream>
#include <QtGui>
#include <sequanto/automation.h>
#include <sequanto/QtWrapper.h>
#include <sequanto/listnode.h>
#include "cxx_root.h"

using namespace sequanto::automation;

PosTestWin::PosTestWin ( QWidget * _parent )
   : QMainWindow ( _parent )
{
   ui.setupUi ( this );

   QtPositionTestEventFilter * filter = new QtPositionTestEventFilter();

   connect(filter, SIGNAL(moved()), this, SLOT(buttonMoved()) );
   connect(filter, SIGNAL(resized()), this, SLOT(buttonResized()) );

   this->ui.m_button->installEventFilter ( filter );
}

void PosTestWin::buttonMoved()
{
   std::cout << "Button moved" << std::endl;
}

void PosTestWin::buttonResized()
{
   std::cout << "Button resized" << std::endl;
}

void PosTestWin::on_m_update_clicked()
{
   QPoint pos ( this->ui.m_button->pos() );

   this->ui.m_buttonX->setText ( QString("X = %1").arg( pos.x() ) );
   this->ui.m_buttonY->setText ( QString("Y = %1").arg( pos.y() ) );

   pos = this->ui.m_button->mapToGlobal ( pos );

   this->ui.m_buttonGlobalX->setText ( QString("Global X = %1").arg( pos.x() ) );
   this->ui.m_buttonGlobalY->setText ( QString("Global Y = %1").arg( pos.y() ) );

   pos = this->ui.m_button->mapTo ( this, this->ui.m_button->pos() );
   
   this->ui.m_buttonParentX->setText ( QString("Parent X = %1").arg( pos.x() ) );
   this->ui.m_buttonParentY->setText ( QString("Parent Y = %1").arg( pos.y() ) );
   
   //pos = qobject_cast<QWidget*>(this->ui.m_button->parent())->mapToGlobal ( this->ui.m_button->pos() );
   
   pos = this->ui.m_button->mapTo ( this->ui.m_button->window(), this->ui.m_button->pos() );
   
   this->ui.m_buttonWindowX->setText ( QString("Window X = %1").arg( pos.x() ) );
   this->ui.m_buttonWindowY->setText ( QString("Window Y = %1").arg( pos.y() ) );

   this->ui.enableButton->setEnabled ( !this->ui.enableButton->isEnabled() );
   this->ui.m_enableCheckBox->setEnabled ( !this->ui.enableButton->isEnabled() );
   this->ui.m_enableRadioButton->setEnabled ( !this->ui.enableButton->isEnabled() );
   this->ui.m_enableLineEdit->setEnabled ( !this->ui.enableButton->isEnabled() );

   this->ui.m_button->setVisible ( !this->ui.m_button->isVisible() );
}

QtPositionTestEventFilter::QtPositionTestEventFilter ()
      : QObject()
{
}

bool QtPositionTestEventFilter::eventFilter ( QObject * _object, QEvent * _event )
{
   switch ( _event->type() )
   {
   case QEvent::Move:
      moved();
      break;
      
   case QEvent::Resize:
      resized();
      break;
   }
   return QObject::eventFilter(_object, _event );
}

QtPositionTestEventFilter::~QtPositionTestEventFilter ()
{
}

int main ( int _argc, char * _argv[] )
{
   SQServer server;
   sq_init ();
   sq_server_init ( &server, 4321 );

   ListNode * ui = new ListNode ( "ui" );
   sq_get_cxx_root()->AddChild ( ui );

   QApplication app ( _argc, _argv );

   PosTestWin mainWin;
   mainWin.show();

   QtWrapper::WrapApplication ( ui );

   atexit ( sq_shutdown );

   sq_server_poll ( &server );

   return app.exec();
}
