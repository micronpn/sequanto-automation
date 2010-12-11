#!/usr/bin/python

import sys
import os

if sys.platform == 'win32':
    os.system ( 'devenv libSequantoAutomation.sln /build Debug' )
else:
    os.system ( 'make' )
