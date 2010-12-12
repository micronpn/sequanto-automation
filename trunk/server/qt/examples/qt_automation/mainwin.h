#include <QStringListModel>
#include <QListWidgetItem>

#include "ui_mainwin.h"

class MainWin : public QMainWindow
{
   Q_OBJECT
   
public:
   MainWin ( QWidget * _parent = 0 );
   
   inline QObject * GetMenuBar ()
   {
      return ui.menubar;
   }

private slots:
   void on_exitButton_clicked();
   void on_m_loadFileButton_clicked();
   void on_actionExit_activated();
   void on_actionAdd_some_items_activated();
   void on_listView_currentItemChanged(  QListWidgetItem * _current, QListWidgetItem * _previous );
   void on_m_updateButton_clicked();
   void on_m_addLabel_clicked ();

private:
   Ui::MainWindow ui;
};
