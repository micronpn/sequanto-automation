import types
import string

def format_value ( _value ):
    '''
    >>> format_value ( "dette er en test" )
    u'"dette er en test"'

    >>> format_value ( 'dette "er en" test' )
    u'"dette \\\\"er en\\\\" test"'

    >>> format_value ( 123 )
    u'123'

    >>> format_value ( 34.5 )
    u'34.5'

    >>> format_value ( True )
    u'true'

    >>> format_value ( False )
    u'false'

    >>> format_value ( None )
    u'null'

    >>> format_value ( [1, 2, 3, 4] )
    u'[ 1 2 3 4 ]'
    '''

    if type(_value) in types.StringTypes:
        return u'"%s"' % _value.replace('\\', '\\\\').replace('"', '\\"')

    elif type(_value) == types.IntType:
        return unicode(_value)
    elif type(_value) == types.FloatType:
        return unicode(_value)
    elif type(_value) == types.BooleanType:
        if _value:
            return u'true'
        else:
            return u'false'
    elif type(_value) == types.NoneType:
        return u'null'
    elif type(_value) in [types.ListType, types.TupleType]:
        ret = u'[ '

        first = True
        for subValue in _value:
            if not first:
                ret += u' '
            else:
                first = False

            ret += format_value(subValue)

        ret += u' ]'
        return ret
    else:
        raise InvalidValueException(_value)

def parse_string ( _str, _start ):
    '''
    >>> parse_string ( '"test"', 0 )
    ('test', 6)

    >>> parse_string ( '"test""test"', 0 )
    ('test', 6)

    >>> parse_string ( '"test" 123 "test"', 11 )
    ('test', 17)

    '''

    end = _start + 1
    while _str[end] != '"':
        end += 1
    return _str[_start + 1:end], end + 1

def parse_int_or_double_or_hex ( _str, _start ):
    '''
    >>> parse_int_or_double_or_hex ( '123', 0 )
    (123, 4)

    >>> parse_int_or_double_or_hex ( '123.123', 0 )
    (123.123, 8)

    >>> parse_int_or_double_or_hex ( '0.123', 0 )
    (0.123, 6)

    >>> parse_int_or_double_or_hex ( '0x010203', 0 )
    ([1, 2, 3], 9)

    >>> parse_int_or_double_or_hex ( '0x', 0 )
    ([], 3)
    '''

    if _start + 1 < len(_str):
        if _str[_start] == '0' and _str[_start + 1] == 'x':
            ret = []
            end = _start + 2
            while end + 1 < len(_str) and _str[end] in string.digits and _str[end + 1] in string.digits:
                ret.append ( int(_str[end] + _str[end + 1], 16) )
                end += 2
            return ret, end + 1

    end = _start + 1
    isDouble = False
    while end < len(_str) and _str[end] in string.digits + '.':
        if _str[end] == '.':
            isDouble = True
        end += 1

    if isDouble:
        return float(_str[_start:end]), end + 1
    else:
        return int(_str[_start:end]), end + 1

def parse_identifier ( _str, _start ):
    '''
    >>> parse_identifier('true', 0)
    (True, 5)

    >>> parse_identifier('false', 0)
    (False, 6)

    >>> parse_identifier('null', 0)
    (None, 5)
    '''

    end = _start + 1
    while end < len(_str) and _str[end] in string.ascii_letters:
        end += 1

    if _str[_start:end] == 'true':
        return True, end + 1
    elif _str[_start:end] == 'false':
        return False, end + 1
    elif _str[_start:end] == 'null':
        return None, end + 1
    else:
        raise ParseException ( 'Uknown identifier "%s".' % _str[_start:end] )

def parse_values_with_start ( _str, _start ):
    _str = _str.strip()
    ret = []
    index = _start
    while index < len(_str):
        if _str[index] == '"':
            foundString, index = parse_string ( _str, index )
            ret.append ( foundString )

        elif _str[index] in string.digits + '-':
            foundValue, index = parse_int_or_double_or_hex ( _str, index )
            ret.append ( foundValue )

        elif _str[index] == '[':
            foundList, index = parse_values_with_start ( _str, index + 1 )
            ret.append ( foundList )

        elif _str[index] in string.whitespace:
            index += 1

        elif _str[index] in string.ascii_letters:
            identifierValue, index = parse_identifier ( _str, index )
            ret.append ( identifierValue )

        elif _str[index] == ']':
            return ret, index + 1

        else:
            raise ParseException ( 'Could not parse character %i ("%s") of the string "%s".' % (index, _str[index], _str) )

    return ret, index

def parse_values ( _str ):
    '''
    >>> parse_values ( 'true' )
    ([True], 5)

    >>> parse_values ( '123' )
    ([123], 4)

    >>> parse_values ( '[123]' )
    ([[123]], 5)

    >>> parse_values ( '[]' )
    ([[]], 2)

    >>> parse_values ( '' )
    ([], 0)

    >>> parse_values ( '34.5 "ostemad" [1 2 3 4 5 6]' )
    ([34.5, 'ostemad', [1, 2, 3, 4, 5, 6]], 28)

    >>> parse_values ( '[][[][]]' )
    ([[], [[], []]], 8)
    '''

    _str = _str.strip()
    return parse_values_with_start(_str, 0)

class AutomationObject ( object ):
    def __init__ ( self, _client, _name, _parent = None ):
        self.m_client = _client
        self.m_name = _name
        self.m_parent = _parent
        self.m_info = None

    @property
    def name ( self ):
        return self.m_name

    @property
    def fullname ( self ):
        if self.m_parent is None:
            return '/'
        else:
            parentFullName = self.m_parent.fullname
            if parentFullName.endswith('/'):
                return parentFullName + self.name
            else:
                return parentFullName + '/' + self.name

    @property
    def parent ( self ):
        return self.m_parent

    @property
    def type ( self ):
        self._get_info_if_needed ()

        return self.m_info[0]

    @property
    def returnType ( self ):
        self._get_info_if_needed ()

        if len(self.m_info) > 1:
            return self.m_info[1]
        else:
            return None

    def _get_value ( self ):
        if self.returnType is not None:
            if self.type == 'ReadableProperty' or self.type == 'ReadWriteProperty':
                response = self.m_client.request('GET ' + self.fullname)
                assert response.startswith('+')
                rawValue = response[1:].strip()
                values = parse_values(rawValue)[0]
                if len(values) == 1:
                    return values[0]
                else:
                    return values

#                if self.returnType == 'Boolean':
#                    return bool(self.returnType)
#                elif self.returnType == 'Integer':
#                    return int(self.returnType)
        return None

    def _set_value ( self, _newValue ):
        response = self.m_client.request('SET ' + self.fullname + ' ' + format_value(_newValue) )
        assert response.strip() == '+', response

    value = property(_get_value, _set_value)

    @property
    def stringValue ( self ):
        return unicode(self.value)

    def _get_info_if_needed ( self ):
        if self.m_info is None:
            response = self.m_client.request('INFO ' + self.fullname)
            assert response.startswith('+INFO ')
            self.m_info = response.split(' ')[1:]

    @property
    def children ( self ):
        if self.type == 'List':
            response = self.m_client.request('LIST ' + self.fullname)
            assert response.startswith('+LIST ')
            for childName in response.split(' ')[1:]:
                yield AutomationObject(self.m_client, childName, self)

class Client ( object ):
    def __init__ ( self, _io ):
        self.m_io = _io

    @property
    def root ( self ):
        return AutomationObject ( self, '<ROOT>' )

    def request ( self, _request ):
        print 'REQ:', _request
        self.m_io.write ( unicode(_request + '\n') )
        self.m_io.flush()
        line = self.m_io.readline().strip()
        print 'RESP', line
        return line
