/*
 * Copyright 2010 Rasmus Toftdahl Olesen <rasmus@sequanto.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 */

#ifndef SEQUANTO_QT_LOGGING_H_
#define SEQUANTO_QT_LOGGING_H_

#include <sequanto/mutex.h>
#include <sequanto/readwritepropertynode.h>
#include <fstream>

namespace sequanto
{
   namespace automation
   {
       class QtLoggingEnabledProperty : public BooleanPropertyNode
       {
       protected:
           virtual bool GetValue();
           virtual void SetValue( bool _value );
           
       public:
           QtLoggingEnabledProperty();
       };

       class QtLoggingFilenameProperty : public StringPropertyNode
       {
       protected:
           virtual const std::string & GetValue();
           virtual void SetValue( const std::string & _value );
           
       public:
           QtLoggingFilenameProperty();
       };
       
       class QtLogging
       {
       private:
           static Mutex s_mutex;
           static std::ofstream * s_stream;
           static std::string s_filename;
           
       public:
           friend class QtLoggingFilenameProperty;
           
           static bool s_enabled;
           
           static void logText ( const char * _file, int _line, const char * _function, const char * _text );
       };
   }
}

#define SQ_QT_LOG_TEXT(text) if ( QtLogging::s_enabled ) { QtLogging::logText(__FILE__,__LINE__,__FUNCTION__,text); }

#endif
