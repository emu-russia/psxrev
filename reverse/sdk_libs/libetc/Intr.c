// PSYQ/LIB/LIBETC.LIB, INTR.OBJ

static char Copyright[] = "Library Programs (c) 1993-1997 Sony Computer Entertainment Inc., All Rights Reserved.";

// PreGit era...

static char IntrSid[] = "$Id: intr.c,v 1.75 1997/02/07 09:00:36 makoto Exp $"; 

// Used by CheckCallback. Bogus.

static uint16 InsideCallback; 


// Interrupt Control Block (internals)

typedef struct _PsxInterruptControl
{
    char * Sid;

    void (*DmaCallback) ();
    void (*InterruptCallback) ();
    void (*Reset) ();
    void (*Stop) ();
    void (*VsyncCallback) ();
    void (*Restart) ();
    uint32 * word_958;

} PsxInterruptControl;


static PsxInterruptControl IntrCB = {

    IntrSid,
    NULL,       // Dma
    interruptCallback,
    resetCallback,
    stopCallback,
    NULL,       // Vsync
    restartCallback,
    &word_958,

};
static PsxInterruptControl * pIntrCB = &IntrCB;

//
// Thunks
//

// Initialize all callbacks.

// Initializes all system callbacks. Sets all callback functions to 0 (unregistered), and after securing the interrupt
// context stack, sets up the environment for accepting interrupts.

// ResetCallback() must be called after program boot, before any other processing is performed.

int ResetCallback(void)
{


}

// Issue Generic interrupt handler

InterruptCallback ()
{

}

// Issue DMA interrupt handler

DMACallback ()
{


}

// Issue Vsync handler

int VSyncCallback(void (*f)())
{

}

int VSyncCallbacks(int ch, void (*f)())
{

}

// Stop all callbacks.

// Stops all system callbacks.
// Before terminating programs, StopCallback() can be called to disable all interrupts.

int StopCallback(void)
{

}

// Restart a halted callback.

// Restores the halted call-back to the status immediately prior to when it was halted.
// Differs from ResetCallback() in that the call-back functions and call-back stack are not initialized.

int RestartCallback(void)
{

}

// Bogus.

// Determine whether the program is executing a callback.

int CheckCallback ()
{
    return InsideCallback;
}

GetIntrMask ()
{

}

SetIntrMask ()
{

}

//
// Internal implementation
//

resetCallback ()
{

}

sub_46C ()
{

}

interruptCallback ()
{

}

stopCallback ()
{

}

restartCallback ()
{

}

ZeroMem ()
{

}
