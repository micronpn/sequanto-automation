#!/usr/bin/python

import sys
import subprocess
import os
from os import path
import shutil

def call ( cmdline ):
    retcode = subprocess.call ( cmdline, shell = True )
    assert ( retcode == 0 )

ROOT = path.join ( os.getcwd(), 'build_qmake_test' )
if path.exists(ROOT):
    shutil.rmtree ( ROOT )

DIR = path.join(ROOT, 'qmake-machine-automation')
os.makedirs ( DIR )
os.chdir ( DIR )
call ( 'cmake -DSQ_GENERATE_QMAKE:BOOL=ON -DSQ_BUILD_SHARED_LIBRARIES:BOOL=ON -DSQ_QT4:BOOL=ON -DSQ_QT_MACHINE_AUTOMATION:BOOL=ON ../..' )
call ( 'make package -j 2' )
os.makedirs ( path.join(DIR, 'qmake_test', 'machine-automation') )
call ( 'tar -zxvf lib*.tar.gz -C qmake_test/machine-automation --strip 1' )
os.chdir ( path.join(DIR, 'qmake_test', 'machine-automation') )
call ( 'qmake' )
call ( 'make' )

DIR = path.join(ROOT, 'qmake')
os.makedirs ( DIR )
os.chdir ( DIR )
call ( 'cmake -DSQ_GENERATE_QMAKE:BOOL=ON -DSQ_BUILD_SHARED_LIBRARIES:BOOL=ON -DSQ_QT4:BOOL=ON ../..' )
call ( 'make package -j 2' )
os.makedirs ( path.join(DIR, 'qmake_test', 'SequantoAutomation') )
call ( 'tar -zxvf lib*.tar.gz -C qmake_test/SequantoAutomation --strip 1' )
os.chdir ( path.join(DIR, 'qmake_test', 'SequantoAutomation') )
call ( 'qmake' )
call ( 'make' )
