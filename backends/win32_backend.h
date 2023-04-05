#ifndef __SR_WIN32_BACKEND_IMPL
#define __SR_WIN32_BACKEND_IMPL

#include <windows.h>
#include <stdbool.h>
#include <winuser.h>
#include "../src/software_renderer.h"


typedef struct {
    HWND*       handle;
    HDC         hdc;
    bool        initialized;
} SrWindowContext;


static SrWindowContext sr_context;


static sr_u32 sr_pack_vec4_to_u32(sr_vec4 in);
static sr_vec4 sr_rgba_to_argb(sr_vec4 in);




/**
* sr_init - initialize the win32 api context
*
* @hwnd: a pointer to the Win32 window handle passed by the user
* Return: 1 if inialized correctly otherwise it returns 0
*/
inline bool sr_context_init(HWND* hwnd) {
    if (sr_context.initialized) {
        return false;
    }

    if (!hwnd) {
        return false;
    }

    sr_context.initialized = true;
    sr_context.handle      = hwnd;
    sr_context.hdc         = GetDC(*hwnd);

    return true;
}



/**
* sr_present - it present(draw) the given framebuffer to the 
* screen
* 
* @framebuffer: a framebuffer to be drawn into the screen
* Return: none
*/
inline bool sr_present(SrFramebuffer* fb) {
    sr_u32 width = fb->spec.width;
    sr_u32 height = fb->spec.height;

    if (!sr_context.initialized)
        return false;


    BITMAPINFO bmi {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    sr_u32 size = width * height;

    sr_u32* buffer = (sr_u32*)malloc(size * 4);

    for (sr_u32 i = 0; i < size; i++) {
        buffer[i] = sr_pack_vec4_to_u32(sr_rgba_to_argb(*(sr_vec4*)&fb->color_buffer[i]));
    }

    StretchDIBits(sr_context.hdc, 0, 0, width, height, 0, 0, width, height, buffer, &bmi, DIB_RGB_COLORS, SRCCOPY);

    free(buffer);

    return true;
}





static inline sr_u32 sr_pack_vec4_to_u32(sr_vec4 in) {

    sr_u32 r = (sr_u32)(in.x * 255.0f);
    sr_u32 g = (sr_u32)(in.y * 255.0f);
    sr_u32 b = (sr_u32)(in.z * 255.0f);
    sr_u32 a = (sr_u32)(in.w * 255.0f);

    return r << 24 | g << 16 | b << 8 | a;
}


static inline sr_vec4 sr_rgba_to_argb(sr_vec4 in) {
    sr_vec4 out;

    out.x = in.w;
    out.y = in.x;
    out.z = in.y;
    out.w = in.z;

    return out;
}



#endif // __SR_WIN32_BACKEND_IMPL