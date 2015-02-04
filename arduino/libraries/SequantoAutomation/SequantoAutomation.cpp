#include "SequantoAutomation.h"
#include <sequanto/automation.h>

#include "utility/src/sq_server.c"
#include "utility/src/sq_parser.c"
#include "utility/src/sq_stream_arduino.c"
#include "utility/src/sq_value.c"
#include "utility/src/sq_types.c"
#include "utility/src/sq_protocol.c"
#include "utility/src/sq_log.c"
#include "utility/src/sq_thread.c"
#include "utility/src/sq_automation.c"

static SQServer _server;

void SequantoAutomation::init()
{
    sq_init();
    sq_server_init ( &_server, 4321 );
}

void SequantoAutomation::poll()
{
    sq_server_poll ( &_server );
}
