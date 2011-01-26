#include <sequanto/mutex.h>

using namespace sequanto::automation;

Mutex::Mutex ()
   : m_mutex(sq_mutex_create())
{
}

Mutex::~Mutex ()
{
   sq_mutex_destroy ( m_mutex );
   m_mutex = NULL;
}
