void windows_init_subsystem ();
void windows_deinit_subsystem ();

struct Process
{
   int m_id;
   char * m_name;
   char * m_filename;
   int m_owner;
   char * m_cmdline;
   int m_memoryUsage;
};

void process_resize_internal_buffer( size_t _numberOfProcesses );
struct Process * process_get_process_internal ( size_t _index );
void process_refresh_internal();
