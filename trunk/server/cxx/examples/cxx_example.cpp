#include <vector>

#include <sequanto/automation.h>
#include <sequanto/thread.h>
#include <sequanto/tree.h>
#include <cstdlib>
#include <cstdio>

#include "cxx_example_automation.h"
#include "config.h"

using namespace std;
using namespace sequanto::automation;

static Tree tree;

extern "C"
{
   int test_me ( int _x, int _y )
   {
      return -1;
   }

   SQBool sq_handle_branch_info_cxx ( SQStream * _stream, const char * _objectPath )
   {
      return tree.HandleInfo ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_list_cxx ( SQStream * _stream, const char * _objectPath )
   {
      return tree.HandleList ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_get_cxx ( SQStream * _stream, const char * _objectPath )
   {
      return tree.HandleGet ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_set_cxx ( SQStream * _stream, const char * _objectPath, const SQValue * const _value )
   {
      return tree.HandleSet ( _stream, _objectPath, _value );
   }

   SQBool sq_handle_branch_disable_cxx ( SQStream * _stream, const char * _objectPath )
   {
      return tree.HandleDisable ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_enable_cxx ( SQStream * _stream, const char * _objectPath )
   {
      return tree.HandleEnable ( _stream, _objectPath );
   }

   SQBool sq_handle_branch_call_cxx ( SQStream * _stream, const char * _objectPath, const SQValue * const _values, int _numberOfValues )
   {
      return tree.HandleCall ( _stream, _objectPath, _values, _numberOfValues );
   }
}


class MyTestObject
{
private:
   std::string m_name;
   int m_x;
   int m_y;

public:
   MyTestObject ( const std::string & _name ) : m_name(_name) {}
   const std::string & Name() { return m_name; }
   void SetName ( const std::string & _name ) { m_name = _name; }
   int X() { return m_x; }
   void SetX ( int _x ) { m_x = _x; }
   int Y() { return m_y; }
   void SetY ( int _y ) { m_y = _y; }
};

class MyTestObject_X_Property : public IntegerPropertyNode
{
private:
   MyTestObject * m_self;

public:
   MyTestObject_X_Property  ( MyTestObject * _self )
      : IntegerPropertyNode ( "X" ),
        m_self ( _self )
   {
   }

   virtual int GetValue ()
   {
      return m_self->X();
   }

   virtual void SetValue ( int _newValue )
   {
      m_self->SetX ( _newValue );
   }
};

class MyTestObject_Y_Property : public IntegerPropertyNode
{
private:
   MyTestObject * m_self;

public:
   MyTestObject_Y_Property  ( MyTestObject * _self )
      : IntegerPropertyNode ( "Y" ),
        m_self ( _self )
   {
   }

   virtual int GetValue ()
   {
      return m_self->Y();
   }

   virtual void SetValue ( int _newValue )
   {
      m_self->SetY ( _newValue );
   }
};

class MyTestObject_Name_Property : public StringPropertyNode
{
private:
   MyTestObject * m_self;

public:
   MyTestObject_Name_Property  ( MyTestObject * _self )
      : StringPropertyNode ( "Name" ),
        m_self ( _self )
   {
   }

   virtual const std::string & GetValue ()
   {
      return m_self->Name();
   }

   virtual void SetValue ( const std::string & _newValue )
   {
      m_self->SetName ( _newValue );
   }
};

int main ( int _argc, char * _argv[] )
{
   static SQServer server;
    
   sq_init ();   
    
   sq_server_init ( &server, 4321 );

   ListNode root ( "ROOT" );
   tree.SetRoot ( &root );

   vector<MyTestObject*> listOfObjects;
   for ( int i = 0; i < 100; i++ )
   {
      char nameBuffer[10];
#ifdef _WIN32
      ::_itoa_s ( i, nameBuffer, 10, 16 );
#else
      SNPRINTF_FUNCTION ( nameBuffer, 10, "%i", i );
#endif
      ListNode * testObjectContainer = new ListNode ( nameBuffer );

      string name ( "My Test Object (" );
      name += nameBuffer;
      name += ")";
      MyTestObject * testObject = new MyTestObject(name);
      
      listOfObjects.push_back ( testObject );

      testObjectContainer->AddChild ( new MyTestObject_X_Property(testObject) );
      testObjectContainer->AddChild ( new MyTestObject_Y_Property(testObject) );
      testObjectContainer->AddChild ( new MyTestObject_Name_Property(testObject) );

      root.AddChild ( testObjectContainer );
   }

   while ( SQ_TRUE )
   {
      if ( sq_thread_is_supported() )
      {
         sq_system_sleep ( 1000 );
      }
      sq_server_poll ( &server );
   }

   sq_shutdown ();   
}
