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
