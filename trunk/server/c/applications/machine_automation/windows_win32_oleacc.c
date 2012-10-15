#include <sequanto/automation.h>

#define COBJMACROS
#include <ObjBase.h>
#include <OAIdl.h>
#include <OleAcc.h>

static VARIANT s_childIdSelf;

void windows_init_subsystem ()
{
   CoInitialize( NULL );

   s_childIdSelf.vt = VT_I4;
   s_childIdSelf.lVal = CHILDID_SELF;
}

void windows_deinit_subsystem ()
{
   CoUninitialize();
}

char * windows_bstr_to_utf8 ( BSTR _buffer )
{
   int length;
   char * ret;
   
   length = WideCharToMultiByte ( CP_UTF8, 0, _buffer, -1, NULL, 0, NULL, NULL );
   
   ret = malloc ( length );
   
   WideCharToMultiByte ( CP_UTF8, 0, _buffer, -1, ret, length, NULL, NULL );
   
   SysFreeString( _buffer );

   return ret;
}

IAccessible * windows_to_accessible ( const SQByteArray * _pointer )
{
   IAccessible * accessibleObject = NULL;
   memcpy ( &accessibleObject, _pointer->m_start, sizeof(IAccessible*) );
   return accessibleObject;
}

SQByteArray * windows_from_accessible ( const IAccessible * _accessibleObject )
{
   SQByteArray * ret = sq_byte_array_create_prealloc ( sizeof(IAccessible*) );
   memcpy ( ret->m_start, &_accessibleObject, sizeof(IAccessible*) );
   return ret;
}

int windows_desktops ( void )
{
    return 1;
}

SQByteArray * windows_desktop ( int _desktop )
{
   HWND desktopHandle;
   void * accessible;
   
   desktopHandle = GetDesktopWindow();
   AccessibleObjectFromWindow ( desktopHandle, OBJID_CLIENT, &IID_IAccessible, &accessible );
   return windows_from_accessible ( (IAccessible*) accessible );
}

void windows_ref ( const SQByteArray * _pointer )
{
   IAccessible * accessible = windows_to_accessible ( _pointer );
   IAccessible_AddRef ( accessible );
}

void windows_unref ( const SQByteArray * _pointer )
{
   IAccessible * accessible = windows_to_accessible ( _pointer );
   IAccessible_Release ( accessible );
}


char * windows_name ( const SQByteArray * _pointer )
{
   IAccessible * accessible;
   BSTR buffer;
   
   accessible = windows_to_accessible ( _pointer );

   IAccessible_get_accName ( accessible, s_childIdSelf, &buffer );
   
   return windows_bstr_to_utf8 ( buffer );
}

char * windows_role ( const SQByteArray * _pointer )
{
   IAccessible * accessible;
   VARIANT roleVar;
   char * buffer = malloc(100);
   
   accessible = windows_to_accessible ( _pointer );

   if ( SUCCEEDED(IAccessible_get_accRole ( accessible, s_childIdSelf, &roleVar ) ) )
   {
       GetRoleText ( roleVar.lVal, buffer, 100 );
   }   
   return buffer;
}

int windows_process_id ( const SQByteArray * _pointer )
{
   IAccessible * accessible;
   
   accessible = windows_to_accessible ( _pointer );

   return 0;
}

int windows_x ( const SQByteArray * _pointer )
{
   IAccessible * accessible;
   VARIANT roleVar;
   long x = -1, y = -1, width = 0, height = 0;
   
   accessible = windows_to_accessible ( _pointer );

   IAccessible_accLocation ( accessible, &x, &y, &width, &height, s_childIdSelf );
   
   return x;
}

int windows_y ( const SQByteArray * _pointer )
{
   IAccessible * accessible;
   VARIANT roleVar;
   long x = -1, y = -1, width = 0, height = 0;
   
   accessible = windows_to_accessible ( _pointer );

   IAccessible_accLocation ( accessible, &x, &y, &width, &height, s_childIdSelf );
   
   return y;
}

int windows_width ( const SQByteArray * _pointer )
{
   IAccessible * accessible;
   VARIANT roleVar;
   long x = -1, y = -1, width = 0, height = 0;
   
   accessible = windows_to_accessible ( _pointer );

   IAccessible_accLocation ( accessible, &x, &y, &width, &height, s_childIdSelf );
   
   return width;
}

int windows_height ( const SQByteArray * _pointer )
{
   IAccessible * accessible;
   VARIANT roleVar;
   long x = -1, y = -1, width = 0, height = 0;
   
   accessible = windows_to_accessible ( _pointer );

   IAccessible_accLocation ( accessible, &x, &y, &width, &height, s_childIdSelf );
   
   return height;
}

char * windows_text ( const SQByteArray * _pointer )
{
   IAccessible * accessible;
   BSTR buffer;
   
   accessible = windows_to_accessible ( _pointer );

   if ( SUCCEEDED(IAccessible_get_accValue ( accessible, s_childIdSelf, &buffer ) ) )
   {
      return windows_bstr_to_utf8 ( buffer );
   }
   else
   {
      return strdup("");
   }
}

static IAccessible * windows_get_child ( IAccessible * _accessible, long _childId )
{
   VARIANT childNum;
   IDispatch * dispChild;
   IAccessible * accessibleChild;
   
   childNum.vt = VT_I4;
   childNum.lVal = _childId;

   if ( SUCCEEDED(IAccessible_get_accChild ( _accessible, childNum, &dispChild )) )
   {
      if ( SUCCEEDED(IDispatch_QueryInterface ( dispChild, &IID_IAccessible, &accessibleChild )) )
      {
         return accessibleChild;
      }
   }
   return NULL;
}

long windows_children ( const SQByteArray * _pointer )
{
   IAccessible * accessible;
   IAccessible * accessibleChild;
   long childCount;
   //long childrenObtained;
   long ret;
   long i;
   //VARIANT * childrenArray;
   
   accessible = windows_to_accessible ( _pointer );

   if ( SUCCEEDED(IAccessible_get_accChildCount ( accessible, &childCount )) )
   {
      ret = 0;
      for ( i = 0; i < childCount; i++ )
      {
         accessibleChild = windows_get_child ( accessible, i );
         if ( accessibleChild != NULL )
         {
            IAccessible_Release ( accessibleChild );
            ret++;
         }
      }
      
      /*
      childrenArray = malloc ( sizeof(VARIANT) * childCount );
      if ( SUCCEEDED(AccessibleChildren ( accessible, 0, childCount, childrenArray, &childrenObtained )) )
      {
         for ( i = 0; i < (size_t) childrenObtained; i++ )
         {
            if ( childrenArray[i].vt == VT_DISPATCH )
            {
               if ( SUCCEEDED(IDispatch_QueryInterface( childrenArray[i].pdispVal, &IID_IAccessible, &accessibleChild) ) )
               {
                  IAccessible_Release ( accessibleChild );
                  ret++;
               }
               IDispatch_Release ( childrenArray[i].pdispVal );
            }
            else
            {
            }
         }
      }
      free ( childrenArray );
      */
      return ret;
   }
   return 0;
}

SQByteArray * windows_child ( const SQByteArray * _pointer, long _child )
{
   IAccessible * accessible;
   IAccessible * accessibleChild;
   
   accessible = windows_to_accessible ( _pointer );
   
   accessibleChild = windows_get_child ( accessible, _child );
   
   if ( accessibleChild != NULL )
   {
      return windows_from_accessible ( accessibleChild );
   }
   else
   {
      return sq_byte_array_create_prealloc ( 0 );
   }
}
