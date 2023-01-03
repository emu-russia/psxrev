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

void DumpTPage(u_short tpage)
{
    GPU_printf ("tpage: (%d,%d,%d,%d)\n",
                ((tpage)>>7)&0x003,((tpage)>>5)&0x003,       // tp, abr
                ((tpage)<<6)&0x7c0,                          // x
                (((tpage)<<4)&0x100)+(((tpage)>>2)&0x200) ); // y
}
