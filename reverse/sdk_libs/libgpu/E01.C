// Load 256-color CLUT to VRAM.
#include <sys/types.h>
#include <kernel.h>
#include <libgte.h>
#include <libgpu.h>

u_short LoadClut(u_long *clut, int x, int y)
{
    RECT rect;

    setRECT (&rect, x, y, 256, 1);

    LoadImage (&rect, clut);
    return GetClut (x, y);
}
