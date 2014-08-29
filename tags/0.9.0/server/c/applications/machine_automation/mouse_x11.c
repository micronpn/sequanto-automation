#include "config.h"

#include <memory.h>
#include <X11/X.h>
#include <X11/extensions/XTest.h>

void mouse_click_at ( int _x, int _y, int _button )
{
    Display *display = XOpenDisplay(NULL);
    XTestFakeMotionEvent(display, 0, _x, _y, 0);
    XFlush(display);
    XTestFakeButtonEvent(display, _button + 1, True, 0);
    XFlush(display);
    XTestFakeButtonEvent(display, _button + 1, False, 0);
    XFlush(display);
    XCloseDisplay(display);
}
