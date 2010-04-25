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

class SmartObject ( object ):
    @property
    def index ( self ):
        return self.m_index
    
    @property
    def objectPath ( self ):
        return self.m_objectPath
    
    @property
    def smartObjectPath ( self ):
        return self.m_smartObjectPath
    
    @property
    def firstSmartObjectPath ( self ):
        return self.m_firstSmartObjectPath
    
    @property
    def normalizedSmartObjectPath ( self ):
        return self._normalizeSmartObjectPath()
    
    @property
    def smart ( self ):
        return self.objectPath.find('(') != -1
    
    @property
    def numSmartParamers ( self ):
        return len([element for element in self.objectPath.split ( '/' ) if len(element) > 1 and element[0] == '(' and element[-1] == ')'])

    @property
    def allSmartObjectPaths ( self ):
        return self._listAllSmartObjectPaths ()
    
    @property
    def smartValues ( self ):
        return self.m_smartValues
    
    @property
    def additionalSmartName ( self ):
        if self.m_smartValues:
            return '_' + '_'.join(self.m_smartValues)
        else:
            return ''
    
    @property
    def additionalSmartParameters ( self ):
        return ', '.join ( ['%s %s' % (parameter.type, parameter.name) for parameter in self.getFunction.parameters] )

    def __init__ ( self, _automationFile, _index, _objectPath, _smartObjectPath = None, _firstSmartObjectPath = False, _smartValues = None ):
        self.m_automationFile = _automationFile
        self.m_index = _index
        self.m_objectPath = _objectPath
        self.m_smartObjectPath = _smartObjectPath
        self.m_firstSmartObjectPath = _firstSmartObjectPath
        self.m_smartValues = _smartValues
        self.m_updateFunctionName = None

    def _normalizeSmartObjectPath ( self ):
        ret = ''
        for element in self.objectPath.split ( '/' ):
            if len(element) > 1 and element[0] == '(' and element[-1] == ')':
                ret += '/%s'
            elif element == '':
                pass
            else:
                ret += '/%s' % element
        return ret
    
    def _listAllSmartObjectPaths ( self ):
        elements = self.objectPath.split ( '/' )[1:]
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
            
        return self._generateSmartObjectPaths ( metaPath, 0 )
    
    def _generateSmartObjectPaths ( self, metaPath, index ):
        if len(metaPath) == index:
            yield '', []
        
        else:
            if type(metaPath[index]) in types.StringTypes:
                for rest, values in self._generateSmartObjectPaths ( metaPath, index + 1 ):
                    yield '/' + metaPath[index] + rest, values
            else:
                for start in metaPath[index]:
                    for rest, values in self._generateSmartObjectPaths ( metaPath, index + 1 ):
                        yield  '/' + str(start) + rest, [start] + values[:]

class Property ( SmartObject ):
    @property
    def type ( self ):
        return self.getFunction.returnType
    
    @property
    def automationType ( self ):
        return self.m_automationFile.getAutomationType(self.getFunction.returnType)

    @property
    def getFunction ( self ):
        return self.m_getFunction
    
    @property
    def setFunction ( self ):
        return self.m_setFunction
    
    @property
    def translatedObjectPath ( self ):
        if self.smart:
            return self.normalizedSmartObjectPath[1:].replace('/', '_').replace('_%s', '')
        else:
            return self.objectPath[1:].replace('/', '_')
    
    @property
    def writeUpdateFunction ( self ):
        return not self.smart or self.firstSmartObjectPath
    
    @property
    def updateFunctionName ( self ):
        if self.m_updateFunctionName is None:
            base_function_name = 'sq_%s_updated' % self.translatedObjectPath
            self.m_updateFunctionName = base_function_name
            i = 2
            while self.m_updateFunctionName in self.m_automationFile.m_seenUpdateFunctions:
                self.m_updateFunctionName = '%s%i' % (base_function_name, i)
                i += 1
            self.m_automationFile.m_seenUpdateFunctions.add ( self.m_updateFunctionName )
        
        return self.m_updateFunctionName
    
    def __init__ ( self, _automationFile, _index, _objectPath, _getFunction, _setFunction, _smartObjectPath = None, _firstSmartObjectPath = False, _smartValues = None ):
        super ( Property, self ).__init__ ( _automationFile, _index, _objectPath, _smartObjectPath, _firstSmartObjectPath, _smartValues )
        
        self.m_getFunction = _getFunction
        self.m_setFunction = _setFunction
    

class Function ( SmartObject ):
    @property
    def returnType ( self ):
        return self.m_function.returnType
    
    @property
    def parameters ( self ):
        return self.m_function.parameters

    @property
    def name ( self ):
        return self.m_function.m_name
    
    def __init__ ( self, _automationFile, _index, _objectPath, _function, _smartObjectPath = None, _firstSmartObjectPath = False, _smartValues = None ):
        super ( Function, self ).__init__ ( _automationFile, _index, _objectPath, _smartObjectPath, _firstSmartObjectPath, _smartValues )
        
        self.m_function = _function

class AutomationFile ( object ):
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
        self.m_seenUpdateFunctions = sets.Set()
    
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
                    
                    property = Property(self, propertyIndex, objectPath, get_function, set_function)
                    
                    if len(get_function.parameters) == property.numSmartParamers:
                        if set_function is None or len(set_function.parameters) == 1 + property.numSmartParamers:
                            if set_function is None or get_function.returnType == set_function.parameters[0 + property.numSmartParamers].type:
                                if property.automationType is not None:
                                    if property.smart:
                                        first = True
                                        for smartObjectPath, values in property.allSmartObjectPaths:
                                            self.createParents ( smartObjectPath )
                                            self.m_objectPaths.append ( (smartObjectPath, 'INFO_TYPE_PROPERTY', propertyIndex) )
                                            self.m_foundProperties.append ( Property(self, propertyIndex, objectPath, get_function, set_function, smartObjectPath, first, values) )
                                            propertyIndex += 1
                                            first = False
                                    else:
                                        self.createParents ( objectPath )
                                        self.m_objectPaths.append ( (objectPath, 'INFO_TYPE_PROPERTY', propertyIndex) )
                                        self.m_foundProperties.append ( property )
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
                        functionObject = Function(self, functionIndex, objectPath, function)
                        if functionObject.smart:
                            first = True
                            for smartObjectPath, values in functionObject.allSmartObjectPaths:
                                self.createParents ( smartObjectPath )
                                self.m_objectPaths.append ( (smartObjectPath, 'INFO_TYPE_CALLABLE', functionIndex) )
                                self.m_foundFunctions.append ( Function(self, functionIndex, objectPath, function, smartObjectPath, first, values) )
                                functionIndex += 1
                                first = False
                            
                        else:
                            self.createParents ( objectPath )
                            self.m_objectPaths.append ( (objectPath, 'INFO_TYPE_CALLABLE', functionIndex) )
                            self.m_foundFunctions.append ( functionObject )
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
        
        for property in self.m_foundProperties:
            translatedObjectPath = property.translatedObjectPath
            
            if property.writeUpdateFunction:
                if property.smart:
                    fp.write ( 'void %s ( %s, %s _value )\n' % (property.updateFunctionName, property.additionalSmartParameters, property.type) )
                else:
                    fp.write ( 'void %s ( %s _value )\n' % (property.updateFunctionName, property.type) )
                
                fp.write ( '{\n' )
                fp.write ( '   SQStream * stream = NULL;\n' )
                fp.write ( '   SQServer * server = sq_server_get_instance ();\n' )
                fp.write ( '   stream = server->m_stream;\n' )
                fp.write ( '   sq_stream_enter_write ( stream );\n' )
                fp.write ( '   sq_stream_write_string ( stream, sq_get_constant_string( UPDATE_START ) );\n' )
                if property.smart:
                    parts = property.normalizedSmartObjectPath.split('/%s')
                    initialPart = parts[0]
                    fp.write ( '   sq_stream_write_string ( stream, sq_get_constant_string( NAME%i ) );\n' % self.findObjectPathIndex(initialPart) )
                    for i in range(1, len(parts)):
                        fp.write ( '   sq_stream_write_string ( stream, sq_get_constant_string(ROOT) );\n' )
                        parameter = property.getFunction.parameters[i - 1]
                        if self.getAutomationType(parameter.type) == "integer":
                            fp.write ( '   sq_protocol_write_integer ( stream, %s );\n' % parameter.name )
                        else:
                            fp.write ( '   sq_stream_write_string ( stream, %s );\n' % parameter.name )
                        if parts[i] != '':
                            fp.write ( '   sq_stream_write_string ( stream, "%s" );\n' % parts[i] )
                
                else:
                    fp.write ( '   sq_stream_write_string ( stream, sq_get_constant_string( NAME%i ) );\n' % self.findObjectPathIndex(objectPath) )
                
                fp.write ( '   sq_stream_write_byte ( stream, \' \' );\n' )
                if property.automationType == 'byte_array':
                    fp.write ( '   sq_protocol_write_%s ( stream, _value->m_start, _value->m_end );\n' % property.automationType )
                elif property.type == 'SQStringOut':
                    fp.write ( '   sq_protocol_write_string_out ( stream, &_value );\n' )
                elif property.type == 'SQStringOut *':
                    fp.write ( '   sq_protocol_write_string_out ( stream, _value );\n' )
                else:
                    fp.write ( '   sq_protocol_write_%s ( stream, _value );\n' % property.automationType )
                fp.write ( '   sq_stream_write_string ( stream, sq_get_constant_string( NEWLINE ) );\n' )
                fp.write ( '   sq_stream_exit_write ( stream );\n' )
                fp.write ( '}\n' )
                fp.write ( '\n' )
            
            if property.smart:
                if property.firstSmartObjectPath:
                    fp.write ( '%s %s ( %s );\n' % (property.type, property.getFunction.name, property.additionalSmartParameters) )
            else:
                fp.write ( '%s %s ( void );\n' % (property.type, property.getFunction.name) )
            
            fp.write ( 'void sq_generated_property_%s%s ( SQStream * _stream )\n' % (property.getFunction.name, property.additionalSmartName) )
            fp.write ( '{\n' )
            if property.smart:
                fp.write ( '   %s value = %s( %s );\n' % (property.type, property.getFunction.name, ', '.join(property.smartValues)) )
                
            else:
                fp.write ( '   %s value = %s();\n' % (property.type, property.getFunction.name) )
            
            if property.automationType == 'byte_array':
                fp.write ( '   sq_protocol_write_%s ( _stream, value->m_start, value->m_end );\n' % property.automationType )
            elif property.type == 'SQStringOut':
                fp.write ( '   sq_protocol_write_string_out ( _stream, &value );\n' )
            elif property.type == 'SQStringOut *':
                fp.write ( '   sq_protocol_write_string_out ( _stream, value );\n' )
            else:
                fp.write ( '   sq_protocol_write_%s ( _stream, value );\n' % property.automationType )
            fp.write ( '}\n' )
            fp.write ( '\n' )
            
            if property.setFunction is not None:
                fp.write ( 'void %s ( %s );\n' % (property.setFunction.name, ', '.join(['%s %s' % (parameter.type, parameter.name) for parameter in property.setFunction.parameters])) )
                fp.write ( 'void sq_generated_property_%s%s ( const SQValue * const _value )\n' % (property.setFunction.name, property.additionalSmartName) )
                fp.write ( '{\n' )
                if property.smart:
                    fp.write ( '   %s ( %s, _value->Value.m_%sValue );\n'
                               % ( property.setFunction.name, ', '.join(property.smartValues),
                                   property.automationType ) )
                else:
                    fp.write ( '   %s ( _value->Value.m_%sValue );\n'
                               % (property.setFunction.name, property.automationType) )
                
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
            for property in self.m_foundProperties:
                if first:
                    first = False
                else:
                    fp.write ( ',\n' )
                
                if property.setFunction is None:
                    fp.write ( '   { sq_generated_property_%s%s, NULL, VALUE_TYPE_%s }' \
                                   % (property.getFunction.name, property.additionalSmartName, property.automationType.upper()) )
                else:
                    fp.write ( '   { sq_generated_property_%s%s, sq_generated_property_%s%s, VALUE_TYPE_%s }' \
                                   % (property.getFunction.name, property.additionalSmartName, 
                                      property.setFunction.name, property.additionalSmartName, 
                                      property.automationType.upper()) )
        
        fp.write ( '};\n' )
        fp.write ( '\n' )

        for function in self.m_foundFunctions:
            if not function.smart or function.firstSmartObjectPath:
                if len(function.parameters) == 0:
                    fp.write ( '%s %s ( void );\n' % (function.returnType, function.name) )
                else:
                    fp.write ( '%s %s ( %s );\n' % (function.returnType, function.name, ', '.join(['%s %s' % (parm.type, parm.name) for parm in function.parameters])) )
            
            fp.write ( 'void sq_generated_function_%s%s ( SQStream * _stream, const SQValue * _inputValues )\n' % (function.name, function.additionalSmartName) )
            fp.write ( '{\n' )
            for index, parameter in enumerate(function.parameters):
                if index < function.numSmartParamers:
                    fp.write ( '   %s %s_parameter = %s;\n' % (parameter.type, parameter.name, function.smartValues[index]) )
                else:
                    fp.write ( '   %s %s_parameter = _inputValues[%i].Value.m_%sValue;\n' % (parameter.type, parameter.name, index, self.getAutomationType(parameter.type)) )

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
                fp.write ( '   { sq_generated_function_%s%s, VALUE_TYPE_%s' % (function.name, function.additionalSmartName, self.getAutomationType(function.returnType).upper()) )
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
        for property in self.m_foundProperties:
            if property.writeUpdateFunction:
                if property.smart:
                    fp.write ( 'void %s ( %s, %s _value );\n' % (property.updateFunctionName, property.additionalSmartParameters, property.type) )
                else:
                    fp.write ( 'void %s ( %s _value );\n' % (property.updateFunctionName, property.type) )
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
