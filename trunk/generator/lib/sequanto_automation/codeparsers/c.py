import types
import re

from sequanto_automation.codeparser import ICodeParser, Function, Parameter, Enum

class CodeParser ( ICodeParser ):
    RE_IDENTIFIER = '[A-Za-z_][A-Za-z0-9_]*'
    RE_TYPE = '((const|unsigned|signed)\s+)*%s(\s+|\s*((\\*|\\[\\])\s*)+)(const\s+)?' % RE_IDENTIFIER
    RE_PARAMETER = '((?P<parameterType>%s)\s*(?P<parameterName>%s))' % (RE_TYPE, RE_IDENTIFIER)
    RE_PARAMETER_COMPILED = re.compile(RE_PARAMETER)
    RE_FUNCTION = re.compile ( '(?P<returnType>%(type)s)(?P<name>%(identifier)s)\s*\\(\s*(?P<parameters>((%(parameter)s\s*,?\s*)*)|void)\s*\\)' % {'identifier' : RE_IDENTIFIER, 'type' : RE_TYPE, 'parameter' : RE_PARAMETER} )
    RE_TYPE_PARTS = re.compile('const|void|\\[|\\]|\\*|%s' % RE_IDENTIFIER)
    RE_BRACKET_PAIRS_WITH_SPACE = re.compile('\\[\s+\\]')
    RE_ENUM = re.compile ( 'enum\s+(?P<name>%s)\s*\{\s*(?P<values>.*?)\s*\}' % RE_IDENTIFIER, re.MULTILINE + re.DOTALL )
    RE_TYPEDEF_ENUM = re.compile ( 'typedef enum\s+(?P<name>%s)\s*\{\s*(?P<values>.*?)\s*\}\s*(?P<typedef_name>%s)\s*;' % (RE_IDENTIFIER, RE_IDENTIFIER), re.MULTILINE + re.DOTALL )
    RE_ENUM_VALUE = re.compile ( '^\s*(?P<name>%s)(\s*=\s*(?P<value>\-?[0-9]+))?\s*$' % RE_IDENTIFIER )
    
    def __init__ ( self ):
        self.clear()
    
    def cleanType ( self, _type ):
        _type = self.RE_TYPE_PARTS.findall ( _type )
        if type(_type) in types.StringTypes:
            return self.RE_BRACKET_PAIRS_WITH_SPACE.sub ( '[]', _type )
        else:
            return self.RE_BRACKET_PAIRS_WITH_SPACE.sub ( '[]', ' '.join ( _type ) )
    
    def _parseEnumValues ( self, values_string ):
        values_string = [s.strip() for s in values_string.split(',') if len(s.strip()) > 0]
        values = []
        for value_string in values_string:
            value_match = self.RE_ENUM_VALUE.match ( value_string )
            if value_match:
                if value_match.group('value') != None:
                    values.append ( (value_match.group('name'), value_match.group('value')) )
                else:
                    values.append ( (value_match.group('name'), None) )
            else:
                raise Exception('Unparseable enum value string: "%s" in enum %s (defined in character %i to %i)' % (value_string, name, match.start(), match.end()) )
        return values
    
    def parse ( self, _input ):
        for match in self.RE_FUNCTION.finditer ( _input ):
            returnType = self.cleanType(match.group('returnType'))
            name = match.group('name')
            parameters = match.group('parameters').strip()
            if parameters == '' or parameters == 'void':
                parameters = []
            else:
                parameterList = []
                for parameter in parameters.split(','):
                    match = self.RE_PARAMETER_COMPILED.match ( parameter.strip() )
                    parameterObject = Parameter(match.group('parameterName'), self.cleanType(match.group('parameterType')))
                    parameterList.append ( parameterObject )
                
                parameters = parameterList
            
            function = Function ( name, returnType, parameters )
            self.m_functions[name] = function
        
        for match in self.RE_ENUM.finditer ( _input ):
            name = match.group('name')
            values = self._parseEnumValues ( match.group('values') )
            self.m_enums[name] = Enum(name, values)
            
        for match in self.RE_TYPEDEF_ENUM.finditer ( _input ):
            name = match.group('typedef_name')
            values = self._parseEnumValues ( match.group('values') )
            self.m_enums[name] = Enum(name, values)
    
    def hasFunction ( self, _name ):
        return _name in self.m_functions
    
    def getFunction ( self, _name ):
        return self.m_functions[_name]
    
    def listFunctions ( self ):
        return self.m_functions.items()

    def hasEnum ( self, _name ):
        return _name in self.m_enums
    
    def getEnum ( self, _name ):
        return self.m_enums[_name]
    
    def listEnums ( self ):
        return self.m_enums.items()
    
    def clear ( self ):
        self.m_functions = {}
        self.m_enums = {}
