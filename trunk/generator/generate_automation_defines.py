#!/usr/bin/env python

import sys
import re
import types
if sys.version_info[0] == 2 and sys.version_info[1] < 6:
    from sets import Set as set
from os import path
import datetime
from StringIO import StringIO

if path.isdir ( path.join ( path.dirname(__file__), 'lib' ) ):
    sys.path.append ( path.join ( path.dirname(__file__), 'lib' ) )

code_dir = 'code'
if path.isdir ( path.join ( path.dirname(__file__), 'code' ) ):
    code_dir = path.join ( path.dirname(__file__), 'code' )

from sequanto_automation.codeparser import get as get_code_parser
from sequanto_automation.codeparser import Parameter

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
    def numSmartParameters ( self ):
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
    def listAdditionalSmartParameters ( self ):
        ret = []
        i = 0
        for value in self.m_smartValues:
            name = 'parameter%i' % i
            
            try:
                int(value)
                ret.append ( Parameter ( name, 'int' ) )
                
            except TypeError:
                ret.append ( Parameter ( name, 'const char *' ) )
            
            i += 1
    
        return ret
    
    @property
    def additionalSmartParameters ( self ):
        return ', '.join ( ['%s %s' % (parameter.type, parameter.name) for parameter in self.listAdditionalSmartParameters] )
    
    @property
    def writeUpdateFunction ( self ):
        return not self.smart or self.firstSmartObjectPath

    @property
    def translatedObjectPath ( self ):
        if self.smart:
            return self.normalizedSmartObjectPath[1:].replace('/', '_').replace('_%s', '')
        else:
            return self.objectPath[1:].replace('/', '_')
    
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
    def listAdditionalSmartParameters ( self ):
        return self.getFunction.parameters
    
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
    def listAdditionalSmartParameters ( self ):
        return self.m_function.parameters[:self.numSmartParameters]
    
    @property
    def name ( self ):
        return self.m_function.m_name
    
    def __init__ ( self, _automationFile, _index, _objectPath, _function, _smartObjectPath = None, _firstSmartObjectPath = False, _smartValues = None ):
        super ( Function, self ).__init__ ( _automationFile, _index, _objectPath, _smartObjectPath, _firstSmartObjectPath, _smartValues )
        
        self.m_function = _function

class Monitor ( SmartObject ):
    @property
    def types ( self ):
        return self.m_types
    
    @property
    def automationTypes ( self ):
        return [self.m_automationFile.getAutomationType(type) for type in self.types]
    
    @property
    def parameterString ( self ):
        return ', '.join ( ['%s _value%i' % (self.m_automationFile.getRecognizedCType(type), num) for num, type in enumerate(self.types)] )
    
    def __init__ ( self, _automationFile, _index, _objectPath, _types, _smartObjectPath = None, _firstSmartObjectPath = False, _smartValues = None ):
        super ( Monitor, self ).__init__ ( _automationFile, _index, _objectPath, _smartObjectPath, _firstSmartObjectPath, _smartValues )
        
        self.m_types = _types

class Branch ( object ):
    @property
    def index ( self ):
        return self.m_index
    
    @property
    def generatedName ( self ):
        return 'BRANCH_NAME%i' % self.index
    
    @property
    def objectPath ( self ):
        return self.m_objectPath
    
    @property
    def infoHandlerFunction ( self ):
        return 'sq_handle_branch_info_%s' % (self.objectPath[1:].replace ( '/', '_' ))

    @property
    def listHandlerFunction ( self ):
        return 'sq_handle_branch_list_%s' % (self.objectPath[1:].replace ( '/', '_' ))
    
    @property
    def getHandlerFunction ( self ):
        return 'sq_handle_branch_get_%s' % (self.objectPath[1:].replace ( '/', '_' ))

    @property
    def setHandlerFunction ( self ):
        return 'sq_handle_branch_set_%s' % (self.objectPath[1:].replace ( '/', '_' ))

    @property
    def disableHandlerFunction ( self ):
        return 'sq_handle_branch_disable_%s' % (self.objectPath[1:].replace ( '/', '_' ))

    @property
    def enableHandlerFunction ( self ):
        return 'sq_handle_branch_enable_%s' % (self.objectPath[1:].replace ( '/', '_' ))

    @property
    def callHandlerFunction ( self ):
        return 'sq_handle_branch_call_%s' % (self.objectPath[1:].replace ( '/', '_' ))

    
    def __init__ ( self, _index, _objectPath ):
        self.m_index = _index
        self.m_objectPath = _objectPath

class AutomationFile ( object ):
    AUTOMATION_TYPES = ['boolean', 'integer', 'float', 'string']
    
    def setErrorReportingFilename ( self, _filename ):
        self.m_errorReportingFilename = _filename
    
    def __init__ ( self ):
        self.m_parser = get_code_parser('C')
        self.m_imports = []
        self.m_properties = []
        self.m_functions = []
        self.m_monitors = []
        self.m_branches = []
        self.m_name = None
        self.m_typedefs = {}
        self.m_typedefs_c_name = {}
        
        self.m_errorReportingFilename = '<unknown>'
        
        self.m_objectPaths = []
        self.m_foundProperties = []
        self.m_foundFunctions = []
        self.m_foundMonitors = []
        self.m_foundBranches = []
        self.m_maxNumberOfParameters = 0
        self.m_seenUpdateFunctions = set()
    
    def parse ( self, _input, _searchDirectory ):
        lineNumber = 1
        for line in _input.readlines():
            line = line.strip()
            if len(line) == 0 or line[0] == '#':
                lineNumber = lineNumber + 1
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
                objectPath, function = rest.split(' ', 1)
                self.m_functions.append ( (lineNumber, objectPath, function) )
            
            elif command == 'monitor':
                objectPath, type = rest.split(' ', 1)
                types = [t.strip() for t in type.split(',')]
                self.m_monitors.append ( (lineNumber, objectPath, types) )
            
            elif command == 'branch':
                objectPath = rest
                self.m_branches.append ( (lineNumber, objectPath) )
                
            elif command == 'typedef':
                c_type, c_name = rest.rsplit(' ', 1)
                
                try:
                    self.m_typedefs[c_name] = self.getAutomationType(c_type)
                    self.m_typedefs_c_name[c_name] = c_type
                except:
                    self.reportError ( lineNumber, '%s is not a recognized C-type' % (c_type) )
                
            else:
                self.reportError ( lineNumber, 'Unknown command "%s"' % command )
            
            lineNumber = lineNumber + 1

        if self.m_name is None:
            raise 'No name defined in the automation definition'
    
    def reportError ( self, _lineNumber, _text ):
        print '%s (%i) : error: %s' % (self.m_errorReportingFilename, _lineNumber, _text)
        
    def getRecognizedCType ( self, type ):
        if type in ['unsigned long', 'long', 'signed long', 'unsigned int', 'int', 'signed int', 'unsigned short', 'short', 'signed short', 'unsigned char', 'char', 'signed char',
                    'int8_t', 'uint8_t', 'int16_t', 'uint16_t', 'int32_t', 'uint32_t', 'int64_t', 'uint64_t',
                    'char *', 'char * const', 'const char *', 'const char * const', 'SQStringOut', 'SQStringOut *',
                    'float', 'double',
                    'bool', 'SQBool',
                    'void',
                    'SQByteArray *']:
            return type
        elif type in self.m_typedefs_c_name:
            return self.getRecognizedCType(self.m_typedefs_c_name[type])
        else:
            raise Exception('Could not resolve the C-type "%s" to a recognized C type.' % type)
    
    def getAutomationType ( self, type ):
        if type in ['unsigned long', 'long', 'signed long', 'unsigned int', 'int', 'signed int', 'unsigned short', 'short', 'signed short', 'unsigned char', 'char', 'signed char',
                    'int8_t', 'uint8_t', 'int16_t', 'uint16_t', 'int32_t', 'uint32_t', 'int64_t', 'uint64_t']:
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
        elif type in self.m_typedefs:
            return self.m_typedefs[type]
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
            try:
                if not path.exists(filename):
                    self.reportError ( lineNumber, 'Could not find file named "%s"' % filename )
                else:
                    fp = open ( filename, 'r' )
                    self.m_parser.parse ( fp.read() )
                    fp.close()
            except Exception, ex:
                self.reportError ( lineNumber, ex )
        
        propertyIndex = 0
        for lineNumber, objectPath, get_function, set_function in self.m_properties:
            try:
                if self.m_parser.hasFunction(get_function):
                    if set_function is None or self.m_parser.hasFunction(set_function):
                        get_function = self.m_parser.getFunction(get_function)
                        if set_function is not None:
                            set_function = self.m_parser.getFunction(set_function)

                        property = Property(self, propertyIndex, objectPath, get_function, set_function)

                        if len(get_function.parameters) == property.numSmartParameters:
                            if set_function is None or len(set_function.parameters) == 1 + property.numSmartParameters:
                                if set_function is None or get_function.returnType == set_function.parameters[0 + property.numSmartParameters].type:
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
            except Exception, ex:
                self.reportError ( lineNumber, ex )
        
        functionIndex = 0
        for lineNumber, objectPath, function in self.m_functions:
            try:
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
                                    self.m_maxNumberOfParameters = max(self.m_maxNumberOfParameters, len(functionObject.parameters) - functionObject.numSmartParameters)
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
            except Exception, ex:
                self.reportError ( lineNumber, ex )
        
        monitorIndex = 0
        for lineNumber, objectPath, types in self.m_monitors:
            try:
                allTypesOk = True
                for type in types:
                    try:
                        automationType = self.getAutomationType(type)
                    except:
                        self.reportError ( lineNumber, 'The monitor type is not recognized (%s)' % (type) )
                        allTypesOk = False
                if allTypesOk:
                    monitor = Monitor(self, monitorIndex, objectPath, types)
                    if monitor.smart:
                        first = True
                        for smartObjectPath, values in monitor.allSmartObjectPaths:
                            self.createParents ( smartObjectPath )
                            self.m_objectPaths.append ( (smartObjectPath, 'INFO_TYPE_MONITOR', monitorIndex) )
                            self.m_foundMonitors.append ( Monitor(self, monitorIndex, objectPath, types, smartObjectPath, first, values) )
                            monitorIndex += 1
                            first = False

                    else:
                        self.createParents ( objectPath )
                        self.m_objectPaths.append ( (objectPath, 'INFO_TYPE_MONITOR', monitorIndex) )
                        self.m_foundMonitors.append ( monitor )
                        monitorIndex += 1
        
            except Exception, ex:
                self.reportError ( lineNumber, ex )
        
        branchIndex = 0
        for lineNumber, objectPath in self.m_branches:
            try:
                self.m_objectPaths.append ( (objectPath, 'INFO_TYPE_BRANCH', branchIndex) )
                self.m_foundBranches.append ( Branch(branchIndex, objectPath) )
                branchIndex += 1
            except Exception, ex:
                self.reportError ( lineNumber, ex )
        
        self.m_objectPaths.sort()
        
    def findObjectPathIndex ( self, objectPath ):
        ret = 0
        for oPath, type, index in self.m_objectPaths:
            if objectPath == oPath:
                return ret
            ret += 1
        raise Exception('Could not find %s' % objectPath)
    
    def writeValue ( self, type, value ):
        automationType = self.getAutomationType(type)
        if automationType == 'String':
            return 'sq_protocol_write_string ( _stream, %s )' % value
        elif automationType == 'Integer':
            return 'sq_protocol_write_integer ( _stream, %s )' % value
        else:
            raise 'Unknown type %s' % automationType
    
    def writeSuccessMessageWithValue ( self, fp, c_type, automation_type, value ):
        if automation_type == 'byte_array':
            fp.write ( '   sq_protocol_write_success_with_byte_array_message ( _stream, %s->m_start, %s->m_end );\n' % (value, value) )
        elif c_type == 'SQStringOut':
            fp.write ( '   sq_protocol_write_success_with_string_out_message ( _stream, &%s );\n' % value )
        elif automation_type == 'float' and c_type != 'float':
            fp.write ( '   sq_protocol_write_success_with_float_message ( _stream, (float) %s );\n' % value )
        else:
            if c_type == 'SQStringOut *':
                automation_type = 'string_out'
            fp.write ( '   sq_protocol_write_success_with_%s_message ( _stream, %s );\n' % (automation_type, value) )
    
    def generate ( self ):
        print 'Writing interface to %s_automation.c (in %s)' % (self.m_name, path.abspath(path.curdir))
        
        fp = open ( '%s_automation.c' % self.m_name, 'w' )
        fp.write ( '/*\n' )
        fp.write ( ' * DO NOT EDIT THIS FILE IT IS AUTO GENERATED!!!\n' )
        fp.write ( ' *\n' )
        fp.write ( ' * Generated by %s\n' % sys.argv[0] )
        fp.write ( ' *           on %s\n' % datetime.datetime.now() )
        fp.write ( ' */\n' )
        fp.write ( '\n' )
        fp.write ( '#include <string.h>\n' )
        fp.write ( '#ifdef HAVE_STDINT_H\n' )
        fp.write ( '#include <stdint.h>\n' )
        fp.write ( '#endif\n' )
        fp.write ( '#include <sequanto/types.h>\n' )
        fp.write ( '#include <sequanto/stream.h>\n' )
        fp.write ( '#include <sequanto/value.h>\n' )
        fp.write ( '#include <sequanto/protocol.h>\n' )
        fp.write ( '#include <sequanto/server.h>\n' )
        fp.write ( '\n' )
        fp.write ( '#include "%s_automation.h"\n' % (self.m_name) )
        fp.write ( '\n' )
        
        fp.write ( 'static const char NEWLINE[] SQ_CONST_VARIABLE = "\\r\\n";\n' )
        fp.write ( 'static const char UPDATE_START[] SQ_CONST_VARIABLE = "!UPDATE ";\n' )
        
        fp.write ( 'typedef enum _SQInfoType { INFO_TYPE_LIST = 0, INFO_TYPE_PROPERTY = 1, INFO_TYPE_CALLABLE = 2, INFO_TYPE_MONITOR = 3, INFO_TYPE_BRANCH = 4 } SQInfoType;\n' )
        
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

        for branch in self.m_foundBranches:
            fp.write ( 'static const char %s[] SQ_CONST_VARIABLE = "%s";\n' % (branch.generatedName, branch.objectPath) )
        fp.write ( '\n' )
        fp.write ( 'static const int NUMBER_OF_BRANCHES  SQ_CONST_VARIABLE = %i;\n' % (len(self.m_foundBranches)) )
        fp.write ( '\n' )
        fp.write ( 'typedef SQBool (*SQBranchInfoHandler) ( SQStream * _stream, const char * _objectPath );\n' )
        fp.write ( 'typedef SQBool (*SQBranchListHandler) ( SQStream * _stream, const char * _objectPath );\n' )
        fp.write ( 'typedef SQBool (*SQBranchGetHandler) ( SQStream * _stream, const char * _objectPath );\n' )
        fp.write ( 'typedef SQBool (*SQBranchSetHandler) ( SQStream * _stream, const char * _objectPath, const SQValue * const _value );\n' )
        fp.write ( 'typedef SQBool (*SQBranchEnableHandler) ( SQStream * _stream, const char * _objectPath );\n' )
        fp.write ( 'typedef SQBool (*SQBranchDisableHandler) ( SQStream * _stream, const char * _objectPath );\n' )
        fp.write ( 'typedef SQBool (*SQBranchCallHandler) ( SQStream * _stream, const char * _objectPath, const SQValue * const _values, int _numberOfValues );\n' )
        fp.write ( '\n' )
        fp.write ( 'typedef struct _SQBranch\n' )
        fp.write ( '{\n' )
        fp.write ( '    const char * name;\n' )
        fp.write ( '    size_t length;\n' )
        fp.write ( '    SQBranchInfoHandler info_handler;\n' )
        fp.write ( '    SQBranchListHandler list_handler;\n' )
        fp.write ( '    SQBranchGetHandler get_handler;\n' )
        fp.write ( '    SQBranchSetHandler set_handler;\n' )
        fp.write ( '    SQBranchEnableHandler enable_handler;\n' )
        fp.write ( '    SQBranchDisableHandler disable_handler;\n' )
        fp.write ( '    SQBranchCallHandler call_handler;\n' )
        
        fp.write ( '} SQBranch;\n' )
        fp.write ( '\n' )
        fp.write ( 'static const SQBranch BRANCH_LIST[] SQ_CONST_VARIABLE = {\n' )
        if len(self.m_foundBranches) > 0:
            for branch in self.m_foundBranches:
                fp.write ( '    { %s, %i, %s, %s, %s, %s, %s, %s, %s },\n' % (branch.generatedName, len(branch.objectPath), branch.infoHandlerFunction, branch.listHandlerFunction,
                                                                  branch.getHandlerFunction, branch.setHandlerFunction, branch.enableHandlerFunction, branch.disableHandlerFunction,
                                                                  branch.callHandlerFunction) )
        else:
            fp.write ( '    { NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },\n' )
        fp.write ( '};\n' )
        
        for property in self.m_foundProperties:
            translatedObjectPath = property.translatedObjectPath
            
            if property.writeUpdateFunction:
                if property.smart:
                    fp.write ( 'void %s ( %s, %s _value )\n' % (property.updateFunctionName, property.additionalSmartParameters, self.getRecognizedCType(property.type)) )
                else:
                    fp.write ( 'void %s ( %s _value )\n' % (property.updateFunctionName, self.getRecognizedCType(property.type)) )
                
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
                    fp.write ( '   sq_stream_write_string ( stream, sq_get_constant_string( NAME%i ) );\n' % self.findObjectPathIndex(property.objectPath) )
                
                fp.write ( '   sq_stream_write_byte ( stream, \' \' );\n' )
                if property.automationType == 'byte_array':
                    fp.write ( '   sq_protocol_write_%s ( stream, _value->m_start, _value->m_end );\n' % property.automationType )
                elif property.type == 'SQStringOut':
                    fp.write ( '   sq_protocol_write_string_out ( stream, &_value );\n' )
                elif property.type == 'SQStringOut *':
                    fp.write ( '   sq_protocol_write_string_out ( stream, _value );\n' )
                elif property.automationType == 'float' and property.type != 'float':
                    fp.write ( '   sq_protocol_write_float ( stream, (float) _value );\n' )
                else:
                    fp.write ( '   sq_protocol_write_%s ( stream, _value );\n' % property.automationType )
                fp.write ( '   sq_stream_write_string ( stream, sq_get_constant_string( NEWLINE ) );\n' )
                fp.write ( '   sq_stream_exit_write ( stream );\n' )
                fp.write ( '}\n' )
                fp.write ( '\n' )
            
            if property.smart:
                if property.firstSmartObjectPath:
                    fp.write ( '%s %s ( %s );\n' % (self.getRecognizedCType(property.type), property.getFunction.name, property.additionalSmartParameters) )
            else:
                fp.write ( '%s %s ( void );\n' % (self.getRecognizedCType(property.type), property.getFunction.name) )
            
            fp.write ( 'void sq_generated_property_%s%s ( SQStream * _stream )\n' % (property.getFunction.name, property.additionalSmartName) )
            fp.write ( '{\n' )
            if property.smart:
                fp.write ( '   %s value = %s( %s );\n' % (self.getRecognizedCType(property.type), property.getFunction.name, ', '.join(property.smartValues)) )
                
            else:
                fp.write ( '   %s value = %s();\n' % (self.getRecognizedCType(property.type), property.getFunction.name) )
            
            
            assert property.automationType == self.getAutomationType(property.type)
            
            self.writeSuccessMessageWithValue ( fp, property.type, property.automationType, 'value' )
            
            fp.write ( '}\n' )
            fp.write ( '\n' )
            
            if property.setFunction is not None:
                fp.write ( 'void %s ( %s );\n' % (property.setFunction.name, ', '.join(['%s %s' % (self.getRecognizedCType(parameter.type), parameter.name) for parameter in property.setFunction.parameters])) )
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
                    fp.write ( '%s %s ( void );\n' % (self.getRecognizedCType(function.returnType), function.name) )
                else:
                    parms = ', '.join(['%s %s' % (self.getRecognizedCType(parm.type), parm.name) for parm in function.parameters])
                    fp.write ( '%s %s ( %s );\n' % (self.getRecognizedCType(function.returnType), function.name, parms) )
            
            fp.write ( 'void sq_generated_function_%s%s ( SQStream * _stream, const SQValue * _inputValues )\n' % (function.name, function.additionalSmartName) )
            fp.write ( '{\n' )
            
            for index, parameter in enumerate(function.parameters):
                if index < function.numSmartParameters:
                    fp.write ( '   %s %s_parameter = %s;\n' % (self.getRecognizedCType(parameter.type), parameter.name, function.smartValues[index]) )
                else:
                    fp.write ( '   %s %s_parameter = _inputValues[%i].Value.m_%sValue;\n' % (self.getRecognizedCType(parameter.type), parameter.name, index - function.numSmartParameters, self.getAutomationType(parameter.type)) )
            
            if function.returnType == 'void':
                fp.write ( '   %s ( %s );\n' % (function.name, ', '.join(['%s_parameter' % parm.name for parm in function.parameters]) ) )
                fp.write ( '   sq_protocol_write_success_message ( _stream );\n' )
                
            else:
                fp.write ( '   %s ret = %s ( %s );\n' % (self.getRecognizedCType(function.returnType), function.name, ', '.join(['%s_parameter' % parm.name for parm in function.parameters]) ) )
                self.writeSuccessMessageWithValue ( fp, function.returnType, self.getAutomationType(function.returnType), 'ret' )

            if len(function.parameters) - function.numSmartParameters <= 0:
                fp.write ( '   SQ_UNUSED_PARAMETER(_inputValues);\n' )
            
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
                for parameter in function.parameters[function.numSmartParameters:]:
                    fp.write ( ', VALUE_TYPE_%s' % self.getAutomationType(parameter.type).upper() )
                fp.write ( ', VALUE_TYPE_NO_VALUE' * (self.m_maxNumberOfParameters - len(function.parameters) ) )
                fp.write ( ' },\n' )
        
        fp.write ( '};\n' )
        fp.write ( '\n' )
        
        fp.write ( 'SQValueType sq_automation_get_parameter ( const SQCallableInfo * const _callableInfo, int _parameterIndex )\n' )
        fp.write ( '{\n' )
        if self.m_maxNumberOfParameters == 0:
            fp.write ( '   SQ_UNUSED_PARAMETER(_callableInfo);\n' )
            fp.write ( '   SQ_UNUSED_PARAMETER(_parameterIndex);\n\n' )
            fp.write ( '   return VALUE_TYPE_NO_VALUE;\n' )
        else:
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
        
        if len(self.m_foundMonitors) > 0:
            fp.write ( 'static SQBool monitor_state[] = { %s };\n' % ', '.join ( (['SQ_FALSE'] * len(self.m_foundMonitors)) ) )
        else:
            fp.write ( '/* This is just a dummy value, actually there were no monitors defined */\n' )
            fp.write ( 'static SQBool monitor_state[] = { SQ_FALSE };\n' )
        
        for monitor in self.m_foundMonitors:
            if monitor.writeUpdateFunction:
                if monitor.smart:
                    fp.write ( 'void %s ( %s, %s )\n' % (monitor.updateFunctionName, monitor.additionalSmartParameters, monitor.parameterString) )
                else:
                    fp.write ( 'void %s ( %s )\n' % (monitor.updateFunctionName, monitor.parameterString) )
                
                fp.write ( '{\n' )
                fp.write ( '   SQStream * stream = NULL;\n' )
                fp.write ( '   SQServer * server = sq_server_get_instance ();\n' )
                fp.write ( '   if ( monitor_state[%i] == SQ_TRUE )\n' % monitor.index )
                fp.write ( '   {\n' )
                fp.write ( '      stream = server->m_stream;\n' )
                fp.write ( '      sq_stream_enter_write ( stream );\n' )
                fp.write ( '      sq_stream_write_string ( stream, sq_get_constant_string( UPDATE_START ) );\n' )
                if  monitor.smart:
                    parts = monitor.normalizedSmartObjectPath.split('/%s')
                    initialPart = parts[0]
                    fp.write ( '      sq_stream_write_string ( stream, sq_get_constant_string( NAME%i ) );\n' % self.findObjectPathIndex(initialPart) )
                    for i in range(1, len(parts)):
                        fp.write ( '      sq_stream_write_string ( stream, sq_get_constant_string(ROOT) );\n' )
                        
                        parameter = monitor.listAdditionalSmartParameters[i - 1]
                        
                        if self.getAutomationType(parameter.type) == "integer":
                            fp.write ( '      sq_protocol_write_integer ( stream, %s );\n' % parameter.name )
                        else:
                            fp.write ( '      sq_stream_write_string ( stream, %s );\n' % parameter.name )
                        if parts[i] != '':
                            fp.write ( '      sq_stream_write_string ( stream, "%s" );\n' % parts[i] )
                
                else:
                    fp.write ( '      sq_stream_write_string ( stream, sq_get_constant_string( NAME%i ) );\n' % self.findObjectPathIndex(monitor.objectPath) )
                
                fp.write ( '      sq_stream_write_byte ( stream, \' \' );\n' )
                for num, automationType in enumerate(monitor.automationTypes):
                    if num != 0:
                        fp.write ( '      sq_stream_write_byte ( stream, \' \' );\n' )
                    
                    if automationType == 'byte_array':
                        fp.write ( '      sq_protocol_write_%s ( stream, _value%i->m_start, _value%i->m_end );\n' % (automationType, num, num) )
                    elif monitor.types[num] == 'SQStringOut':
                        fp.write ( '      sq_protocol_write_string_out ( stream, &_value%i );\n' % num )
                    elif monitor.types[num] == 'SQStringOut *':
                        fp.write ( '      sq_protocol_write_string_out ( stream, _value%i );\n' % num )
                    elif automationType == 'float' and monitor.types[num] != 'float':
                        fp.write ( '   sq_protocol_write_float ( stream, (float) _value%i );\n' % num )
                    else:
                        fp.write ( '      sq_protocol_write_%s ( stream, _value%i );\n' % (automationType, num) )
                fp.write ( '      sq_stream_write_string ( stream, sq_get_constant_string( NEWLINE ) );\n' )
                fp.write ( '      sq_stream_exit_write ( stream );\n' )
                fp.write ( '   }\n' )
                fp.write ( '}\n' )
                fp.write ( '\n' )

        maxNumberOfValues = 1
        for monitor in self.m_foundMonitors:
            maxNumberOfValues = max(maxNumberOfValues, len(monitor.automationTypes))
        
        fp.write ( 'static const int NUMBER_OF_MONITOR_VALUES SQ_CONST_VARIABLE = %i;\n' % maxNumberOfValues )
        fp.write ( 'typedef struct _SQMonitorInfo { %s; unsigned char index; } SQMonitorInfo;\n' % ('; '.join(['SQValueType value%i' % i for i in xrange(maxNumberOfValues)]) ) )
        fp.write ( 'static const SQMonitorInfo MONITOR_LIST[] SQ_CONST_VARIABLE = {\n' )
        # Some compilers (e.g. VS2005) does not support empty lists, so add a dummy one.
        if len(self.m_foundMonitors) == 0:
            fp.write ( '   { VALUE_TYPE_NO_VALUE, 0xFF }\n' )
        else:
            for monitor in self.m_foundMonitors:
                parameters = ['VALUE_TYPE_%s' % type.upper() for type in monitor.automationTypes]
                while len(parameters) != maxNumberOfValues:
                    parameters.append ( 'VALUE_TYPE_NO_VALUE' )
                
                fp.write ( '   { %s, %i },\n' % (', '.join(parameters), monitor.index) )
        
        fp.write ( '};\n' )
        fp.write ( '\n' )
        
        fp.write ( 'SQValueType sq_automation_get_monitor_value_type ( const SQMonitorInfo * const _monitorInfo, int _valueIndex )\n' )
        fp.write ( '{\n' )
        fp.write ( '   switch ( _valueIndex )\n' )
        fp.write ( '   {\n' )
        for i in range(maxNumberOfValues):
            fp.write ( '   case %i:\n' % i )
            fp.write ( '      return _monitorInfo->value%i;\n' % i )
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
        
        fp.write ( '#ifdef SQ_DISABLE_AUTOMATION_INTERFACE\n' )
        
        #disabledText = '/* Disabled because SQ_DISABLE_AUTOMATION_INTERFACE is defined. */'
        disabledText = '{ do {} while(0); }'
        for property in self.m_foundProperties:
            if property.writeUpdateFunction:
                fp.write ( '#define %s(...) %s\n' % (property.updateFunctionName, disabledText) )

        for monitor in self.m_foundMonitors:
            if monitor.writeUpdateFunction:
                fp.write ( '#define %s(...) %s\n' % (monitor.updateFunctionName, disabledText) )
        
        fp.write ( '#else /* SQ_DISABLE_AUTOMATION_INTERFACE */\n' )
        
        for property in self.m_foundProperties:
            if property.writeUpdateFunction:
                if property.smart:
                    fp.write ( 'void %s ( %s, %s _value );\n' % (property.updateFunctionName, property.additionalSmartParameters, self.getRecognizedCType(property.type)) )
                else:
                    fp.write ( 'void %s ( %s _value );\n' % (property.updateFunctionName, self.getRecognizedCType(property.type)) )

        for monitor in self.m_foundMonitors:
            if monitor.writeUpdateFunction:
                if monitor.smart:
                    fp.write ( 'void %s ( %s, %s );\n' % (monitor.updateFunctionName, monitor.additionalSmartParameters, monitor.parameterString) )
                else:
                    fp.write ( 'void %s ( %s );\n' % (monitor.updateFunctionName, monitor.parameterString) )
        
        fp.write ( '#endif /* SQ_DISABLE_AUTOMATION_INTERFACE */\n' )
        
        for branch in self.m_foundBranches:
            fp.write ( 'SQBool %s ( SQStream * _stream, const char * _objectPath );\n' % (branch.listHandlerFunction) )
            fp.write ( 'SQBool %s ( SQStream * _stream, const char * _objectPath );\n' % (branch.infoHandlerFunction) )
            fp.write ( 'SQBool %s ( SQStream * _stream, const char * _objectPath );\n' % (branch.getHandlerFunction) )
            fp.write ( 'SQBool %s ( SQStream * _stream, const char * _objectPath, const SQValue * const _value );\n' % (branch.setHandlerFunction) )
            fp.write ( 'SQBool %s ( SQStream * _stream, const char * _objectPath );\n' % (branch.disableHandlerFunction) )
            fp.write ( 'SQBool %s ( SQStream * _stream, const char * _objectPath );\n' % (branch.enableHandlerFunction) )
            fp.write ( 'SQBool %s ( SQStream * _stream, const char * _objectPath, const SQValue * const _values, int _numberOfValues );\n' % (branch.callHandlerFunction) )
        
        fp.write ( '#ifdef __cplusplus\n' )
        fp.write ( '}\n' )
        fp.write ( '#endif\n' )
        fp.close()

automationFile = AutomationFile()
for arg in sys.argv[1:]:
    print 'Parsing %s' % arg
    
    fp = None
    try:
        fp = open (arg, 'r')
    except Exception, ex:
        automationFile.reportError ( 0, 'Could not open file named %s: %s' % (arg, ex) )
        sys.exit(-1)
        
    automationFile.setErrorReportingFilename ( arg )
    automationFile.parse ( fp, path.dirname(arg) )
    fp.close()
automationFile.analyze()
automationFile.generate ()
