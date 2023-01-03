/*
    Reverse engineering of SCE libgpu library.
    Released by ORG, 2007.

    Compilable with PsyQ 4.6
*/

#include <sys/types.h>
#include <r3000.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

/* Prototypes. */
static  u_long setDrawOffset (u_short ofsx, u_short ofsy);
static  void writeGP1(u_long val);
static  int sendPrimIO (u_long *p, int len);
static  int updateQueue (void);
static  int resetGPU (int mode);
static  int drawSync(int mode);
static  void clearTimeout (void);
static  int checkTimeout (void);
static  int resetGPUVersion (int mode);

typedef struct GEnv
{
    u_char  gpuType;
    u_char  que;
    u_char  debugLevel;
    u_char  reverse;

    u_short p4;
    u_short p6;

    u_long  p8;
    void    (*drawSyncCB)();

    DRAWENV draw;
    DISPENV disp;
} GEnv;

typedef struct JumpTable
{
    char    *rid;

    void    (*writeGP1)(u_long val);
    int     (*sendPrimIO)(u_long *p, int len);
    int     (*resetGPU)(int mode);
    int     (*drawSync)(int mode);
} JumpTable;

typedef struct GQue
{
    u_long p1;
    u_long p2;
    void (*cb)(u_long p1, u_long p2);
    char   buf[84];
} GQue;

static  JumpTable jtb = {
    "$Id: sys.c,v 1.140 1998/01/12 07:52:27 noda Exp yos $",

    writeGP1,
    sendPrimIO,
    resetGPU,
    drawSync
};

static  JumpTable *gpuCB = &jtb;

        int (*GPU_printf)(char *fmt, ...);

        GEnv genv;

// Registers.
static  volatile u_long *gp0 = (void *)0x1F801810;
static  volatile u_long *gp1 = (void *)0x1F801814;
static  volatile u_long *gpu_dma_madr = (void *)0x1F8010A0;
static  volatile u_long *gpu_dma_bcr = (void *)0x1F8010A4;
static  volatile u_long *gpu_dma_chcr = (void *)0x1F8010A8;
static  volatile u_long *gpu_ot_madr = (void *)0x1F8010E0;
static  volatile u_long *gpu_ot_bcr = (void *)0x1F8010E4;
static  volatile u_long *gpu_ot_chcr = (void *)0x1F8010E8;
static  volatile u_long *dma_pcr = (void *)0x1F8010F0;

        int _qin, _qout;

static  u_long intmask[3];  // This is probably declared as static in routines.
                            // Anyway, who cares.

static  int timeoutAmt, timeoutFlag;

static  GQue _que[64];

// ----------------------------------------------------------------------------

// Resets the graphic system according to mode:
// 0    Complete reset. The drawing environment and display environment are
//      initialized.
// 1    Cancels the current drawing and flushes the command buffer.
// 3    Initializes the drawing engine while preserving the current display and
//      draw environment (i.e. the screen is not cleared or the screen mode changed).
//
//      Return GPU model (?).
int ResetGraph(int mode)
{
    switch (mode & 7)
    {
        case 0:
        case 3:
            GPU_printf ("ResetGraph:jtb=%08x,env=%08x\n", &jtb, &genv);
        case 5:
            memset (&genv, 0, sizeof(GEnv));
            ResetCallback ();
            GPU_cw ((u_long)gpuCB & 0x00FFFFFF);        // ???
            genv.gpuType = resetGPU (mode);
            genv.que = 1;   // Enable graph queue
            genv.p4 = 0;//(u_short *)data_D0[genv.debugLevel];
            genv.p6 = 0;//(u_short *)data_DC[genv.debugLevel];
            memset (&genv.draw, -1, sizeof(DRAWENV));
            memset  (&genv.disp, -1, sizeof(DISPENV));
            return genv.gpuType;
            
        case 1:
        case 2:
        default:
            if (genv.debugLevel >= 2)
            {
                GPU_printf ("ResetGraph(%d)...\n", mode);
            }        
            return gpuCB->resetGPU (1);
    }
}

// Sets a debugging level for the graphics system. level can be one of the
// following:
// 0    No checks are performed. (Highest speed mode)
// 1    Checks coordinating registered and drawn primitives.
// 2    Registered and drawn primitives are dumped.
int SetGraphDebug(int level)
{
    u_char oldLevel = genv.debugLevel;
    genv.debugLevel = level;

    if (level)
    {
        GPU_printf ( "SetGraphDebug:level:%d,type:%d reverse:%d\n",
                     genv.debugLevel, genv.gpuType, genv.reverse );
    }

    return oldLevel;
}

// Enable/disable GPU queue. Return previous state.
int SetGraphQueue (int que)
{
    int old = genv.que;

    if (genv.debugLevel >= 2)
    {
        GPU_printf ("SetGrapQue(%d)...\n", que);
    } 

    if (genv.que != que)
    {
        gpuCB->resetGPU (1);
        DMACallback (2, 0);
        genv.que = que;
    }

    return old;
}

// Gets graphics system debug level.
int GetGraphDebug(void)
{
    return genv.debugLevel;
}

// Define a callback function to be called when the GPU is finished executing
// a primitive list.
u_long DrawSyncCallback(void (*func)())
{
    u_long oldCB;

    if (genv.debugLevel >= 2)
    {
        GPU_printf ("DrawSyncCallback(%08x)...\n", (u_long)func);
    }

    oldCB = (u_long)genv.drawSyncCB;
    genv.drawSyncCB = func;
    return oldCB;
}

// Set and cancel display mask.
void SetDispMask(int mask)
{
    if (genv.debugLevel >= 2)
    {
        GPU_printf ("SetDispMask(%d)...\n", mask);
    }

    if (mask == 0)
    {
        memset (&genv.disp, -1, sizeof(DISPENV));
    }

    gpuCB->writeGP1 (mask ? 0x03000001 : 0x03000000);
}

// Wait for all drawing to terminate. Mode:
// 0: Wait for termination of all non-blocking functions registered in the queue.
// 1: Return the number of positions in the current queue.
//
// Return number of positions in the execution queue.
int DrawSync(int mode)
{
    if (genv.debugLevel >= 2)
    {
        GPU_printf ("DrawSync(%d)...\n", mode);
    }

    return gpuCB->drawSync (mode);
}

// Draw a primitive. This routine blocks while waiting for all drawing commands
// in the queue to complete, then executes immediately.
void DrawPrim(void *p)
{
    P_TAG *tag = (P_TAG *)p;
    u_long *prim = (u_long *)p;

    gpuCB->drawSync (0);
    gpuCB->sendPrimIO (&prim[1], tag->len);
}

// Initializes a DR_OFFSET primitive. By using AddPrim() to insert a DR_OFFSET
// primitive into your primitive list, it is possible to change part of your
// drawing environment in the middle of drawing.
void SetDrawOffset(DR_OFFSET *p, u_short *ofs)
{
    setlen (p, 2);
    
    p->code[0] = setDrawOffset (ofs[0], ofs[1]);
    p->code[1] = 0;
}

// ----------------------------------------------------------------------------
// Helper calls.

/*
 * Creates drawing offset primitive.
*/
static u_long setDrawOffset (u_short ofsx, u_short ofsy)
{
    return (0xE5000000 | ((ofsy & 0x7FF) << 11) | (ofsx & 0x7FF));
}

/*
 * Write to GPU status register.
*/
static void writeGP1(u_long val)
{
    *gp1 = val;
}

/*
 * Send GPU primitive by CPU IO. GPU DMA is disabled during transfer.
*/
static int sendPrimIO(u_long *p, int len)
{
    *gp1 = 0x04000000;      // Disable DMA.

    while (len--)
    {
        *gp0 = *p++;
    }

    return 0;
}

/* 
 * Update GPU execution queue. Return number of positions in current queue.
*/
static int updateQueue (void)
{
    // Return 1 immediately, if DMA transfer is still busy.
    if (*gpu_dma_chcr & 0x01000000) return 1;

    // Ignore interrupts.
    intmask[1] = SetIntrMask (0); 

    // Execute queue entries, until GPU DMA is not busy.
    while ( !(*gpu_dma_chcr & 0x01000000) && (_qout != _qin) )
    {
        if ( ((_qout + 1) & 0x3F) == _qin )
        {
            if (genv.drawSyncCB == 0) DMACallback (2, 0);
        }

        // Wait until GPU become busy.
        if ( *gp1 & 0x04000000 )
        {
            while ( !(*gp1 & 0x04000000) ) ;
        }

        _que[_qout].cb (_que[_qout].p1, _que[_qout].p2);
        _qout = (_qout + 1) & 0x3F;
    }

    // Allow interrupts generation.
    SetIntrMask (intmask[1]);

    if (genv.drawSyncCB && genv.p8)
    {
        genv.p8 = 0;
        genv.drawSyncCB ();
    }

    return  (_qin - _qout) & 0x3F;
}

/*
 * Reset GPU. Return GPU model (?).
*/
static int resetGPU (int mode)
{
    intmask[2] = SetIntrMask (0);

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

    SetIntrMask (intmask[2]);

    if ( (mode & 7) == 0 ) return resetGPUVersion (mode);
    else return 0;
}

/*
 * Wait for all drawing to terminate.
 * Return number of positions in the execution queue.
*/
static int drawSync(int mode)
{
    int quelen;

    if (mode == 0)
    {
    // ------------------------------------------------------------------------
    // Wait for termination of all non-blocking functions registered in the queue
    // ------------------------------------------------------------------------

        clearTimeout ();

        // Execute queue entries.
        do
        {
            updateQueue ();
            if (checkTimeout ()) return -1;     // Timeout expired.
        } while (_qin != _qout);

        // Sync DMA and GPU.
        do
        {
            if (checkTimeout ()) return -1;     // Timeout expired.
        } while ( ((*gp1 & 0x04000000) == 0) && (*gpu_dma_chcr & 0x01000000) );                
    }
    else
    {
    // ------------------------------------------------------------------------
    // Return the number of positions in the current queue
    // ------------------------------------------------------------------------

        quelen = (_qin - _qout) & 0x3F;
        
        if ( quelen ) updateQueue ();

        if ( *gpu_dma_chcr & 0x01000000 || ((*gp1 & 0x04000000) == 0)) return (quelen ? quelen : 1);
        else return quelen;
    }
}

/*
 * GPU timeout is issued in cases where four seconds (240VSync) have elapsed,
 * but the GPU nonblocking function has not yet terminated.
*/
static void clearTimeout (void)
{
    timeoutAmt = VSync(-1) + 240;
    timeoutFlag = 0;
}

/*
 * Return 0 if no timeout expired, -1 otherwise.
*/
static int checkTimeout (void)
{
    if (VSync (-1) >= timeoutAmt)
    {
        // Timeout expired.
        if (timeoutFlag++ <= 0xF0000) return 0;
    }

    // Inform user about timeout and clear queue.
    printf ( "GPU timeout:que=%d,stat=%08x,chcr=%08x,madr=%08x\n",
             _qin - _qout, *gp1, *gpu_dma_chcr, *gpu_dma_madr ); 

    intmask[2] = SetIntrMask (0);
    _qin = _qout = 0;
    *gpu_dma_chcr = 0x0401;
    *dma_pcr |= 0x0800;
    *gp1 = 0x02000000;
    *gp1 = 0x01000000;
    SetIntrMask (intmask[2]);
    return -1;
}

/*
 * Perform version-specific GPU reset.
*/
static int resetGPUVersion (int mode)
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
