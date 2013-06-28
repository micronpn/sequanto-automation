#!/usr/bin/python

import sys
import subprocess
import os
from os import path
import shutil

def call ( cmdline ):
    retcode = subprocess.call ( cmdline, shell = True )
    assert ( retcode == 0 )

CMAKE = 'cmake'
GENERATORARGS = ''
MAKE = 'make -j 2 '
UNZIP = 'unzip'
MV = 'mv'
if sys.platform == 'win32':
    CMAKE = path.join ( os.getenv('ProgramFiles'), 'CMake 2.8', 'bin', 'cmake.exe' )
    if not path.exists(CMAKE):
        print 'Can not find cmake:', CMAKE
        sys.exit(-1)
    CMAKE = '"%s"' % CMAKE
    
    GENERATORARGS = '-G "NMake Makefiles"'
    MAKE = 'nmake'
    
    UNZIP = path.join ( 'C:\\Program Files', '7-zip', '7z.exe' )
    if not path.exists(UNZIP):
        print 'Can not find unzip:', UNZIP
        sys.exit(-1)
    UNZIP = '"%s" x ' % UNZIP
    MV = 'ren'

ROOT = path.join ( os.getcwd(), 'build_qmake_test' )
if path.exists(ROOT):
    shutil.rmtree ( ROOT )

DIR = path.join(ROOT, 'qmake-machine-automation')
os.makedirs ( DIR )
os.chdir ( DIR )
call ( CMAKE + ' -DSQ_GENERATE_QMAKE:BOOL=ON -DSQ_BUILD_SHARED_LIBRARIES:BOOL=ON -DSQ_QT4:BOOL=ON -DSQ_QT_MACHINE_AUTOMATION:BOOL=ON -DCPACK_BINARY_NSIS:BOOL=OFF -DCPACK_BINARY_ZIP:BOOL=ON ' + GENERATORARGS + ' ../..' )
call ( MAKE + ' package' )
os.makedirs ( path.join(DIR, 'qmake_test' ) )
os.chdir ( path.join(DIR, 'qmake_test' ) )
call ( UNZIP + '..\*.zip' )
filename = os.listdir('.')[-1]
call ( MV + ' ' + filename + ' machine-automation' )
os.chdir ( path.join(DIR, 'qmake_test', 'machine-automation' ) )
call ( 'qmake' )
call ( MAKE )

DIR = path.join(ROOT, 'qmake')
os.makedirs ( DIR )
os.chdir ( DIR )
call ( CMAKE + ' -DSQ_GENERATE_QMAKE:BOOL=ON -DSQ_BUILD_SHARED_LIBRARIES:BOOL=ON -DSQ_QT4:BOOL=ON -DCPACK_BINARY_NSIS:BOOL=OFF -DCPACK_BINARY_ZIP:BOOL=ON ' + GENERATORARGS + ' ../..' )
call ( MAKE + ' package' )
os.makedirs ( path.join(DIR, 'qmake_test' ) )
os.chdir ( path.join(DIR, 'qmake_test' ) )
call ( UNZIP + '..\*.zip' )
filename = os.listdir('.')[-1]
call ( MV + ' ' + filename + ' SequantoAutomation' )
os.chdir ( path.join(DIR, 'qmake_test', 'SequantoAutomation' ) )
call ( 'qmake' )
call ( MAKE )
