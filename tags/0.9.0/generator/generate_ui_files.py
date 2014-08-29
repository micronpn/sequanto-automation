#!/usr/bin/env python

from __future__ import with_statement
import sys
import re
import os
from os import path

if len(sys.argv) != 3:
    print 'ERROR:'
    print '   %s <wiki dir> <server/c/ dir>' % sys.argv[0]
    sys.exit(-1)

wiki_dir = sys.argv[1]
server_dir = sys.argv[2]

code_dir = 'code'
if path.isdir ( path.join ( path.dirname(__file__), 'code' ) ):
    code_dir = path.join ( path.dirname(__file__), 'code' )

def get_capped_name ( name ):
    ret = ''
    for c in name:
        if c.isupper():
            if ret == '':
                ret = c
            else:
                ret += '_%s' % c
        else:
            ret += c.upper()
    return ret

sq_widget_types = ''
sq_widget_types_variables = ''
sq_widget_nodes_variables = ''

sq_widget_types_const = ''
sq_widget_types_enum_func = ''
sq_widget_nodes_const = ''


FIRST_TABLE_CELL_RE = re.compile ( r'\|\|\s*(\w+).*' )
cont = open ( path.join ( wiki_dir, 'BranchTypesUiTypes.wiki' ), 'r').read()

for m in FIRST_TABLE_CELL_RE.finditer ( cont ):
    name = m.group(1)
    name_capped = get_capped_name ( name )
    
    sq_widget_types += '      SQ_WIDGET_TYPE_%s,\n' % name_capped
    sq_widget_types_variables += '   extern const char * const SQ_WIDGET_TYPE_%s_STRING SQ_CONST_VARIABLE;\n' % name_capped
    sq_widget_types_const += 'const char * const SQ_WIDGET_TYPE_%s_STRING SQ_CONST_VARIABLE = "%s";\n' % (name_capped, name)
    sq_widget_types_enum_func += '   case SQ_WIDGET_TYPE_%s:\n' % name_capped
    sq_widget_types_enum_func += '      return SQ_WIDGET_TYPE_%s_STRING;\n' % name_capped
    sq_widget_types_enum_func += '\n'


RE_HEADER_LINE = re.compile ( r'(\=\s*.+?\s*\=\n)' )
RE_HEADER_NAME = re.compile ( r'\=\s*(?P<name>.+?)\s*\=' )

for fn in os.listdir ( wiki_dir ):
    if fn.startswith('BranchTypesUi') and fn.endswith('.wiki'):
        cont = open ( path.join ( wiki_dir, fn ), 'r' ).read()
        if cont.find ( '#labels Branch-Type-Class' ) != -1:
            klass = fn[len('BranchTypesUi'):-len('.wiki')]
            klass_capped = get_capped_name(klass)
            
            parts = RE_HEADER_LINE.split ( cont )
            current = None
            for part in parts:
                m = RE_HEADER_NAME.match ( part )
                if m:
                    current = m.group('name')
                else:
                    if current:
                        if current == 'Properties':
                            for m in FIRST_TABLE_CELL_RE.finditer ( part.strip() ):
                                node = m.group(1)
                                node_capped = get_capped_name ( node )
                                
                                sq_widget_nodes_variables += '   extern const char * const SQ_UI_%s_%s SQ_CONST_VARIABLE;\n' % (klass_capped, node_capped)
                                sq_widget_nodes_const += 'const char * const SQ_UI_%s_%s SQ_CONST_VARIABLE = "%s";\n' % (klass_capped, node_capped, node)
                                
                        elif current == 'Callables':
                            for m in FIRST_TABLE_CELL_RE.finditer ( part.strip() ):
                                node = m.group(1)
                                node_capped = get_capped_name ( node )
                                
                                sq_widget_nodes_variables += '   extern const char * const SQ_UI_%s_%s SQ_CONST_VARIABLE;\n' % (klass_capped, node_capped)
                                sq_widget_nodes_const += 'const char * const SQ_UI_%s_%s SQ_CONST_VARIABLE = "%s";\n' % (klass_capped, node_capped, node)
                        
                        elif current == 'Monitors':
                            for m in FIRST_TABLE_CELL_RE.finditer ( part.strip() ):
                                node = m.group(1)
                                node_capped = get_capped_name ( node )
                                
                                sq_widget_nodes_variables += '   extern const char * const SQ_UI_%s_%s SQ_CONST_VARIABLE;\n' % (klass_capped, node_capped)
                                sq_widget_nodes_const += 'const char * const SQ_UI_%s_%s SQ_CONST_VARIABLE = "%s";\n' % (klass_capped, node_capped, node)

# remove the comma from the last entry
sq_widget_types = sq_widget_types[:-2]
sq_widget_types_variables = sq_widget_types_variables[:-1]
sq_widget_types_const = sq_widget_types_const[:-1]

ui_h = open(path.join ( code_dir, 'c', 'ui.h' ), 'r').read()
ui_h = ui_h.replace ( '##SQ_WIDGET_TYPES##', sq_widget_types )
ui_h = ui_h.replace ( '##SQ_WIDGET_TYPES_VARIABLES##', sq_widget_types_variables )
ui_h = ui_h.replace ( '##SQ_WIDGET_NODES_VARIABLES##', sq_widget_nodes_variables )

fp = open ( path.join(server_dir, 'include', 'sequanto', 'ui.h' ), 'w' )
fp.write ( ui_h )
fp.close()

sq_ui_c = open(path.join ( code_dir, 'c', 'sq_ui.c' ), 'r').read()
sq_ui_c = sq_ui_c.replace ( '##SQ_WIDGET_TYPES_CONST##', sq_widget_types_const )
sq_ui_c = sq_ui_c.replace ( '##SQ_WIDGET_TYPES_ENUM_FUNC##', sq_widget_types_enum_func )
sq_ui_c = sq_ui_c.replace ( '##SQ_WIDGET_NODES_CONST##', sq_widget_nodes_const )

fp = open ( path.join(server_dir, 'src', 'sq_ui.c' ), 'w' )
fp.write ( sq_ui_c )
fp.close()
