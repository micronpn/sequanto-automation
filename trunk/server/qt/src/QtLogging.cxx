#include <sequanto/lock.h>
#include <sequanto/QtLogging.h>
#include <QtCore>

using namespace sequanto::automation;

bool QtLogging::s_enabled = false;
std::ofstream * QtLogging::s_stream = NULL;
std::string QtLogging::s_filename;
Mutex QtLogging::s_mutex;

void QtLogging::logText ( const char * _file, int _line, const char * _function, const char * _text )
{
    if ( s_enabled )
    {
        Lock lock(s_mutex);
        if ( s_stream == NULL )
        {
            qDebug() << _file << ":" << _line << " " << _function << " " << _text;
        }
        else
        {
            (*s_stream) << _file << ":" << _line << " " << _function << " " << _text << std::endl;;
        }
    }
}

QtLoggingFilenameProperty::QtLoggingFilenameProperty()
    : StringPropertyNode("filename")
{
}

const std::string & QtLoggingFilenameProperty::GetValue ()
{
    static std::string ret;
    
    Lock lock ( QtLogging::s_mutex );
    
    ret = QtLogging::s_filename;
    
    return ret;
}

void QtLoggingFilenameProperty::SetValue ( const std::string & _value )
{
    Lock lock ( QtLogging::s_mutex );
    
    if ( QtLogging::s_filename != _value )
    {
        QtLogging::s_filename = _value;
        if ( QtLogging::s_stream != NULL )
        {
            delete QtLogging::s_stream;
        }
        QtLogging::s_stream = new std::ofstream ( QtLogging::s_filename.c_str(), std::ios::app | std::ios::out );
        
        SendUpdate();
    }
}

QtLoggingEnabledProperty::QtLoggingEnabledProperty()
    : BooleanPropertyNode("enabled")
{
}

bool QtLoggingEnabledProperty::GetValue ()
{
    return QtLogging::s_enabled;
}

void QtLoggingEnabledProperty::SetValue ( bool _value )
{
    QtLogging::s_enabled = _value;

    SendUpdate();
}
