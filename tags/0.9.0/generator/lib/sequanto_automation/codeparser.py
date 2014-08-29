import types

#class Type:
#    @property
#    def name ( self ):
#        return self.m_name
#    
#    @property
#    def modifiers ( self ):
#        
#    def __init__ ( self, _name, _type ):
#        self.m_name = _name
#        self.m_type = _type

class Parameter:
    @property
    def name ( self ):
        return self.m_name
    
    @property
    def type ( self ):
        return self.m_type
    
    def __init__ ( self, _name, _type ):
        self.m_name = _name
        self.m_type = _type

class Function:
    @property
    def name ( self ):
        return self.m_name
    
    @property
    def returnType ( self ):
        return self.m_returnType
    
    @property
    def parameters ( self ):
        return self.m_parameters
    
    def __init__ ( self, _name, _returnType, _parameters ):
        self.m_name = _name
        self.m_returnType = _returnType
        self.m_parameters = _parameters

class Enum:
    @property
    def name ( self ):
        return self.m_name
    
    @property
    def valueNames ( self ):
        for value in self.m_values:
            if type(value) == types.TupleType:
                yield value[0]
            else:
                yield value
    
    @property
    def values ( self ):
        for value in self.m_values:
            if type(value) == types.TupleType:
                yield value
            else:
                yield value, None
    
    def __init__ ( self, _name, _values ):
        assert type(_name) in types.StringTypes
        assert type(_values) == types.ListType
        
        self.m_name = _name
        self.m_values = _values

class ICodeParser:
    def parse ( self, _input ): pass
    
    def hasFunction ( self, _name ): pass
    def getFunction ( self, _name ): pass
    def listFunctions ( self ): pass
    
    def hasEnum ( self, _name ): pass
    def getEnum ( self, _name ): pass
    def listEnums ( self ): pass
    
    def clear ( self ): pass

def get ( _name ):
    if _name in ['c', 'C']:
        import sequanto_automation.codeparsers.c
        return sequanto_automation.codeparsers.c.CodeParser()
