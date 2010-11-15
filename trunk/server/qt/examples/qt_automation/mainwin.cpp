#include "mainwin.h"

#include <iostream>
#include <QtGui>

MainWin::MainWin ( QWidget * _parent )
   : QMainWindow ( _parent )
{
   ui.setupUi ( this );
}

void MainWin::on_exitButton_clicked()
{
   this->close();
}

void MainWin::on_m_loadFileButton_clicked()
{
   QMessageBox * messageBox = new QMessageBox( QMessageBox::Information, "About to open file", "I am not going to ask you which file to open", QMessageBox::Ok );
   messageBox->setModal ( true );
   messageBox->show();
   messageBox->deleteLater();

   QString filename = QFileDialog::getOpenFileName( this, "Open a file..." );
}

void MainWin::on_actionExit_activated()
{
   this->close();
}

void MainWin::on_actionAdd_some_items_activated()
{
   int count = 0;
   for ( int i = 0; i < 5; i++ )
   {
      ui.listView->addItem ( QString("Item %1").arg(count + i) );
   }
}

void MainWin::on_listView_currentItemChanged( QListWidgetItem * _current, QListWidgetItem * _previous )
{
   std::cout << _current->text().toStdString() << std::endl;
}

void MainWin::on_m_updateButton_clicked()
{
  QPoint windowTopLeft ( this->geometry().topLeft() );
  ui.m_windowGlobalPosition->setText ( QString("Global window pos: %1, %2").arg(windowTopLeft.x()).arg(windowTopLeft.y()) );
  
  QPoint pos = ui.m_updateButton->mapTo ( this, ui.m_updateButton->pos() );
  ui.m_buttonPositionMappedToWindow->setText ( QString("Button pos mapped to window: %1, %2").arg(pos.x()).arg(pos.y()) );

  pos = ui.m_updateButton->mapToGlobal(QPoint(0,0));
  ui.m_buttonPositionMappedToGlobal->setText ( QString("Button pos mapped to global: %1, %2").arg(pos.x()).arg(pos.y()) );
  
  pos -= windowTopLeft;
  ui.m_buttonPositionTranslated->setText ( QString("Button pos (translated): %1, %2").arg(pos.x()).arg(pos.y()) );
}

