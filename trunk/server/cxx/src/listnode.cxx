#include <sequanto/listnode.h>
#include <sequanto/protocol.h>
#include <sequanto/server.h>
#include <sequanto/lock.h>
#include <stdexcept>

using namespace std;
using namespace sequanto::automation;

class ListNodeInfo : public NodeInfo
{
   Type GetType () const
   {
      return NodeInfo::SQ_NODE_TYPE_LIST;
   }
};

class ListNodeIterator : public Node::Iterator
{
private:
   ListNode::NodeMap::const_iterator m_current;
   ListNode::NodeMap::const_iterator m_end;
   Lock m_lock;
   
public:
   ListNodeIterator ( const Mutex & _mutex, ListNode::NodeMap::const_iterator _begin, ListNode::NodeMap::const_iterator _end )
      : m_current ( _begin ),
        m_end( _end ),
        m_lock ( _mutex )
   {
   }
   
   virtual bool HasNext () const
   {
      return m_current != m_end;
   }
   
   virtual Node * GetCurrent () const
   {
      return (*m_current).second;
   }
   
   virtual void Next ()
   {
      m_current ++;
   }
   
   virtual ~ListNodeIterator ()
   {
   }
};

ListNode::ListNode ( const std::string & _name )
   : Node(_name),
     m_children (),
     m_mutex ()
{
}

const NodeInfo & ListNode::Info () const
{
   static ListNodeInfo info;
   return info;
}

Node * ListNode::FindChild ( const std::string & _name ) const
{
   Lock lock ( m_mutex );
   NodeMap::const_iterator it = m_children.find ( _name );
   if ( it == m_children.end() )
   {
      return NULL;
   }
   else
   {
      return it->second;
   }
}

Node::Iterator * ListNode::ListChildren () const
{
   Lock lock ( m_mutex );

   return new ListNodeIterator ( m_mutex, m_children.begin(), m_children.end() );
}

void ListNode::AddChild ( Node * _child )
{
   Lock lock ( m_mutex );
   if ( this->HasChild(_child->GetName()) )
   {
      throw std::runtime_error ( "SequantoAutomation_CXX: This ListNode already contains a child with the same name." );
   }
   m_children[_child->GetName()] = _child;
   _child->SetParent ( this );
}

void ListNode::RemoveChild ( std::string _name )
{
   Lock lock ( m_mutex );
   NodeMap::iterator it = m_children.find(_name);
   if ( it != m_children.end() )
   {
      it->second->SendRemove ();
      delete it->second;
      m_children.erase ( it );
   }
}

void ListNode::RemoveChild ( Node * _child )
{
   Lock lock ( m_mutex );
   for ( NodeMap::iterator it = m_children.begin(); it != m_children.end(); ++it )
   {
      if ( it->second == _child )
      {
          m_children.erase ( it );
          break;
      }
   }
}

bool ListNode::HasChild ( std::string _name )
{
   Lock lock ( m_mutex );
   return m_children.find ( _name ) != m_children.end();
}

void ListNode::SendUpdate ()
{
   std::string fullname ( this->GetFullName() );
   SQServer * server = sq_server_get_instance();
   sq_protocol_write_update_message ( server->m_stream, fullname.c_str() );
}

ListNode::~ListNode()
{
   Lock lock ( m_mutex );
   
   for ( NodeMap::iterator it = m_children.begin(); it != m_children.end(); ++it )
   {
      delete it->second;
   }
   m_children.clear();
}
