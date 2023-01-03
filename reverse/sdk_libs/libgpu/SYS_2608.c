/*
 * Reset GPU.
*/
int resetGPU (int mode)
{
    static u_long oldmask = SetIntrMask (0);

    _qin = _qout = 0;

    switch (mode & 7)
    {
        case 0:
        case 3:
        case 5:
            *gpu_dma_chcr = 0x0401;
            *dma_pcr |= 0x0800;
            *gp1 = 0x00000000;
            memset (_que, 0, sizeof _que);
            break;

        case 1:
            *gpu_dma_chcr = 0x0401;
            *dma_pcr |= 0x0800;
            *gp1 = 0x02000000;
            *gp1 = 0x01000000;
            break;
    }

    SetIntrMask (oldmask);

    if ( (mode & 7) == 0 ) return resetGPU2 (mode);
    else return 0;
}


int resetGPU2 (int mode)
{
    *gp1 = 0x10000007;      /* Get GPU version. */

    if((*gp0 & 0xffffff) != 2)
    {
        *gp0 = 0xe1001000 | (*gp1 & 0x3fff);
        *gp0;
        return 0;
    }

    if((mode & 8) == 0) return 1;
    else
    {
        *gp1 = 0x09000001;
        return 2;
    }
}
