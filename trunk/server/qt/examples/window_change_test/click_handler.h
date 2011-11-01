#include <QtGui>

class ClickHandler : public QObject
{
    Q_OBJECT
    
public slots:
    void showWindowA ();
    void showWindowB ();
    void showWindowC ();
};

class MyDialog : public QDialog
{
    Q_OBJECT
    
public:
    MyDialog ( QWidget * _parent );
    
};
