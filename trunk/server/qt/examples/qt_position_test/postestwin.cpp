#include "postestwin.h"

#include <iostream>
#include <QtGui>

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
   QApplication app ( _argc, _argv );
   
   PosTestWin mainWin;
   mainWin.show();

   return app.exec();
}
