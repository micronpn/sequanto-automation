#include <string>
#include <map>
#include <QtCore>

namespace sequanto
{
   namespace automation
   {
      class QtUnnamedObjectStore
      {
      private:
         typedef std::map<const QObject*, std::string> ObjectMap;
         
         size_t m_nextNameId;
         ObjectMap m_map;

         static std::string CreateName ( size_t _num );

         QtUnnamedObjectStore ();
         static QtUnnamedObjectStore & Instance();
         
      public:
         static const std::string & GetName ( const QObject * _object );
         static void Deleted ( const QObject * _object );
         static bool IsKnown ( const QObject * _object );
         static ObjectMap::size_type CachedObjects ();
      };
   }
}
