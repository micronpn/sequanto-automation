#!/usr/bin/env python

import sys
import re
from os import path
import datetime
from StringIO import StringIO

if path.isdir ( path.join ( path.dirname(__file__), 'lib' ) ):
    sys.path.append ( path.join ( path.dirname(__file__), 'lib' ) )

code_dir = 'code'
if path.isdir ( path.join ( path.dirname(__file__), 'code' ) ):
    code_dir = path.join ( path.dirname(__file__), 'code' )

from sequanto_automation.codeparser import get as get_code_parser

class AutomationFile:
    def setErrorReportingFilename ( self, _filename ):
        self.m_errorReportingFilename = _filename
    
    def __init__ ( self ):
        self.m_parser = get_code_parser('C')
        self.m_imports = []
        self.m_properties = []
        self.m_functions = []
        self.m_name = None
        
        self.m_errorReportingFilename = '<unknown>'
        
        self.m_objectPaths = []
        self.m_foundProperties = []
        self.m_foundFunctions = []
        self.m_maxNumberOfParameters = 0
        
    def parse ( self, _input, _searchDirectory ):
        lineNumber = 1
        for line in _input.readlines():
            line = line.strip()
            if len(line) == 0:
                continue
            if line[0] == '#':
                continue
            
            command, rest = line.split(' ', 1)
            if command == 'name':
                name = rest.strip()
                if self.m_name is None:
                    self.m_name = name
            
            elif command == 'import':
                filename = path.join(_searchDirectory, rest.strip())
                self.m_imports.append ( (lineNumber, filename) )
                
            elif command == 'property':
                objectPath, get_function, set_function = None, None, None
                
                parms = rest.split(' ')
                if len(parms) == 3:
                    objectPath, get_function, set_function = parms
                elif len(parms) == 2:
                    objectPath, get_function = parms
                else:
                    raise 'property needs two or three parameters, the object path, the get function and optionally an set function'
                
                self.m_properties.append ( (lineNumber, objectPath, get_function, set_function) )

            elif command == 'function':
                objectPath, function = rest.split(' ')
                self.m_functions.append ( (lineNumber, objectPath, function) )
            
            lineNumber = lineNumber + 1

        if self.m_name is None:
            raise 'No name defined in the automation definition'
    
    def reportError ( self, _lineNumber, _text ):
        print '%s (%i) : error: %s' % (self.m_errorReportingFilename, _lineNumber, _text)
        
    def getAutomationType ( self, type ):
        if type == 'int':
            return 'integer'
        elif type in ['char *', 'char * const', 'const char *', 'const char * const']:
            return 'string'
        elif type == 'float':
            return 'float'
        elif type in ['bool', 'SQBool']:
            return 'boolean'
        elif type == 'void':
            return 'no_value'
        else:
            raise Exception('Could not resolve the C-type "%s" to an automation type.' % type)
    
    def createParents ( self, objectPath ):
        elements = objectPath.split('/')
        i = len(elements[1:]) - 1
        while i > 0:
            objectPath = '/'.join ( elements[:i + 1] )
            if (objectPath, 'INFO_TYPE_LIST', -1) not in self.m_objectPaths:
                self.m_objectPaths.append ( (objectPath, 'INFO_TYPE_LIST', -1) )
            
            i = i - 1
    
    def analyze ( self ):
        for lineNumber, filename in self.m_imports:
            if not path.exists(filename):
                self.reportError ( lineNumber, 'Could not find file named "%s"' % filename )
            else:
                fp = open ( filename, 'r' )
                self.m_parser.parse ( fp.read() )
                fp.close()
        
        propertyIndex = 0
        for lineNumber, objectPath, get_function, set_function in self.m_properties:
            if self.m_parser.hasFunction(get_function):
                if set_function is None or self.m_parser.hasFunction(set_function):
                    get_function = self.m_parser.getFunction(get_function)
                    if set_function is not None:
                        set_function = self.m_parser.getFunction(set_function)
                    
                    if len(get_function.parameters) == 0:
                        if set_function is None or len(set_function.parameters) == 1:
                            if set_function is None or get_function.returnType == set_function.parameters[0].type:
                                automationType = self.getAutomationType(get_function.returnType)
                                if automationType is not None:
                                    self.createParents ( objectPath )
                                    self.m_objectPaths.append ( (objectPath, 'INFO_TYPE_PROPERTY', propertyIndex) )
                                    self.m_foundProperties.append ( (objectPath, get_function, set_function) )
                                    propertyIndex += 1
                                    
                                else:
                                    self.reportError ( lineNumber, 'The property type is not recognized (%s)' % (get_function.returnType) )
                            else:
                                self.reportError ( lineNumber, 'The get function returns %s, while the set function expects %s' % (get_function.returnType, set_function.parameters[0].type) )
                        else:
                            self.reportError ( lineNumber, 'The set function takes %i parameters' % len(set_function.parameters) )
                    else:
                        self.reportError ( lineNumber, 'The get function takes %i parameters' % len(get_function.parameters) )                    
                else:
                    self.reportError ( lineNumber, 'Could not find set function "%s"' % set_function )
            else:
                self.reportError ( lineNumber, 'Could not find get function "%s"' % get_function )

        functionIndex = 0

        for lineNumber, objectPath, function in self.m_functions:
            if self.m_parser.hasFunction(function):
                function = self.m_parser.getFunction(function)
                automationType = self.getAutomationType(function.returnType)
                if automationType is not None:
                    allParmsOk = True
                    for parameter in function.parameters:
                        if self.getAutomationType(parameter.type) is None:
                            self.reportError ( lineNumber, 'The function takes an unknown parameter type "%s" for the parameter named "%s".' % (parameter.type, parameter.name) )
                            allParmsOk = False
                    
                    if allParmsOk:
                        self.createParents ( objectPath )
                        self.m_objectPaths.append ( (objectPath, 'INFO_TYPE_CALLABLE', functionIndex) )
                        self.m_foundFunctions.append ( function )
                        self.m_maxNumberOfParameters = max(self.m_maxNumberOfParameters, len(function.parameters))
                        functionIndex += 1
                    
                else:
                    self.reportError ( lineNumber, 'The return type is not recognized (%s)' % (function.returnType) )
            else:
                self.reportError ( lineNumber, 'Could not find function "%s"' % function )
        
        self.m_objectPaths.sort()
        
    def findObjectPathIndex ( self, objectPath ):
        ret = 0
        for oPath, type, index in self.m_objectPaths:
            if objectPath == oPath:
                return ret
            ret += 1
    
    def writeValue ( self, type, value ):
        automationType = self.getAutomationType(type)
        if automationType == 'String':
            return 'sq_protocol_write_string ( _stream, %s )' % value
        elif automationType == 'Integer':
            return 'sq_protocol_write_integer ( _stream, %s )' % value
        else:
            raise 'Unknown type %s' % automationType
    
    def generate ( self ):
        fp = open ( '%s_automation.c' % self.m_name, 'w' )
        fp.write ( '/*\n' )
        fp.write ( ' * DO NOT EDIT THIS FILE IT IS AUTO GENERATED!!!\n' )
        fp.write ( ' *\n' )
        fp.write ( ' * Generated by %s\n' % sys.argv[0] )
        fp.write ( ' *           on %s\n' % datetime.datetime.now() )
        fp.write ( ' */\n' )
        fp.write ( '\n' )
        fp.write ( '#include <string.h>\n' )
        fp.write ( '#include <sequanto/types.h>\n' )
        fp.write ( '#include <sequanto/stream.h>\n' )
        fp.write ( '#include <sequanto/value.h>\n' )
        fp.write ( '#include <sequanto/protocol.h>\n' )
        fp.write ( '#include <sequanto/server.h>\n' )
        fp.write ( '\n' )
        
        fp.write ( 'static const char NEWLINE[] SQ_CONST_VARIABLE = "\\r\\n";\n' )
        fp.write ( 'static const char UPDATE_START[] SQ_CONST_VARIABLE = "!UPDATE ";\n' )
        
        fp.write ( 'typedef enum _SQInfoType { INFO_TYPE_LIST = 0, INFO_TYPE_PROPERTY = 1, INFO_TYPE_CALLABLE = 2 } SQInfoType;\n' )
        
        fp.write ( 'typedef struct _SQInfo { const char * name; SQInfoType type; int index; } SQInfo;\n' )
        fp.write ( '\n' )

        fp.write ( 'static const char ROOT[] SQ_CONST_VARIABLE = "/";\n' )
        i = 0
        for objectPath, type, index in self.m_objectPaths:
            fp.write ( 'static const char NAME%i[] SQ_CONST_VARIABLE = "%s";\n' % (i, objectPath) )
            i += 1
        fp.write ( 'static const SQInfo INFO_LIST[] SQ_CONST_VARIABLE = {\n' )
        fp.write ( '                 { ROOT, INFO_TYPE_LIST, -1},\n' )
        i = 0
        for objectPath, type, index in self.m_objectPaths:
            fp.write ( '                 { NAME%i, %s, %s},\n' % (i, type, index) )
            i += 1
        fp.write ( '};' )
        fp.write ( '\n' )
        fp.write ( 'static const int NUMBER_OF_INFO  SQ_CONST_VARIABLE = %i;\n' % (len(self.m_objectPaths) + 1) )
        fp.write ( '\n' )
        
        for objectPath, get_function, set_function in self.m_foundProperties:
            translatedObjectPath = objectPath[1:].replace('/', '_')
            fp.write ( 'void sq_%s_updated ( SQStream * _stream, %s _value )\n' % (translatedObjectPath, get_function.returnType) )
            fp.write ( '{\n' )
            fp.write ( '   sq_stream_enter_write ( _stream );\n' )
            fp.write ( '   sq_stream_write_string ( _stream, sq_get_constant_string( UPDATE_START ) );\n' )
            fp.write ( '   sq_stream_write_string ( _stream, sq_get_constant_string( NAME%i ) );\n' % self.findObjectPathIndex(objectPath) )
            fp.write ( '   sq_stream_write_byte ( _stream, \' \' );\n' )
            fp.write ( '   sq_protocol_write_%s ( _stream, _value );\n' % self.getAutomationType(get_function.returnType) )
            fp.write ( '   sq_stream_write_string ( _stream, sq_get_constant_string( NEWLINE ) );\n' )
            fp.write ( '   sq_stream_exit_write ( _stream );\n' )
            fp.write ( '}\n' )
            fp.write ( '\n' )
            
            fp.write ( '%s %s ( void );\n' % (get_function.returnType, get_function.name) )
            fp.write ( 'void sq_generated_%s ( SQStream * _stream )\n' % get_function.name )
            fp.write ( '{\n' )
            fp.write ( '   %s value = %s();\n' % (get_function.returnType, get_function.name) )
            fp.write ( '   sq_protocol_write_%s ( _stream, value );\n' % self.getAutomationType(get_function.returnType) )
            fp.write ( '}\n' )
            fp.write ( '\n' )
            
            if set_function is not None:
                fp.write ( 'void %s ( %s %s );\n' % (set_function.name, set_function.parameters[0].type, set_function.parameters[0].name) )
                fp.write ( 'void sq_generated_%s ( const SQValue * const _value )\n' % set_function.name )
                fp.write ( '{\n' )
                fp.write ( '   %s ( _value->m_%sValue );\n' % (set_function.name, self.getAutomationType(get_function.returnType) ) )
                fp.write ( '}\n' )
                fp.write ( '\n' )
        
        fp.write ( 'typedef void (*SQPropertyGetFunction) ( SQStream * _stream );\n' )
        fp.write ( 'typedef void (*SQPropertySetFunction) ( const SQValue * const _value );\n' )
        fp.write ( 'typedef struct _SQPropertyInfo { SQPropertyGetFunction get; SQPropertySetFunction set; SQValueType type; } SQPropertyInfo;\n' )
        fp.write ( 'static const SQPropertyInfo PROPERTY_LIST[] SQ_CONST_VARIABLE = {\n' )
        # Some compilers (e.g. VS2005) does not support empty lists, so add a dummy one.
        if len(self.m_foundProperties) == 0:
            fp.write ( '   { NULL, NULL, VALUE_TYPE_NO_VALUE }\n' )
        else:
            first = True
            for objectPath, get_function, set_function in self.m_foundProperties:
                if first:
                    first = False
                else:
                    fp.write ( '   ,\n' )
                
                if set_function is None:
                    fp.write ( '   { sq_generated_%s, NULL, VALUE_TYPE_%s }\n' % (get_function.name, self.getAutomationType(get_function.returnType).upper()) )
                else:
                    fp.write ( '   { sq_generated_%s, sq_generated_%s, VALUE_TYPE_%s }\n' % (get_function.name, set_function.name, self.getAutomationType(get_function.returnType).upper()) )
        
        fp.write ( '};\n' )
        fp.write ( '\n' )

        for function in self.m_foundFunctions:
            if len(function.parameters) == 0:
                fp.write ( '%s %s ( void );\n' % (function.returnType, function.name) )
            else:
                fp.write ( '%s %s ( %s );\n' % (function.returnType, function.name, ', '.join(['%s %s' % (parm.type, parm.name) for parm in function.parameters])) )
            
            fp.write ( 'void sq_generated_%s ( SQStream * _stream, const SQValue * _inputValues )\n' % function.name )
            fp.write ( '{\n' )
            for index, parameter in enumerate(function.parameters):
                fp.write ( '   %s %s_parameter = _inputValues[%i].m_%sValue;\n' % (parameter.type, parameter.name, index, self.getAutomationType(parameter.type)) )
                index += 1
            if function.returnType == 'void':
                fp.write ( '   %s ( %s );\n' % (function.name, ', '.join(['%s_parameter' % parm.name for parm in function.parameters]) ) )
            else:
                fp.write ( '   %s ret = %s ( %s );\n' % (function.returnType, function.name, ', '.join(['%s_parameter' % parm.name for parm in function.parameters]) ) )
                fp.write ( '   sq_protocol_write_%s ( _stream, ret );\n' % self.getAutomationType(function.returnType) )
            
            fp.write ( '}\n' )
            fp.write ( '\n' )
        
        fp.write ( 'static const int NUMBER_OF_PARAMETERS SQ_CONST_VARIABLE = %i;\n' % self.m_maxNumberOfParameters )
        fp.write ( 'typedef void (*SQCallFunction) ( SQStream * _stream, const SQValue * const _inputValues );\n' )
        fp.write ( 'typedef struct _SQCallableInfo { SQCallFunction function; SQValueType returnType; %s } SQCallableInfo;\n' % ' '.join ( ['SQValueType parm%i;' % i for i in range(self.m_maxNumberOfParameters)]) )
        fp.write ( 'static const SQCallableInfo CALLABLE_LIST[] SQ_CONST_VARIABLE = {\n' )
        # Some compilers (e.g. VS2005) does not support empty lists, so add a dummy one.
        if len(self.m_foundFunctions) == 0:
            fp.write ( '   { NULL, VALUE_TYPE_NO_VALUE }\n' )
        else:
            for function in self.m_foundFunctions:
                fp.write ( '   { sq_generated_%s, VALUE_TYPE_%s' % (function.name, self.getAutomationType(function.returnType).upper()) )
                for parameter in function.parameters:
                    fp.write ( ', VALUE_TYPE_%s' % self.getAutomationType(parameter.type).upper() )
                fp.write ( ', VALUE_TYPE_NO_VALUE' * (self.m_maxNumberOfParameters - len(function.parameters) ) )
                fp.write ( ' },\n' )
        
        fp.write ( '};\n' )
        fp.write ( '\n' )
        
        fp.write ( 'SQValueType sq_automation_get_parameter ( const SQCallableInfo * const _callableInfo, int _parameterIndex )\n' )
        fp.write ( '{\n' )
        fp.write ( '   switch ( _parameterIndex )\n' )
        fp.write ( '   {\n' )
        for i in range(self.m_maxNumberOfParameters):
            fp.write ( '   case %i:\n' % i )
            fp.write ( '      return _callableInfo->parm%i;\n' % i )
        fp.write ( '   default:\n' )
        fp.write ( '      return VALUE_TYPE_NO_VALUE;\n' )
        fp.write ( '   }\n' )
        fp.write ( '}\n' )
        fp.write ( '\n' )
        
        automation_functions_c = open(path.join ( code_dir, 'c', 'automation_functions.c' ), 'r').read()
        fp.write ( automation_functions_c.replace('\r\n', '\n') )
        
        fp.close()

        fp = open ( '%s_automation.h' % self.m_name, 'w' )
        fp.write ( '#ifdef __cplusplus\n' )
        fp.write ( 'extern "C" {\n' )
        fp.write ( '#endif\n' )
        for objectPath, get_function, set_function in self.m_foundProperties:
            objectPath = objectPath[1:].replace('/', '_')
            fp.write ( 'void sq_%s_updated ( SQStream * _stream, %s _value );\n' % (objectPath, get_function.returnType) )
        fp.write ( '#ifdef __cplusplus\n' )
        fp.write ( '}\n' )
        fp.write ( '#endif\n' )
        fp.close()

automationFile = AutomationFile()
for arg in sys.argv[1:]:
    fp = open (arg, 'r')
    automationFile.setErrorReportingFilename ( arg )
    automationFile.parse ( fp, path.dirname(arg) )
    fp.close()
automationFile.analyze()
automationFile.generate ()
