#define COBJMACROS
#include <ObjBase.h>
#include <OAIdl.h>
#include <OleAcc.h>

#include <sequanto/automation.h>
#include "config.h"

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

   ret = (char*) malloc ( length );

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
   char * buffer = (char*) malloc(100);

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
   //VARIANT roleVar;
   long x = -1, y = -1, width = 0, height = 0;

   accessible = windows_to_accessible ( _pointer );

   IAccessible_accLocation ( accessible, &x, &y, &width, &height, s_childIdSelf );

   return x;
}

int windows_y ( const SQByteArray * _pointer )
{
   IAccessible * accessible;
   //VARIANT roleVar;
   long x = -1, y = -1, width = 0, height = 0;

   accessible = windows_to_accessible ( _pointer );

   IAccessible_accLocation ( accessible, &x, &y, &width, &height, s_childIdSelf );

   return y;
}

int windows_width ( const SQByteArray * _pointer )
{
   IAccessible * accessible;
   //VARIANT roleVar;
   long x = -1, y = -1, width = 0, height = 0;

   accessible = windows_to_accessible ( _pointer );

   IAccessible_accLocation ( accessible, &x, &y, &width, &height, s_childIdSelf );

   return width;
}

int windows_height ( const SQByteArray * _pointer )
{
   IAccessible * accessible;
   //VARIANT roleVar;
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
      return SQ_STRDUP_FUNCTION("");
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

int windows_actions ( SQByteArray * _pointer )
{
   return 1;
}

char * windows_action_name ( SQByteArray * _pointer, int _actionIndex )
{
    return SQ_STRDUP_FUNCTION("invoke");
}

void windows_action_do ( SQByteArray * _pointer, int _actionIndex )
{
   IAccessible * accessible;

   accessible = windows_to_accessible ( _pointer );

   IAccessible_accDoDefaultAction( accessible, s_childIdSelf );
}

HBITMAP CopyScreenToBitmap(HDC displayDC, LPRECT lpRect)
{
   HDC         hMemDC;         // screen DC and memory DC
   int         nX, nY, nX2, nY2;       // coordinates of rectangle to grab
   int         nWidth, nHeight;        // DIB width and height
   int         xScrn, yScrn;           // screen resolution

   HGDIOBJ     hOldBitmap , hBitmap;

   // check for an empty rectangle
   if (IsRectEmpty(lpRect))
      return NULL;
   // create a DC for the screen and create
   // a memory DC compatible to screen DC

   hMemDC = CreateCompatibleDC(displayDC);      // get points of rectangle to grab

   nX = lpRect->left;
   nY = lpRect->top;
   nX2 = lpRect->right;
   nY2 = lpRect->bottom;      // get screen resolution

   xScrn = GetDeviceCaps(displayDC, HORZRES);
   yScrn = GetDeviceCaps(displayDC, VERTRES);

   //make sure bitmap rectangle is visible

   if (nX < 0)
      nX = 0;

   if (nY < 0)
      nY = 0;

   if (nX2 > xScrn)
      nX2 = xScrn;

   if (nY2 > yScrn)
      nY2 = yScrn;

   nWidth = nX2 - nX;
   nHeight = nY2 - nY;

   // create a bitmap compatible with the screen DC

   hBitmap = CreateCompatibleBitmap(displayDC, nWidth, nHeight);

   // select new bitmap into memory DC

   hOldBitmap =   SelectObject (hMemDC, hBitmap);

   // bitblt screen DC to memory DC

   BitBlt(hMemDC, 0, 0, nWidth, nHeight, displayDC, nX, nY, SRCCOPY);

   // select old bitmap back into memory DC and get handle to
   // bitmap of the screen

   hBitmap = SelectObject(hMemDC, hOldBitmap);

   DeleteDC(hMemDC);

   return (HBITMAP)hBitmap;
}


SQByteArray * SaveToByteArray(HDC displayDC, HBITMAP hBitmap)
{
   int iBits;
   WORD wBitCount;
   DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;
   BITMAP Bitmap;
   BITMAPFILEHEADER bmfHdr;
   BITMAPINFOHEADER bi;
   LPBITMAPINFOHEADER lpbi;
   HANDLE hDib, hPal,hOldPal=NULL;
   SQByteArray * ret;

   iBits = GetDeviceCaps(displayDC, BITSPIXEL) * GetDeviceCaps(displayDC, PLANES);
   DeleteDC(displayDC);

   if (iBits <= 1)
      wBitCount = 1;
   else if (iBits <= 4)
      wBitCount = 4;
   else if (iBits <= 8)
      wBitCount = 8;
   else
      wBitCount = 24;
   GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
   bi.biSize = sizeof(BITMAPINFOHEADER);
   bi.biWidth = Bitmap.bmWidth;
   bi.biHeight = Bitmap.bmHeight;
   bi.biPlanes = 1;
   bi.biBitCount = wBitCount;
   bi.biCompression = BI_RGB;
   bi.biSizeImage = 0;
   bi.biXPelsPerMeter = 0;
   bi.biYPelsPerMeter = 0;
   bi.biClrImportant = 0;
   bi.biClrUsed = 0;
   dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

   hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
   lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
   *lpbi = bi;

   hPal = GetStockObject(DEFAULT_PALETTE);
   if (hPal)
   {
      displayDC = GetDC(NULL);
      hOldPal = SelectPalette(displayDC, (HPALETTE)hPal, FALSE);
      RealizePalette(displayDC);
   }


   GetDIBits(displayDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
      +dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);

   if (hOldPal)
   {
      SelectPalette(displayDC, (HPALETTE)hOldPal, TRUE);
      RealizePalette(displayDC);
      ReleaseDC(NULL, displayDC);
   }

   bmfHdr.bfType = 0x4D42; // "BM"
   dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
   bmfHdr.bfSize = dwDIBSize;
   bmfHdr.bfReserved1 = 0;
   bmfHdr.bfReserved2 = 0;
   bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

   ret = sq_byte_array_create_prealloc ( sizeof(BITMAPFILEHEADER) + dwDIBSize );
   memcpy ( ret->m_start, &bmfHdr, sizeof(BITMAPFILEHEADER) );
   memcpy ( ret->m_start + sizeof(BITMAPFILEHEADER), lpbi, dwDIBSize );

   GlobalUnlock(hDib);
   GlobalFree(hDib);
   return ret;
}

SQByteArray * windows_capture_screen ( int _desktopIndex )
{
   HWND desktopWindow;
   RECT windowRect;
   HBITMAP bitmap;
   HDC displayDC;
   SQByteArray * ret;

   desktopWindow = GetDesktopWindow();
   GetWindowRect(desktopWindow, &windowRect);
   displayDC = CreateDC("DISPLAY", NULL, NULL, NULL);
   bitmap = CopyScreenToBitmap ( displayDC, &windowRect );
   ret = SaveToByteArray(displayDC, bitmap);
   DeleteObject(bitmap);
   return ret;
}
