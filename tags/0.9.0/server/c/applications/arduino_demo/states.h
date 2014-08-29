#ifndef STATES_H_
#define STATES_H_

#include "statemanager.h"

boolean Idle( STATE_EVENT _event, char _ch, long _l, void **_pSave );
boolean Login( STATE_EVENT _event, char _ch, long _l, void **_pSave );
boolean LoginFailed( STATE_EVENT _event, char _ch, long _l, void **_pSave );
boolean MainMenu( STATE_EVENT _event, char _ch, long _l, void **_pSave );

#endif
