#!/usr/bin/env python

import sys
import re
import types
import sets
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
                    raise 'property needs two or three parameters, the object path, the get function and optionally a set function'
                
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
        if type in ['unsigned long', 'long', 'signed long', 'unsigned int', 'int', 'signed int', 'unsigned short', 'short', 'signed short', 'unsigned char', 'char', 'signed char']:
            return 'integer'
        elif type in ['char *', 'char * const', 'const char *', 'const char * const', 'SQStringOut', 'SQStringOut *']:
            return 'string'
        elif type in ['float', 'double']:
            return 'float'
        elif type in ['bool', 'SQBool']:
            return 'boolean'
        elif type == 'void':
            return 'void'
        elif type == 'SQByteArray *':
            return 'byte_array'
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
    
    def isSmartObjectPath ( self, _objectPath ):
        return _objectPath.find('(') != -1
    
    def numberOfAdditionalSmartParameters ( self, _objectPath ):
        return len([element for element in _objectPath.split ( '/' ) if len(element) > 1 and element[0] == '(' and element[-1] == ')'])
    
    def normalizeSmartObjectPath ( self, _objectPath ):
        ret = ''
        for element in _objectPath.split ( '/' ):
            if len(element) > 1 and element[0] == '(' and element[-1] == ')':
                ret += '/%s'
            elif element == '':
                pass
            else:
                ret += '/%s' % element
        return ret
    
    def listAllSmartObjectPaths ( self, _objectPath ):
        elements = _objectPath.split ( '/' )[1:]
        metaPath = [None] * len(elements)
        
        i = 0
        for element in elements:
            if len(element) > 1 and element[0] == '(' and element[-1] == ')':
                element = element[1:-1]
                j = element.find ( '..' )
                if j != -1:
                    metaPath[i] = [str(a) for a in range(int(element[0:j]), int(element[j+2:]) + 1)]
                else:
                    metaPath[i] = [a.strip() for a in element.split(',')]
            else:
                metaPath[i] = element
            
            i += 1
            
        return self.generateSmartObjectPaths ( metaPath, 0 )
    
    def generateSmartObjectPaths ( self, metaPath, index ):
        if len(metaPath) == index:
            yield '', []
        
        else:
            if type(metaPath[index]) in types.StringTypes:
                for rest, values in self.generateSmartObjectPaths ( metaPath, index + 1 ):
                    yield '/' + metaPath[index] + rest, values
            else:
                for start in metaPath[index]:
                    for rest, values in self.generateSmartObjectPaths ( metaPath, index + 1 ):
                        yield  '/' + str(start) + rest, [start] + values[:]
    
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
                    
                    numAdditionalSmartParameters = 0
                    if self.isSmartObjectPath(objectPath):
                        numAdditionalSmartParameters = self.numberOfAdditionalSmartParameters ( objectPath )
                    
                    if len(get_function.parameters) == numAdditionalSmartParameters:
                        if set_function is None or len(set_function.parameters) == 1 + numAdditionalSmartParameters:
                            if set_function is None or get_function.returnType == set_function.parameters[0 + numAdditionalSmartParameters].type:
                                automationType = self.getAutomationType(get_function.returnType)
                                if automationType is not None:

                                    if self.isSmartObjectPath(objectPath):
                                        first = True
                                        for smartObjectPath, values in self.listAllSmartObjectPaths ( objectPath ):
                                            self.createParents ( smartObjectPath )
                                            self.m_objectPaths.append ( (smartObjectPath, 'INFO_TYPE_PROPERTY', propertyIndex) )
                                            self.m_foundProperties.append ( (smartObjectPath, get_function, set_function, values, self.normalizeSmartObjectPath(objectPath), first) )
                                            propertyIndex += 1
                                            first = False
                                    else:
                                        self.createParents ( objectPath )
                                        self.m_objectPaths.append ( (objectPath, 'INFO_TYPE_PROPERTY', propertyIndex) )
                                        self.m_foundProperties.append ( (objectPath, get_function, set_function, None, None, None) )
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
        
        seen_update_functions = sets.Set()
        for objectPath, get_function, set_function, smart_values, normalize_object_path, first_smart_property in self.m_foundProperties:
            translatedObjectPath = objectPath[1:].replace('/', '_')
            if normalize_object_path is not None:
                translatedObjectPath = normalize_object_path[1:].replace('/', '_').replace('_%s', '')
            
            write_update_function = True
            if first_smart_property is not None and not first_smart_property:
                write_update_function = False
            
            additional_smart_name = ''
            additional_smart_parameters = ''
            base_function_name = 'sq_%s_updated' % translatedObjectPath
            function_name = base_function_name
            if write_update_function:
                i = 2
                while function_name in seen_update_functions:
                    function_name = '%s%i' % (base_function_name, i)
                    i += 1
                seen_update_functions.add ( function_name )
            
            if smart_values is not None:
                additional_smart_name = '_' + '_'.join(smart_values)
                additional_smart_parameters = ', '.join ( ['%s %s' % (parameter.type, parameter.name) for parameter in get_function.parameters] )
                if write_update_function:
                    fp.write ( 'void %s ( SQStream * _stream, %s, %s _value )\n' % (function_name, additional_smart_parameters, get_function.returnType) )
            else:
                if write_update_function:
                    fp.write ( 'void %s ( SQStream * _stream, %s _value )\n' % (function_name, get_function.returnType) )
            
            if write_update_function:
                fp.write ( '{\n' )
                fp.write ( '   sq_stream_enter_write ( _stream );\n' )
                fp.write ( '   sq_stream_write_string ( _stream, sq_get_constant_string( UPDATE_START ) );\n' )
                if smart_values is None:
                    fp.write ( '   sq_stream_write_string ( _stream, sq_get_constant_string( NAME%i ) );\n' % self.findObjectPathIndex(objectPath) )
                else:
                    parts = normalize_object_path.split('/%s')
                    initialPart = parts[0]
                    fp.write ( '   sq_stream_write_string ( _stream, sq_get_constant_string( NAME%i ) );\n' % self.findObjectPathIndex(initialPart) )
                    for i in range(1, len(parts)):
                        fp.write ( '   sq_protocol_write_string ( _stream, sq_get_constant_string(ROOT) );\n' )
                        fp.write ( '   sq_protocol_write_%s ( _stream, %s );\n' % (self.getAutomationType(get_function.parameters[i - 1].type), get_function.parameters[i - 1].name) )
                        if parts[i] != '':
                            fp.write ( '   sq_protocol_write_string ( _stream, "%s" );\n' % parts[i] )
                
                fp.write ( '   sq_stream_write_byte ( _stream, \' \' );\n' )
                if self.getAutomationType(get_function.returnType) == 'byte_array':
                    fp.write ( '   sq_protocol_write_%s ( _stream, _value->m_start, _value->m_end );\n' % self.getAutomationType(get_function.returnType) )
                elif get_function.returnType == 'SQStringOut':
                    fp.write ( '   sq_protocol_write_string_out ( _stream, &_value );\n' )
                elif get_function.returnType == 'SQStringOut *':
                    fp.write ( '   sq_protocol_write_string_out ( _stream, _value );\n' )
                else:
                    fp.write ( '   sq_protocol_write_%s ( _stream, _value );\n' % self.getAutomationType(get_function.returnType) )
                fp.write ( '   sq_stream_write_string ( _stream, sq_get_constant_string( NEWLINE ) );\n' )
                fp.write ( '   sq_stream_exit_write ( _stream );\n' )
                fp.write ( '}\n' )
                fp.write ( '\n' )
            
            if smart_values is None:
                fp.write ( '%s %s ( void );\n' % (get_function.returnType, get_function.name) )
            else:
                if first_smart_property:
                    fp.write ( '%s %s ( %s );\n' % (get_function.returnType, get_function.name, additional_smart_parameters) )
            fp.write ( 'void sq_generated_property_%s%s ( SQStream * _stream )\n' % (get_function.name, additional_smart_name) )
            fp.write ( '{\n' )
            if smart_values is None:
                fp.write ( '   %s value = %s();\n' % (get_function.returnType, get_function.name) )
            else:
                fp.write ( '   %s value = %s( %s );\n' % (get_function.returnType, get_function.name, ', '.join(smart_values)) )
            if self.getAutomationType(get_function.returnType) == 'byte_array':
                fp.write ( '   sq_protocol_write_%s ( _stream, value->m_start, value->m_end );\n' % self.getAutomationType(get_function.returnType) )
            elif get_function.returnType == 'SQStringOut':
                fp.write ( '   sq_protocol_write_string_out ( _stream, &value );\n' )
            elif get_function.returnType == 'SQStringOut *':
                fp.write ( '   sq_protocol_write_string_out ( _stream, value );\n' )
            else:
                fp.write ( '   sq_protocol_write_%s ( _stream, value );\n' % self.getAutomationType(get_function.returnType) )
            fp.write ( '}\n' )
            fp.write ( '\n' )
            
            if set_function is not None:
                fp.write ( 'void %s ( %s );\n' % (set_function.name, ', '.join(['%s %s' % (parameter.type, parameter.name) for parameter in set_function.parameters])) )
                fp.write ( 'void sq_generated_property_%s%s ( const SQValue * const _value )\n' % (set_function.name, additional_smart_name) )
                fp.write ( '{\n' )
                if smart_values is None:
                    fp.write ( '   %s ( _value->Value.m_%sValue );\n' % (set_function.name, self.getAutomationType(get_function.returnType) ) )
                else:
                    fp.write ( '   %s ( %s, _value->Value.m_%sValue );\n' % (set_function.name, ', '.join(smart_values), self.getAutomationType(get_function.returnType) ) )
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
            for objectPath, get_function, set_function, smart_values, normalize_object_path, first_smart_property in self.m_foundProperties:
                if first:
                    first = False
                else:
                    fp.write ( ',\n' )
                
                additional_smart_name = ''
                if smart_values is not None:
                    additional_smart_name = '_' + '_'.join(smart_values)
                    
                if set_function is None:
                    fp.write ( '   { sq_generated_property_%s%s, NULL, VALUE_TYPE_%s }' % (get_function.name, additional_smart_name, self.getAutomationType(get_function.returnType).upper()) )
                else:
                    fp.write ( '   { sq_generated_property_%s%s, sq_generated_property_%s%s, VALUE_TYPE_%s }' % (get_function.name, additional_smart_name,
                                                                                                               set_function.name, additional_smart_name, self.getAutomationType(get_function.returnType).upper()) )
        
        fp.write ( '};\n' )
        fp.write ( '\n' )

        for function in self.m_foundFunctions:
            if len(function.parameters) == 0:
                fp.write ( '%s %s ( void );\n' % (function.returnType, function.name) )
            else:
                fp.write ( '%s %s ( %s );\n' % (function.returnType, function.name, ', '.join(['%s %s' % (parm.type, parm.name) for parm in function.parameters])) )
            
            fp.write ( 'void sq_generated_function_%s ( SQStream * _stream, const SQValue * _inputValues )\n' % function.name )
            fp.write ( '{\n' )
            for index, parameter in enumerate(function.parameters):
                fp.write ( '   %s %s_parameter = _inputValues[%i].Value.m_%sValue;\n' % (parameter.type, parameter.name, index, self.getAutomationType(parameter.type)) )
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
                fp.write ( '   { sq_generated_function_%s, VALUE_TYPE_%s' % (function.name, self.getAutomationType(function.returnType).upper()) )
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
        for objectPath, get_function, set_function, smart_values, normalize_object_path, first_smart_property in self.m_foundProperties:
            objectPath = objectPath[1:].replace('/', '_')
            if normalize_object_path is not None:
                objectPath = objectPath[1:].replace('/', '_').replace('_%s', '')
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
