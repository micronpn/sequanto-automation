#include <sequanto/automation.h>
#include <stdio.h>

#include "config.h"

#ifdef SQ_WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static char * found_filename = NULL;

void files_info ( const char * _path, int * _numberOfDirectories, int * _numberOfFiles, int _directoryIndex, int _fileIndex )
{
   char * pathWithWildcard;
   WIN32_FIND_DATA item;
   HANDLE findHandle;
   size_t pathlen = strlen(_path);
   pathWithWildcard = malloc ( pathlen + 3 );
   strcpy_s ( pathWithWildcard, pathlen + 3, _path );
   pathWithWildcard[pathlen] = '\\';
   pathWithWildcard[pathlen + 1] = '*';
   pathWithWildcard[pathlen + 2] = '\0';
   findHandle = FindFirstFile ( pathWithWildcard, &item );

   *_numberOfDirectories = 0;
   *_numberOfFiles = 0;

   if ( found_filename != NULL )
   {
      free ( found_filename );
   }
   found_filename = NULL;

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
               }
               *_numberOfDirectories = *_numberOfDirectories + 1;
            }
            else
            {
               if ( *_numberOfFiles == _fileIndex )
               {
                  found_filename = _strdup ( item.cFileName );
               }
               *_numberOfFiles = *_numberOfFiles + 1;
            }
         }
      }
      while (FindNextFile(findHandle, &item) != 0);
      FindClose ( findHandle );
   }
   free ( pathWithWildcard );
}

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
   
   files_info ( _path, &numberOfDirectories, &numberOfFiles, -1, _index );
   
   return found_filename;
}

char * files_directoryname ( const char * _path, int _index )
{
   int numberOfDirectories, numberOfFiles;
   
   files_info ( _path, &numberOfDirectories, &numberOfFiles, _index, -1 );
   
   return found_filename;
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

SQByteArray * files_read ( const char * _path )
{
   SQByteArray * ret;
   FILE * fp = NULL;
   size_t file_size = files_size ( _path );

   ret = sq_byte_array_create_prealloc ( file_size );
   fopen_s ( &fp, _path, "rb" );
   ret->m_length = file_size;
   fread ( ret->m_start, sizeof(SQByte), file_size, fp );
   fclose ( fp );
   return ret;
}

void files_write ( const char * _path, SQByteArray * _value )
{
   FILE * fp;
   fopen_s ( &fp, _path, "wb" );
   fwrite ( _value->m_start, sizeof(SQByte), _value->m_length, fp );
   fclose ( fp );
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

#include <dirent.h>

void files_list ( const char * _path )
{
   
}

#endif /* SQ_WIN32 */
