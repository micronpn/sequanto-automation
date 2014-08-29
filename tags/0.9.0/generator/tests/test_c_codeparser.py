import unittest
import re
from sequanto_automation.codeparser import get as get_code_parser


class Test_C_CodeParser ( unittest.TestCase ):
    def assertEmpty ( self, _collection ):
        self.assert_ ( len(_collection) == 0, 'The collection is not empty, it contains %i elements' % len(_collection) )
    
    def assertLength ( self, _collection, _expectedLength ):
        self.assert_ ( len(_collection) == _expectedLength, 'The collection contains %i elements, not the expected %i' % (len(_collection), _expectedLength) )
    
    def setUp ( self ):
        self.parser = get_code_parser ( 'c' )
        
    def testGetParserWorks ( self ):
        self.assert_ ( self.parser )
    
    def testIdentifierRe ( self ):
        r = re.compile('^%s$' % self.parser.RE_IDENTIFIER)
        self.assert_ ( r.match('ostemad') )
        
    def testTypeRe ( self ):
        r = re.compile('^%s$' % self.parser.RE_TYPE)
        self.assert_ ( r.match('void ') )
        self.assert_ ( r.match('int ') )
        self.assert_ ( r.match('char[]') )
        self.assert_ ( r.match('char*') )
        self.assert_ ( r.match('void * *') )

    def testBracketsWithSpaceRe ( self ):
        r = self.parser.RE_BRACKET_PAIRS_WITH_SPACE
        self.assert_ ( not r.match('void') )
        self.assertEqual ( r.sub('[]', '[ ]'), '[]' )
        self.assertEqual ( r.sub('[]', '[                       ]'), '[]' )
        self.assertEqual ( r.sub('[]', 'void[                       ]'), 'void[]' )
        self.assertEqual ( r.sub('[]', 'void[                       ][      ]'), 'void[][]' )
        
    def testCleanType ( self ):
        self.assertEqual ( 'void', self.parser.cleanType('void') )
        self.assertEqual ( 'void *', self.parser.cleanType('void*') )
        self.assertEqual ( 'void []', self.parser.cleanType('void [ ]') )
        
    def testParameterRe ( self ):
        r = re.compile('^%s$' % self.parser.RE_PARAMETER)
        self.assert_ ( r.match('void ostemad') )
        self.assert_ ( r.match('char[] t0') )
        self.assert_ ( r.match('char* argv') )
        self.assert_ ( r.match('void**myparameter_has_a_very_long_name') )
        
    def testEnumRe ( self ):
        m = self.parser.RE_ENUM.match('enum MyEnum { }' )
        self.assert_ ( m )
        self.assertEqual ( 'MyEnum', m.group('name') )
        self.assertEqual ( '', m.group('values') )
        
        m = self.parser.RE_TYPEDEF_ENUM.match('typedef enum MyEnum { } EnumName;' )
        self.assert_ ( m )
        self.assertEqual ( 'MyEnum', m.group('name') )
        self.assertEqual ( 'EnumName', m.group('typedef_name') )
        self.assertEqual ( '', m.group('values') )

        m = self.parser.RE_TYPEDEF_ENUM.match('typedef enum MyEnum \n{ all\nthe\nvalues\goes\here\n } EnumName;' )
        self.assert_ ( m )
        self.assertEqual ( 'MyEnum', m.group('name') )
        self.assertEqual ( 'EnumName', m.group('typedef_name') )
        self.assertEqual ( 'all\nthe\nvalues\goes\here', m.group('values') )

    def testEnumValuesRe ( self ):
        matches = list ( [self.parser.RE_ENUM_VALUE.match(s) for s in ['A', 'B', 'C', 'D'] ] )
        self.assertLength ( matches, 4 )
        
        matches = list ( [self.parser.RE_ENUM_VALUE.match(s) for s in ['C1 = 23', 'C4 = 123', 'free_libre_ = -1']] )
        self.assertLength ( matches, 3 )
    
    def testEnums ( self ):
        self.parser.parse ( 'void test ()\n{\n}\nenum Enum1 { A, B, C }\n\ntypedef enum _Enum2 { V1 = 1, V2 = 2, V3 = 3 } Enum2;\n' )
        self.assert_ ( self.parser.hasEnum ( 'Enum1' ) )
        self.assert_ ( self.parser.hasEnum ( 'Enum2' ) )
        self.assertEquals ( ['A', 'B', 'C'], list(self.parser.getEnum('Enum1').valueNames) )
        self.assertEquals ( ['V1', 'V2', 'V3'], list(self.parser.getEnum('Enum2').valueNames) )
        self.assertEquals ( ['V1', 'V2', 'V3'], list(self.parser.getEnum('_Enum2').valueNames) )
    
    def testFunctions ( self ):
        self.parser.parse ( 'void test ()' )
        self.assert_ ( self.parser.hasFunction ( 'test' ) )
        function = self.parser.getFunction ( 'test' )
        self.assertEqual ( function.name, 'test' )
        self.assertEqual ( function.returnType, 'void' )
        self.assertEmpty ( function.parameters )
        self.parser.clear()
        self.assertFalse ( self.parser.hasFunction ( 'test' ) )
        
        self.parser.parse ( 'void * test ()' )
        self.assert_ ( self.parser.hasFunction ( 'test' ) )
        function = self.parser.getFunction ( 'test' )
        self.assertEqual ( function.name, 'test' )
        self.assertEqual ( function.returnType, 'void *' )
        self.assertEmpty ( function.parameters )
        self.parser.clear()
        
        self.parser.parse ( 'void[] test ()' )
        self.assert_ ( self.parser.hasFunction ( 'test' ) )
        function = self.parser.getFunction ( 'test' )
        self.assertEqual ( function.name, 'test' )
        self.assertEqual ( function.returnType, 'void []' )
        self.assertEmpty ( function.parameters )
        self.parser.clear()
        
        self.parser.parse ( 'void * * * test ()' )
        self.assert_ ( self.parser.hasFunction ( 'test' ) )
        function = self.parser.getFunction ( 'test' )
        self.assertEqual ( function.name, 'test' )
        self.assertEqual ( function.returnType, 'void * * *' )
        self.assertEmpty ( function.parameters )
        self.parser.clear()
        
        self.parser.parse ( 'void[][][] test ()' )
        self.assert_ ( self.parser.hasFunction ( 'test' ) )
        function = self.parser.getFunction ( 'test' )
        self.assertEqual ( function.name, 'test' )
        self.assertEqual ( function.returnType, 'void [] [] []' )
        self.assertEmpty ( function.parameters )
        self.parser.clear()

        self.parser.parse ( 'char * test ( int _number )' )
        self.assert_ ( self.parser.hasFunction ( 'test' ) )
        function = self.parser.getFunction ( 'test' )
        self.assertEqual ( function.name, 'test' )
        self.assertEqual ( function.returnType, 'char *' )
        self.assertLength ( function.parameters, 1 )
        self.assertEqual ( function.parameters[0].name, '_number' )
        self.assertEqual ( function.parameters[0].type, 'int' )
        self.parser.clear()

        self.parser.parse ( 'char * test ( int _number, char*val, char v, char v, char v )' )
        self.assert_ ( self.parser.hasFunction ( 'test' ) )
        function = self.parser.getFunction ( 'test' )
        self.assertEqual ( function.name, 'test' )
        self.assertEqual ( function.returnType, 'char *' )
        self.assertLength ( function.parameters, 5 )
        self.assertEqual ( function.parameters[0].name, '_number' )
        self.assertEqual ( function.parameters[0].type, 'int' )
        self.assertEqual ( function.parameters[1].name, 'val' )
        self.assertEqual ( function.parameters[1].type, 'char *' )
        self.assertEqual ( function.parameters[2].name, 'v' )
        self.assertEqual ( function.parameters[2].type, 'char' )
        self.assertEqual ( function.parameters[3].name, 'v' )
        self.assertEqual ( function.parameters[3].type, 'char' )
        self.assertEqual ( function.parameters[4].name, 'v' )
        self.assertEqual ( function.parameters[4].type, 'char' )
        self.parser.clear()

        self.parser.parse ( 'char* test ( int _number )\n { printf("This is a really cool function! %i\\n", _number);a = 43 + test(45);\n return NULL;\n }\n' )
        self.assert_ ( self.parser.hasFunction ( 'test' ) )
        function = self.parser.getFunction ( 'test' )
        self.assertEqual ( function.name, 'test' )
        self.assertEqual ( function.returnType, 'char *' )
        self.assertLength ( function.parameters, 1 )
        self.assertEqual ( function.parameters[0].name, '_number' )
        self.assertEqual ( function.parameters[0].type, 'int' )
        self.parser.clear()

        # A function which takes void as the only parameter has an empty parameter list
        self.parser.parse ( 'void test (void)' )
        self.assert_ ( self.parser.hasFunction ( 'test' ) )
        function = self.parser.getFunction ( 'test' )
        self.assertEqual ( function.name, 'test' )
        self.assertEqual ( function.returnType, 'void' )
        self.assertEmpty ( function.parameters )
        self.parser.clear()
        
def suite ():
    return unittest.TestLoader().loadTestsFromTestCase(Test_C_CodeParser)
