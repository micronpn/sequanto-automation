#include <sequanto/qtautomationresizeevent.h>

using namespace sequanto::automation;

const int QtAutomationResizeEvent::ID = QEvent::registerEventType();

QtAutomationResizeEvent::QtAutomationResizeEvent ( int _width, int _height )
    : QEvent( (QEvent::Type) ID),
      m_size(_width, _height)
{
}

const QSize & QtAutomationResizeEvent::size()
{
    return m_size;
}

QtAutomationResizeEvent::~QtAutomationResizeEvent()
{
}

