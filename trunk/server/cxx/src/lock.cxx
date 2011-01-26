#include <sequanto/lock.h>

using namespace sequanto::automation;

Lock::Lock ( const Mutex & _mutex )
   : m_mutex(_mutex)
{
   sq_mutex_enter ( m_mutex.m_mutex );
}

Lock::~Lock ()
{
   sq_mutex_leave ( m_mutex.m_mutex );
}
