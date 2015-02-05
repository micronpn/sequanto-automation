#!/usr/bin/python
try:
    from sequanto.automation import Client, AutomationObject
except:
    import sys
    from os import path
    sys.path.insert ( 0, path.join ( path.dirname(__file__), '..', 'lib' ) )
    from sequanto.automation import Client, AutomationObject

import serial
from gi.repository import Gtk
import io

def ask_for_string ( title ):
    dialog = Gtk.Dialog(title = title)
    dialog.add_button ( 'OK', 1 )
    dialog.set_default_response ( 1 )
    entry = Gtk.Entry()
    entry.set_property ( 'activates-default', True )
    dialog.vbox.add ( entry )
    dialog.show_all()
    dialog.run()
    text = entry.get_text()
    dialog.destroy()
    return text

class ClientWindow ( Gtk.Window ):
    def __init__ ( self ):
        Gtk.Window.__init__( self, title = 'Sequanto Automation Client' )
        self.connect ( 'delete-event', Gtk.main_quit )
        self.resize ( 600, 500 )

        self.store = Gtk.TreeStore ( object, str, str, str )
        self.tree = Gtk.TreeView ( self.store )
        self.name_column = Gtk.TreeViewColumn('Name', Gtk.CellRendererText(), text = 1)
        self.type_column = Gtk.TreeViewColumn('Type', Gtk.CellRendererText(), text = 2)
        self.value_column = Gtk.TreeViewColumn('Value', Gtk.CellRendererText(), text = 3)
        self.tree.append_column ( self.name_column )
        self.tree.append_column ( self.type_column )
        self.tree.append_column ( self.value_column )
        self.tree.connect ( 'row-activated', self.row_activated )

        scroll = Gtk.ScrolledWindow()
        scroll.add ( self.tree )
        self.add ( scroll )

        self.client = None

    def connect_client ( self, io ):
        self.client = Client(io)
        self.store.clear()
        self.add_node ( self.client.root )

    def add_node ( self, automation_object, tree_parent = None ):
        node = self.store.append ( tree_parent )
        #self.store.set_value ( node, self.name_column, automation_object.name )
        self.store.set_value ( node, 0, automation_object )
        self.store.set_value ( node, 1, automation_object.name )
        if automation_object.returnType is not None:
            self.store.set_value ( node, 2, automation_object.returnType )
        else:
            self.store.set_value ( node, 2, automation_object.type )
        self.store.set_value ( node, 3, automation_object.stringValue )

        for childObj in automation_object.children:
            self.add_node ( childObj, node )

    def row_activated ( self, view, path, column ):
        it = self.store.get_iter(path)
        automation_object = self.store.get_value ( it, 0 )
        if automation_object.returnType == 'Boolean':
            automation_object.value = not automation_object.value

        elif automation_object.returnType == 'String':
            automation_object.value = ask_for_string ( 'Set %s to:' % automation_object.fullname )

        self.store.set_value ( it, 3, automation_object.stringValue )

win = ClientWindow()
win.show_all ()

ser = serial.Serial('/dev/ttyACM0', 57600, timeout = 1)

class SerialWrapper ( io.TextIOBase ):
    def __init__ ( self, _serial ):
        io.TextIOBase.__init__ ( self )
        self.m_serial = _serial

    def readline ( self ):
        ret = ''
        while True:
            c = self.m_serial.read(1)
            if c == '\n':
                return ret
            elif c == '\r':
                continue
            else:
                ret += c

    def write ( self, _data ):
        self.m_serial.write ( _data )

#win.connect_client ( io.TextIOWrapper(ser, ser) ) #io.BufferedRWPair(ser, ser), newline = u'\n' ) )
win.connect_client ( SerialWrapper(ser) )
Gtk.main()
