#include <QtGui>
#include <sequanto/automation.h>
#include <sequanto/QtWrapper.h>
#include "cxx_root.h"

using namespace std;
using namespace sequanto::automation;

const int ROW_COUNT = 10;
const int COLUMN_COUNT = 4;

int main ( int argc, char * argv[] )
{
   SQServer server;

   sq_init();
   sq_server_init ( &server, 4321 );
   
   ListNode * ui_root = new ListNode ( "ui" );
   sq_get_cxx_root()->AddChild ( ui_root );
   
   QApplication application ( argc, argv );
   
   QMainWindow mainWindow;
   QTableWidget * tableWidget = new QTableWidget();
   mainWindow.setCentralWidget ( tableWidget );
   tableWidget->setRowCount ( ROW_COUNT );
   tableWidget->setColumnCount ( COLUMN_COUNT );
   for ( int row = 0; row < ROW_COUNT; row++ )
   {
      for ( int column = 0; column < COLUMN_COUNT; column++ )
      {
         QTableWidgetItem * item = new QTableWidgetItem ( QString("%1, %2").arg(row).arg(column) );
         tableWidget->setItem(row, column, item);
      }
   }
   
   tableWidget->setRowHidden ( 0, true );
   tableWidget->setRowHidden ( 5, true );
   tableWidget->setRowHidden ( 6, true );
   tableWidget->setColumnHidden ( 1, true );
   
   mainWindow.show();
   
   QtWrapper::WrapApplication ( ui_root );
   
   sq_server_poll ( &server );
   
   atexit(sq_shutdown);
   
   return application.exec();
}
