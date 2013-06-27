import sys
import subprocess
import os
from os import path
import shutil

DEVENV_8 = path.join ( os.getenv('ProgramFiles'), 'Microsoft Visual Studio 8', 'Common7', 'IDE', 'devenv.exe' )
DEVENV_9 = path.join ( os.getenv('ProgramFiles'), 'Microsoft Visual Studio 9.0', 'Common7', 'IDE', 'devenv.exe' )
DEVENV_11 = path.join ( os.getenv('ProgramFiles'), 'Microsoft Visual Studio 11.0', 'Common7', 'IDE', 'devenv.exe' )
CMAKE = path.join ( os.getenv('ProgramFiles'), 'CMake 2.8', 'bin', 'cmake.exe' )

if not path.exists(DEVENV_8):
    print 'Can not find VS8 (2005) devenv.exe file'
    print 'Looked at: %s' % DEVENV_8
    sys.exit(-1)

if not path.exists(DEVENV_9):
    print 'Can not find VS9 (2008) devenv.exe file'
    print 'Looked at: %s' % DEVENV_9
    sys.exit(-1)

if not path.exists(DEVENV_11):
    print 'Can not find VS11 (2012) devenv.exe file'
    print 'Looked at: %s' % DEVENV_12
    sys.exit(-1)

if not path.exists(CMAKE):
    print 'Can not find cmake.exe'
    print 'Looked at: %s' % CMAKE
    sys.exit(-1)

BUILD_DIR = 'releases'
CONFIGURATIONS = [
    #
    # Specials go first
    #

    # QMake machine automation
    {'generator': 'Visual Studio 9 2008',
     'name': 'qmake-machine-automation',
     'devenv': DEVENV_9,
     'configuration': 'Release',
     'defines': ['SQ_GENERATE_QMAKE:BOOL=ON',
                 'SQ_BUILD_SHARED_LIBRARIES:BOOL=ON',
                 'SQ_QT4:BOOL=ON',
                 'SQ_QT_MACHINE_AUTOMATION:BOOL=ON']}
    ,

    # Regular QMake
    {'generator': 'Visual Studio 9 2008',
     'name': 'qmake',
     'devenv': DEVENV_9,
     'configuration': 'Release',
     'defines': ['SQ_GENERATE_QMAKE:BOOL=ON',
                 'SQ_BUILD_SHARED_LIBRARIES:BOOL=ON',
                 'SQ_QT4:BOOL=ON']}
    ,
    
    #
    # Normal VS win32 configurations
    #
    
    # Visual Studio 2005
    {'generator': 'Visual Studio 8 2005',
     'name': 'vs2005-win32-no-qt',
     'devenv': DEVENV_8,
     'configuration': 'Release',
     'defines': ['SQ_BUILD_SHARED_LIBRARIES:BOOL=OFF']}
    ,
    {'generator': 'Visual Studio 8 2005',
     'name': 'vs2005-win32-qt',
     'devenv': DEVENV_8,
     'configuration': 'Release',
     'defines': ['SQ_BUILD_SHARED_LIBRARIES:BOOL=ON',
                 'SQ_QT4:BOOL=ON']}
    ,
    {'generator': 'Visual Studio 8 2005',
     'name': 'vs2005-win32-debug',
     'devenv': DEVENV_8,
     'configuration': 'Debug',
     'defines': ['SQ_BUILD_SHARED_LIBRARIES:BOOL=ON',
                 'SQ_QT4:BOOL=ON']}
    ,

    # Visual Studio 2008
    {'generator': 'Visual Studio 9 2008',
     'name': 'vs2008-win32-no-qt',
     'devenv': DEVENV_9,
     'configuration': 'Release',
     'defines': ['SQ_BUILD_SHARED_LIBRARIES:BOOL=OFF']}
    ,
    {'generator': 'Visual Studio 9 2008',
     'name': 'vs2008-win32-qt',
     'devenv': DEVENV_9,
     'configuration': 'Release',
     'defines': ['SQ_BUILD_SHARED_LIBRARIES:BOOL=ON',
                 'SQ_QT4:BOOL=ON']}
    ,
    {'generator': 'Visual Studio 9 2008',
     'name': 'vs2008-win32-debug',
     'devenv': DEVENV_9,
     'configuration': 'Debug',
     'defines': ['SQ_BUILD_SHARED_LIBRARIES:BOOL=ON',
                 'SQ_QT4:BOOL=ON']}
    ,

    # Visual Studio 2012
    {'generator': 'Visual Studio 11',
     'name': 'vs2012-win32-no-qt',
     'devenv': DEVENV_11,
     'configuration': 'Release',
     'defines': ['SQ_BUILD_SHARED_LIBRARIES:BOOL=OFF']}
    ,
    {'generator': 'Visual Studio 11',
     'name': 'vs2012-win32-qt',
     'devenv': DEVENV_11,
     'configuration': 'Release',
     'defines': ['SQ_BUILD_SHARED_LIBRARIES:BOOL=ON',
                 'SQ_QT4:BOOL=ON']}
    ,
    {'generator': 'Visual Studio 11',
     'name': 'vs2012-win32-debug',
     'devenv': DEVENV_11,
     'configuration': 'Debug',
     'defines': ['SQ_BUILD_SHARED_LIBRARIES:BOOL=ON',
                 'SQ_QT4:BOOL=ON']}
    ,
]

def run ( args, cwd = None, shell = False ):
    print 'Running "%s"' % args
    
    process = subprocess.Popen(args, stdout = None, stderr = subprocess.STDOUT, cwd = cwd, shell = shell )
    
    ret = process.wait()
    
    if ret != 0:
        print 'Process returned %i!' % ret
        sys.exit(-1)

for conf in CONFIGURATIONS:
    print 'Building', conf['name']
    build_dir = path.join ( BUILD_DIR, conf['name'] )
    if path.exists(build_dir):
        shutil.rmtree(build_dir)

    os.makedirs ( build_dir )

    args = [CMAKE,
            '-G', conf['generator'],
            '-D', 'SQ_GENERATE_DOCUMENTATION:BOOL=ON',
            '-D', 'CPACK_BINARY_ZIP:BOOL=ON',
            '-D', 'CPACK_BINARY_NSIS:BOOL=OFF',
            '-D', 'CPACK_SYSTEM_NAME:STRING=' + conf['name']]
    
    if 'defines' in conf:
        assert type(conf['defines']) == list
        for define in conf['defines']:
            args.append ( '-D' )
            args.append ( define )

    args.append ( '../..' )

    run ( args, cwd = build_dir )

    run ( [conf['devenv'],
           'libSequantoAutomation.sln',
           '/out', '..\\build-%s.log' % conf['name'],
           '/build', conf['configuration']], cwd = build_dir )

    run ( [conf['devenv'],
           'libSequantoAutomation.sln',
           '/out', '..\\package-%s.log' % conf['name'],
           '/build', conf['configuration'],
           '/project', 'PACKAGE'], cwd = build_dir )

    #run ( ['copy', 'libSequantoAutomation*.exe', '..'], cwd = build_dir, shell = True )
    run ( ['copy', 'libSequantoAutomation*.zip', '..'], cwd = build_dir, shell = True )

    if '--debug' not in sys.argv:
        shutil.rmtree(build_dir)
