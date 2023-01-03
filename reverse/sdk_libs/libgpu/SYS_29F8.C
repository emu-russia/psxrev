
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
