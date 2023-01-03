#include <sys/types.h>
#include <kernel.h>
#include <r3000.h>
#include <libgte.h>
#include <libgpu.h>

static char *primstr[24] = {
 "F3", "FT3", "F4", "FT4", "G3", "GT3", "G4",  "GT4", 
 "LF", "LF",  "LF", "LF",  "GF", "GF",  "GF",  "GF",
 "T",  "S",   "T1", "S1",  "T8", "S8",  "T16", "S16" 
};

static u_char primlen[24] = {
 4, 7, 5, 9, 6, 9, 8, 12,
 3, 0, 0, 0, 4, 0, 0, 0,
 3, 4, 2, 3, 2, 3, 2, 3
};

// Draw ordering table by CPU I/O (slow).
void DrawOTagIO(u_long *p)
{
    P_TAG *tag = (P_TAG *)p;

    while (tag->addr != 0x00FFFFFF)
    {
        if (tag->len) DrawPrim (tag);
        tag = (P_TAG *)PHYS_TO_K0(tag->addr);   // Link to next primitive
    }

    // Last one.
    if (tag->len) DrawPrim (tag);
}

// Dump primitives from ordering table.
void DumpOTag(u_long *ot)
{
    P_TAG *tag = (P_TAG *)ot, *next;
    int prim = 1;
    char *str;

    GPU_printf ("OTag :prim\n%5d:", 0);

    while (tag->addr != 0x00FFFFFF)
    {
        if (tag->len == 0)                      // Skip empty primitives
        {
            next = (P_TAG *)PHYS_TO_K0(tag->addr);
            if ( next->len )
            {
                GPU_printf ("\n%5d:", prim);
            }
            prim++;
        }
        else
        {
            CheckPrim ("DumpOTage", (u_long *)tag);

            if (tag->code == 2) str = "BF";
            else if (0x20 <= tag->code && tag->code < 0x80)
            {
                str = primstr[(tag->code - 0x20) / 4];
            }
            else str = "?";

            GPU_printf ("-%s", str);
        }

        tag = (P_TAG *)PHYS_TO_K0(tag->addr);   // Link to next primitive        
    }

    // Last one.
    CheckPrim ("DumpOTage", (u_long *)tag);
    if (tag->code == 2) str = "BF";
    else if (0x20 <= tag->code && tag->code < 0x80)
    {
        str = primstr[(tag->code - 0x20) / 4];
    }
    else str = "?";
    GPU_printf ("-%s\n", str);
}

// 0 valid primitive; -1 invalid.
int CheckPrim(char *s, u_long *p)
{
    P_TAG *tag = (P_TAG *)p;
    char *str;
    int len;

    if (tag->code == 2)
    {
        if (tag->len == 3) return 0;
        else str = "BF";
    }
    else if ((0x20 <= tag->code && tag->code < 0x80) )
    {
        len = primlen[(tag->code - 0x20) / 4];
        if (len == tag->len) return 0;
        else str = primstr[(tag->code - 0x20) / 4];
    }
    else str = "?";

    GPU_printf ("%s: bad prim:addr=%08X,type=%s,len=%d\n", s, p, str, tag->len);
    return -1;
}
