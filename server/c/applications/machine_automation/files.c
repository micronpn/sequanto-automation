#include <sequanto/automation.h>
#include <stdio.h>

#include "config.h"

#ifdef SQ_WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

char * files_info ( const char * _path, int * _numberOfDirectories, int * _numberOfFiles, int _directoryIndex, int _fileIndex )
{
   char * pathWithWildcard;
   WIN32_FIND_DATA item;
   HANDLE findHandle;
   size_t pathlen;
   char * found_filename;

   if ( strcmp(_path, "") == 0 )
   {
      *_numberOfDirectories = 1;
      *_numberOfFiles = 0;
      if ( _directoryIndex == 0 )
      {
         return strdup("C:");
      }
      else
      {
         return NULL;
      }
   }
   pathlen = strlen(_path);
   pathWithWildcard = (char*) malloc ( pathlen + 3 );
   strcpy_s ( pathWithWildcard, pathlen + 3, _path );
   pathWithWildcard[pathlen] = '\\';
   pathWithWildcard[pathlen + 1] = '*';
   pathWithWildcard[pathlen + 2] = '\0';
   findHandle = FindFirstFile ( pathWithWildcard, &item );

   *_numberOfDirectories = 0;
   *_numberOfFiles = 0;

   if ( findHandle != INVALID_HANDLE_VALUE )
   {
      do
      {
         if ( strcmp(item.cFileName, ".") != 0 && strcmp(item.cFileName, "..") != 0 )
         {
            if (item.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
               if ( *_numberOfDirectories == _directoryIndex )
               {
                  found_filename = _strdup ( item.cFileName );
                  FindClose ( findHandle );
                  return found_filename;
               }
               *_numberOfDirectories = *_numberOfDirectories + 1;
            }
            else
            {
               if ( *_numberOfFiles == _fileIndex )
               {
                  found_filename = _strdup ( item.cFileName );
                  FindClose ( findHandle );
                  return found_filename;
               }
               *_numberOfFiles = *_numberOfFiles + 1;
            }
         }
      }
      while (FindNextFile(findHandle, &item) != 0);
      FindClose ( findHandle );
   }
   free ( pathWithWildcard );
   return NULL;
}

static char PATH_SEP[] = "\\";
const char * files_separator ()
{
   return PATH_SEP;
}

size_t files_size ( const char * _path )
{
   LARGE_INTEGER ret;
   HANDLE fileHandle;
   fileHandle = CreateFile ( _path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL );
   if ( fileHandle != INVALID_HANDLE_VALUE )
   {
      GetFileSizeEx ( fileHandle, &ret );
      CloseHandle ( fileHandle );
      return (size_t) ret.QuadPart;
   }
   else
   {
      return 0;
   }
}

void files_create_directory ( const char * _path )
{
   CreateDirectory ( _path, NULL );
}

void files_remove_directory ( const char * _path )
{
   RemoveDirectory ( _path );
}

void files_remove_file ( const char * _path )
{
   DeleteFile ( _path );
}

#else /* SQ_WIN32 */

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

static char PATH_SEP[] = "/";
const char * files_separator ()
{
   return PATH_SEP;
}

char * files_info ( const char * _path, int * _numberOfDirectories, int * _numberOfFiles, int _directoryIndex, int _fileIndex )
{
   DIR * dir;
   struct dirent * entry;
   char * found_filename = NULL;
   size_t pathLen = strlen(_path);
   char * actual_dir = malloc ( 1 + pathLen + 1 );
   actual_dir[0] = '/';
   memcpy ( actual_dir + 1, _path, pathLen + 1 );
   *_numberOfDirectories = 0;
   *_numberOfFiles = 0;
   
   dir = opendir ( actual_dir );
   if ( dir != NULL )
   {
       while ( entry = readdir ( dir ) )
       {
           if ( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
           {
               if (entry->d_type == DT_DIR )
               {
                   if ( *_numberOfDirectories == _directoryIndex )
                   {
                       found_filename = strdup ( entry->d_name );
                       break;
                   }
                   *_numberOfDirectories = *_numberOfDirectories + 1;
               }
               else
               {
                   if ( *_numberOfFiles == _fileIndex )
                   {
                       found_filename = strdup ( entry->d_name );
                       break;
                   }
                   *_numberOfFiles = *_numberOfFiles + 1;
            }
         }
      }
      closedir ( dir );
   }
   free ( actual_dir );
   return found_filename;
}

size_t files_size ( const char * _path )
{
    struct stat buf;
    if ( stat ( _path, &buf ) == 0 )
    {
        return buf.st_size;
    }
    else
    {
        return 0;
    }
}

void files_create_directory ( const char * _path )
{
   mkdir ( _path, 0 );
}

void files_remove_directory ( const char * _path )
{
   rmdir ( _path );
}

void files_remove_file ( const char * _path )
{
   unlink ( _path );
}

#endif /* SQ_WIN32 */

int files_number_of_directories ( const char * _path )
{
   int numberOfDirectories, numberOfFiles;
   
   files_info ( _path, &numberOfDirectories, &numberOfFiles, -1, -1 );

   return numberOfDirectories;
}

int files_number_of_files ( const char * _path )
{
   int numberOfDirectories, numberOfFiles;
   
   files_info ( _path, &numberOfDirectories, &numberOfFiles, -1, -1 );

   return numberOfFiles;
}

char * files_filename ( const char * _path, int _index )
{
   int numberOfDirectories, numberOfFiles;
   
   char * found_filename = files_info ( _path, &numberOfDirectories, &numberOfFiles, -1, _index );
   
   return found_filename;
}

char * files_directoryname ( const char * _path, int _index )
{
   int numberOfDirectories, numberOfFiles;
   
   char * found_filename = files_info ( _path, &numberOfDirectories, &numberOfFiles, _index, -1 );
   
   return found_filename;
}

SQByteArray * files_read ( const char * _path )
{
   SQByteArray * ret;
   FILE * fp = NULL;
   size_t file_size = files_size ( _path );

   ret = sq_byte_array_create_prealloc ( file_size );
   fp = fopen ( _path, "rb" );
   ret->m_length = file_size;
   fread ( ret->m_start, sizeof(SQByte), file_size, fp );
   fclose ( fp );
   return ret;
}

void files_write ( const char * _path, SQByteArray * _value )
{
   FILE * fp;
   fp = fopen ( _path, "wb" );
   fwrite ( _value->m_start, sizeof(SQByte), _value->m_length, fp );
   fclose ( fp );
}
