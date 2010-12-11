#!/usr/bin/python

import sys
import os

if sys.platform == 'win32':
    os.system ( 'devenv /build libSequantoAutomation.sln' )
else:
    os.system ( 'make' )
