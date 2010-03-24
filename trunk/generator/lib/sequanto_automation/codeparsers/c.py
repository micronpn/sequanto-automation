import types
import re

from sequanto_automation.codeparser import ICodeParser, Function, Parameter

class CodeParser ( ICodeParser ):
    RE_IDENTIFIER = '[A-Za-z_][A-Za-z0-9_]*'
    RE_TYPE = '((const|unsigned)\s+)*%s(\s+|\s*((\\*|\\[\\])\s*)+)(const\s+)?' % RE_IDENTIFIER
    RE_PARAMETER = '((?P<parameterType>%s)\s*(?P<parameterName>%s))' % (RE_TYPE, RE_IDENTIFIER)
    RE_PARAMETER_COMPILED = re.compile(RE_PARAMETER)
    RE_FUNCTION = re.compile ( '(?P<returnType>%(type)s)(?P<name>%(identifier)s)\s*\\(\s*(?P<parameters>((%(parameter)s\s*,?\s*)*)|void)\s*\\)' % {'identifier' : RE_IDENTIFIER, 'type' : RE_TYPE, 'parameter' : RE_PARAMETER} )
    RE_TYPE_PARTS = re.compile('const|void|\\[|\\]|\\*|%s' % RE_IDENTIFIER)
    RE_BRACKET_PAIRS_WITH_SPACE = re.compile('\\[\s+\\]')
    
    def __init__ ( self ):
        self.clear()
    
    def cleanType ( self, _type ):
        _type = self.RE_TYPE_PARTS.findall ( _type )
        if type(_type) in types.StringTypes:
            return self.RE_BRACKET_PAIRS_WITH_SPACE.sub ( '[]', _type )
        else:
            return self.RE_BRACKET_PAIRS_WITH_SPACE.sub ( '[]', ' '.join ( _type ) )
    
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
    
    def hasFunction ( self, _name ):
        return _name in self.m_functions
    
    def getFunction ( self, _name ):
        return self.m_functions[_name]
    
    def listFunctions ( self ):
        return self.m_functions.items()
    
    def clear ( self ):
        self.m_functions = {}
