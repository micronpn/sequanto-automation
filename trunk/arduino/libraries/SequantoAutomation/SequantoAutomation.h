
#ifndef _ARDUINO_SEQUANTO_AUTOMATION_H
#define _ARDUINO_SEQUANTO_AUTOMATION_H

#define SQ_ARDUINO
#include "utility/src/config.h"

class SequantoAutomation
{
private:
    SequantoAutomation();

public:
    static void init();
    static void poll();
};

#endif // _ARDUINO_SEQUANTO_AUTOMATION_H
