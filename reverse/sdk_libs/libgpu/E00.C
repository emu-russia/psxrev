// Load texture page to VRAM.
#include <sys/types.h>
#include <kernel.h>
#include <libgte.h>
#include <libgpu.h>

/*
 * The pages can be located in the frame buffer on X multiples of 64 and Y
 * multiples of 256. More than one texture page can be set up, but each
 * primitive can only contain texture from one page.
*/

u_short LoadTPage(u_long *pix, int tp, int abr, int x, int y, int w, int h)
{
    RECT rect;

    rect.x = x;
    rect.y = y;
    rect.h = h;

    // Calculate correct texture page width.
    switch (tp)
    {
        case 0:
            if (w < 0) w += 3;  // CLUT4, multiple of 64
            rect.w = w / 4;
            break;
        case 1:
            rect.w = w / 2;     // CLUT8, multiple of 128
            break;
        case 2:
            rect.w = w;         // Direct CLUT, multiple of 256
            break;
    }

    // Note: this procedure doesn't check texture page alignment, you must take care of it.

    LoadImage (&rect, pix);
    return GetTPage (tp, abr, x, y);
}
