// Make texture page ID.
#include <sys/types.h>
#include <kernel.h>
#include <libgte.h>
#include <libgpu.h>

/*
 * |F E|D C|B  A 9|8 7|6 5|4 |3 2 1 0|
 * |0 0|mir|y1 0 0|tp |abr|y0|tx     |
*/

// Y1 is undocumented in Doomer's gpu.txt
// Seems Y1 was used in some boards with 2MB of VRAM (ZN1, Namco System 11)
// Mir - use texture page mirroring (seen in Pete GPU plugin)

u_short GetTPage(int tp, int abr, int x, int y)
{
    u_short id;

    id  = ((tp & 3) << 7);
    id |= ((abr & 3) << 5);
    id |= ((y & 0x100) >> 4);       // Y / 256
    id |= ((x & 0x3FF) >> 6);       // X / 64
    id |= ((y & 0x200) << 2);       // Set to 1, when Y >= 512

    return id;
}
