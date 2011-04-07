import sys
import subprocess
import os
from os import path
import shutil

DEVENV_8 = path.join ( os.getenv('ProgramFiles'), 'Microsoft Visual Studio 8', 'Common7', 'IDE', 'devenv.exe' )
VCEXPRESS_9 = path.join ( os.getenv('ProgramFiles'), 'Microsoft Visual Studio 9.0', 'Common7', 'IDE', 'VCExpress.exe' )

if not path.exists(DEVENV_8):
    print 'Can not find VS8 devenv.exe file'
    print 'Looked at: %s' % DEVENV_8
    sys.exit(-1)

if not path.exists(VCEXPRESS_9):
    print 'Can not find VC Express 9 devenv.exe file'
    print 'Looked at: %s' % VCEXPRESS_9
    sys.exit(-1)

BUILD_DIR = 'releases'
CONFIGURATIONS = [
    {'generator': 'Visual Studio 8 2005',
     'name': 'vs2005-win32',
     'devenv': DEVENV_8,
     'configuration': 'Release'}
    ,
    {'generator': 'Visual Studio 8 2005',
     'name': 'vs2005-win32-debug',
     'devenv': DEVENV_8,
     'configuration': 'Debug'}
    ,
#    {'generator': 'Visual Studio 8 2005 Win64',
#     'name': 'vs2005-win64',
#     'devenv': DEVENV_8}
#    ,
    {'generator': 'Visual Studio 9 2008',
     'name': 'vs2008-win32',
     'devenv': VCEXPRESS_9,
     'configuration': 'Release'}
    ,
    {'generator': 'Visual Studio 9 2008',
     'name': 'vs2008-win32-debug',
     'devenv': VCEXPRESS_9,
     'configuration': 'Debug'}
    ,
    {'generator': 'Visual Studio 9 2008',
     'name': 'qmake',
     'devenv': VCEXPRESS_9,
     'configuration': 'Release',
     'defines': ['SQ_GENERATE_QMAKE:BOOL=ON', 'SQ_BUILD_SHARED_LIBRARIES:BOOL=ON']}
    ,
#    {'generator': 'Visual Studio 9 2008 Win64',
#     'name': 'vs2008-win64'}
#    ,
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
    
    args = ['cmake',
           '-G', conf['generator'],
           '-D', 'SQ_GENERATE_DOCUMENTATION:BOOL=ON',
           '-D', 'SQ_QT4:BOOL=ON', 
           '-D', 'CPACK_BINARY_ZIP:BOOL=ON',
           '-D', 'CPACK_SYSTEM_NAME:STRING=' + conf['name']]
    if 'defines' in conf:
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
    
    run ( ['copy', 'libSequantoAutomation*.exe', '..'], cwd = build_dir, shell = True )
    run ( ['copy', 'libSequantoAutomation*.zip', '..'], cwd = build_dir, shell = True )
    
    if '--debug' not in sys.argv:
        shutil.rmtree(build_dir)
