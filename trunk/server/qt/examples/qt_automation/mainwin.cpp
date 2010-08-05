#include "mainwin.h"

#include <iostream>

MainWin::MainWin ( QWidget * _parent )
   : QMainWindow ( _parent )
{
   ui.setupUi ( this );
}

void MainWin::on_exitButton_clicked()
{
   this->close();
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
