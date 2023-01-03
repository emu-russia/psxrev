// Make CLUT ID.
#include <sys/types.h>
#include <kernel.h>
#include <libgte.h>
#include <libgpu.h>

u_short GetClut(int x, int y)
{
    return getClut (x, y);
}
