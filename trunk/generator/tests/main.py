import unittest
import doctest

import sys
from os import path
sys.path.append ( path.join ( path.dirname(__file__), '..', 'lib' ) )

import test_c_codeparser

test_runner = unittest.TextTestRunner()
test_runner.run ( unittest.TestSuite([ test_c_codeparser.suite(),
#                                       doctest.DocTestSuite(automation)
                                       ]) )
