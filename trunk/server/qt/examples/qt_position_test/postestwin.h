#include "ui_postestwin.h"

class PosTestWin : public QMainWindow
{
   Q_OBJECT
   
public:
   PosTestWin ( QWidget * _parent = 0 );
   
private slots:
   void buttonMoved ();
   void buttonResized ();
   void on_m_update_clicked();

private:
   Ui::PosTestWindow ui;
};

class QtPositionTestEventFilter : public QObject
{
   Q_OBJECT

signals:
   void moved ();
   void resized ();

public:
   QtPositionTestEventFilter ();
   bool eventFilter ( QObject * _obj, QEvent * _event );
   virtual ~QtPositionTestEventFilter ();
};
