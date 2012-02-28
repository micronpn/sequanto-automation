#include <sequanto/QtUnnamedObjectStore.h>
#include <iostream>
#include <sstream>

using namespace std;
using namespace sequanto::automation;

QtUnnamedObjectStore::QtUnnamedObjectStore ()
    : m_nextNameId ( 0 )
{
}

QtUnnamedObjectStore & QtUnnamedObjectStore::Instance ()
{
   static QtUnnamedObjectStore instance;
   return instance;
}

string QtUnnamedObjectStore::CreateName ( const QObject * _object, size_t _num )
{
   stringstream ss;
   ss << _object->metaObject()->className();
   ss << '_';
   ss << _num;
   return ss.str();
}

const string & QtUnnamedObjectStore::GetName ( const QObject * _object )
{
    Q_ASSERT ( _object->thread() == QThread::currentThread() );
    
   QtUnnamedObjectStore & instance = Instance();

   ObjectMap::iterator it = instance.m_map.find ( _object );
   if ( it == instance.m_map.end () )
   {
     string name ( CreateName ( _object, instance.m_nextNameId++ ) );
     pair<ObjectMap::iterator, bool> foundIt = instance.m_map.insert ( make_pair ( _object, name ) );
     Q_ASSERT ( foundIt.second );
     it = foundIt.first;
   }
   return it->second;
/*
   string className ( _object->metaObject()->className() );
   TypeToObjectsMap::iterator typeIt = instance.m_map.find ( className );
   if ( typeIt == instance.m_map.end() )
   {
      TypeToObjectsMap::mapped_type initialValue;
      initialValue.first = 0;
      TypeToObjectsMap::iterator typeIt = instance.m_map.insert ( TypeToObjectsMap::value_type(className, initialValue) ).first;

      string name ( CreateName(_object->metaObject(), 0) );
      ObjectMap::iterator it  = typeIt->second.second.insert ( make_pair(_object, name) ).first;
      return it->second;
   }
   ObjectMap::iterator it = typeIt->second.second.find ( _object );
   if ( it == typeIt->second.second.end() )
   {
      typeIt->second.first++;
      string name ( CreateName ( _object->metaObject(), typeIt->second.first ) );
      it = typeIt->second.second.insert ( make_pair(_object, name ) ).first;

   }
   return it->second;
*/
}

bool QtUnnamedObjectStore::IsKnown ( const QObject * _object )
{
   QtUnnamedObjectStore & instance = Instance();

   return instance.m_map.find ( _object ) != instance.m_map.end();
/*
   TypeToObjectsMap::iterator typeIt = instance.m_map.find ( _object->metaObject()->className() );
   if ( typeIt != instance.m_map.end() )
   {
      return typeIt->second.second.find ( _object ) != typeIt->second.second.end();
   }
   else
   {
      return false;
   }
*/
}

QtUnnamedObjectStore::ObjectMap::size_type QtUnnamedObjectStore::CachedObjects ()
{
   return Instance().m_map.size();
}

void QtUnnamedObjectStore::Deleted ( const QObject * _object )
{
   Instance().m_map.erase ( _object );
/*
   TypeToObjectsMap::iterator typeIt = instance.m_map.find ( _object->metaObject()->className() );
   if ( typeIt != instance.m_map.end() )
   {
      ObjectMap::iterator it = typeIt->second.second.find ( _object );
      if ( it != typeIt->second.second.end() )
      {
         typeIt->second.second.erase ( it );
      }
   }
*/
}
