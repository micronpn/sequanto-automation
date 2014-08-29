#ifndef COMMON_H_
#define COMMON_H_

const int null = 0;

#include <sequanto/automation.h>
#include "wiring.h"
#include <string.h>
#include <stdio.h>
#include "LCD03.h"
#include "statemanager.h"
#include "states.h"

extern SQServer server;

extern char responsebuffer[40];

extern const int heaterPin;

#endif
