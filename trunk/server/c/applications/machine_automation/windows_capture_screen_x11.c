#include <sequanto/automation.h>
#include <memory.h>

#include <stdio.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

/* http://stackoverflow.com/questions/69645/take-a-screenshot-via-a-python-script-linux
 * Compile hint: gcc -shared -O3 -lX11 -fPIC -Wl,-soname,prtscn -o prtscn.so prtscn.c
 */

const int BMP_PIXEL_DATA_OFFSET = 54;

SQByteArray * windows_capture_screen ( int _desktop )
{
    Display *display = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(display);
    Window moreRoot;
    int screen_x = 0;
    int screen_y = 0;
    unsigned int screen_width = 100;
    unsigned int screen_height = 100;
    unsigned int border;
    unsigned int depth;
    SQByteArray * data;
    XImage *image;
    unsigned long red_mask;
    unsigned long green_mask;
    unsigned long blue_mask;
    unsigned int x, y;
    unsigned long pixel;
    int ii;
    unsigned char blue;
    unsigned char green;
    unsigned char red;

    SQ_UNUSED_PARAMETER(_desktop);

    if(display == NULL)
    {
        sq_log("Could not open X11 display.");
        return sq_byte_array_create_prealloc( 0 );
    }

    XGetGeometry(display, root, &moreRoot, &screen_x, &screen_y, &screen_width, &screen_height, &border, &depth );

    data = sq_byte_array_create_prealloc( BMP_PIXEL_DATA_OFFSET + screen_width * screen_height * 3 );
    memset ( data->m_start, 0, BMP_PIXEL_DATA_OFFSET );

#define ADD_INT32(index,value) data->m_start[index + 0] = (((value) >> 0) & 0xFF); \
    data->m_start[index + 1] = (((value) >> 8) & 0xFF);                 \
    data->m_start[index + 2] = (((value) >> 16) & 0xFF);                \
    data->m_start[index + 3] = (((value) >> 24) & 0xFF);

    data->m_start[0] = 'B';
    data->m_start[1] = 'M';
    ADD_INT32(0x02, data->m_length);
    ADD_INT32(0x0A, BMP_PIXEL_DATA_OFFSET); /* Total header length */
    ADD_INT32(0x0E, 40); /* DIB header length */
    ADD_INT32(0x12, screen_width);
    ADD_INT32(0x16, screen_height);
    ADD_INT32(0x1A, 1); /* One color plane */
    ADD_INT32(0x1C, 24); /* 24 bits per pixel */
    ADD_INT32(0x1E, 0); /* Compression mode: no compression */
    ADD_INT32(0x22, screen_width * screen_height * 3); /* Bytes of pixel data */
    ADD_INT32(0x26, 2835); /* Horizontal resolution (72-dpi) */
    ADD_INT32(0x2A, 2835); /* Vertical resolution (72-dpi) */

    image = XGetImage(display, root, screen_x, screen_y,
                              screen_width, screen_height, AllPlanes, ZPixmap);
    red_mask = image->red_mask;
    green_mask = image->green_mask;
    blue_mask = image->blue_mask;

    for (x = 0; x < screen_width; x++)
    {
        for (y = 0; y < screen_height; y++)
        {
            pixel = XGetPixel(image,x,y);
            ii = (x + screen_width * (screen_height - y - 1)) * 3;

            blue = pixel & blue_mask;
            green = (pixel & green_mask) >> 8;
            red = (pixel & red_mask) >> 16;

            data->m_start[BMP_PIXEL_DATA_OFFSET + ii + 2] = blue;
            data->m_start[BMP_PIXEL_DATA_OFFSET + ii + 1] = green;
            data->m_start[BMP_PIXEL_DATA_OFFSET + ii + 0] = red;
        }
    }

    XDestroyImage(image);
    XCloseDisplay(display);

    return data;
}
