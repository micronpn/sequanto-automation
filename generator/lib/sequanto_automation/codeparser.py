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
#    
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

class ICodeParser:
    def parse ( self, _input ): pass
    def hasFunction ( self ): pass
    def listFunctions ( self ): pass
    def clear ( self ): pass

def get ( _name ):
    if _name in ['c', 'C']:
        import sequanto_automation.codeparsers.c
        return sequanto_automation.codeparsers.c.CodeParser()
