// Get TIM info to TIM_IMAGE structure.
#include <sys/types.h>
#include <kernel.h>
#include <libgte.h>
#include <libgpu.h>

static u_long *tim_addr;

int OpenTIM (u_long *addr)
{
    // Fairy simple procedure, just save memory address for next ReadTIM call.
    tim_addr = addr;
    return 0;
}

TIM_IMAGE *ReadTIM (TIM_IMAGE *timimg)
{
    int size = ProduceTIM (tim_addr, timimg);

    if (size == -1)
    {
        return 0;
    }
    else
    {
        tim_addr += size;   // Adjust start address to the next TIM (in terms of words)
        return timimg;
    }
}

static int ProduceTIM (u_long *timaddr, TIM_IMAGE *timimg)
{
    int clutsize;

    // Check ID.
    if (*timaddr++ != 16)
    {
        return -1;
    }

    if (GetGraphDebug() == 2)
    {
        printf ("id  =%08x\n", 16);
    }

    // Read mode.
    timimg->mode = *timaddr++;
    if (GetGraphDebug() == 2)
    {
        printf ("mode=%08x\n", timimg->mode);
    }

    // Print start address of actual TIM data.
    if (GetGraphDebug() == 2)
    {
        printf ("timaddr=%08x\n", timaddr);
    }

    // CLUT. Format of CLUT entry is as follow:
    //      +0 : size in bytes of CLUT entry
    //      +4 : CLUT RECT (8 bytes)
    //      +12: CLUT data
    if (timimg->mode & 8)
    {
        timimg->crect = &timaddr[1];
        timimg->caddr = &timaddr[3];
        clutsize = *timaddr / 4;
        timaddr += clutsize;
    }
    else
    {
        clutsize = 0;
        timimg->crect = NULL;
        timimg->caddr = NULL;
    }

    // IMAGE
    // Format of image entry same as CLUT entry.
    timimg->prect = &timaddr[1];
    timimg->paddr = &timaddr[3];
    return (*timaddr / 4) + clutsize + 2;
}
