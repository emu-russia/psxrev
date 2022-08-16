# BIOS (BOOT ROM)

The 512 KB ROM contains the BIOS boot loader, a copy of the PlayStation OS kernel, and a "shell" which opens if no game disc is inserted in the console and contains the memory card manager and CD player.

A typical ROM BIOS looks like this:

![ROM_BIOS_package](/wiki/imgstore/ROM_BIOS_package.jpg)

- The PU-7 and old PU-8 BIOS has a 40 pin chip.
- From the new versions of PU-8 (and on) the chip has 32 pins (including PSOne).

Inside most likely ROM with ion implantation by mask.

## BIOS Versions

This is a complicated question, because BIOS versions firstly differ from region to region, and secondly they differ between motherboard models.
And even within the same motherboard model there can be different versions of BIOS, depending on the revision of the motherboard of the same model.

- http://emu-russia.net/ru/files/bios/psx/ 
- http://www.emu-land.net/consoles/psx/bios

Almost all emulators consider SCPH1001.BIN to be the reference BIOS version. This BIOS has been thoroughly disassembled and is considered "stable" to work in emulators.

## BIOS Image Structure

The BIOS image consists of three parts. Take SCPH-1001 as an example:

- 0x0: Boot. The part of the BIOS that boots the kernel and contains most of the system calls to table A0.
- 0x10000: Kernel. The kernel image which is copied to 0x500. Also contains system calls to tables B0 and C0.
- 0x18000: Shell. Copied to 0x80030000.

At the very end of the Shell is some structure with strings about the BIOS version. Where it is used has not been found yet.

## Boot

Initial boot program (RESET)

Execution starts at address 0xBFC00000

- Initializes undocumented hardware CPU registers (timing, etc.)
- Clears memory and CPU registers
- Executes PIO initialization program (init) if a PIO device is present in PSX
- Switches to kernel loading (Main procedure)

Some reversing of SCPH-1001 BIOS:

```c
//
// SCPH1001 Reset
// Written likely on assembler
//

Reset ()        // 0xBFC00000
{

    dword.0x1F801010 = 0x13243F;

    dword.0x1F801060 = 0xB88;       // ram_size?

    //
    // 20 nops (pipeline reset?)
    //

    nop x 20;

    goto Reset2;
}    

Reset2 ()       // 0xBFC00150
{
    //
    // More registers
    //

    dword.0x1F801020 = 0x31125;

    dword.0x1F801000 = 0x1F000000;

    dword.0x1F801004 = 0x1F802000;

    dword.0x1F801008 = 0x13243F;

    dword.0x1F801014 = 0x200931E1;

    dword.0x1F801018 = 0x20843;

    dword.0x1F80100C = 0x3022;

    dword.0x1F80101C = 0x70777;

    //
    // Clear CPU registers
    //

    gpr[1...31] = 0;

    //
    // B/U Control (Lock ICache?)
    //

    dword.0xFFFE0130 = 0x804;       // Enable I-Cache + Tag Test Mode

    //
    // COP0
    //

    COP0.SR = 0x10000;      // Isolate cache from bus

    //
    // Clear instruction cache Tag memory (Cache line = 16 bytes)
    //

    for ( int Addr=0; Addr<0x1000; Addr += 0x80 )
    {
        dword.[Addr] = 0;
        dword.[Addr + 0x10] = 0;
        dword.[Addr + 0x20] = 0;
        dword.[Addr + 0x30] = 0;
        dword.[Addr + 0x40] = 0;
        dword.[Addr + 0x50] = 0;
        dword.[Addr + 0x60] = 0;
        dword.[Addr + 0x70] = 0;
    }

    //
    // COP0
    //

    COP0.SR = 0;

    //
    // BIU/Cache configuration
    //

    dword.0xFFFE0130 = 0x800;       // Enable I-Cache

    //
    // COP0
    //

    COP0.SR = 0x10000;          // Isolate cache from bus

    //
    // Clear instruction cache lines
    //

    for ( int Addr=0; Addr<0x1000; Addr += 0x80 )
    {
        dword.[Addr] = 0;
        dword.[Addr + 0x4] = 0;
        dword.[Addr + 0x8] = 0;
        dword.[Addr + 0xC] = 0;
        dword.[Addr + 0x10] = 0;
        dword.[Addr + 0x14] = 0;
        dword.[Addr + 0x18] = 0;
        dword.[Addr + 0x1C] = 0;
        dword.[Addr + 0x20] = 0;
        dword.[Addr + 0x24] = 0;
        dword.[Addr + 0x28] = 0;
        dword.[Addr + 0x2C] = 0;
        dword.[Addr + 0x30] = 0;
        dword.[Addr + 0x34] = 0;
        dword.[Addr + 0x38] = 0;
        dword.[Addr + 0x3C] = 0;
        dword.[Addr + 0x40] = 0;
        dword.[Addr + 0x44] = 0;
        dword.[Addr + 0x48] = 0;
        dword.[Addr + 0x4C] = 0;
        dword.[Addr + 0x50] = 0;
        dword.[Addr + 0x54] = 0;
        dword.[Addr + 0x58] = 0;
        dword.[Addr + 0x5C] = 0;
        dword.[Addr + 0x60] = 0;
        dword.[Addr + 0x64] = 0;
        dword.[Addr + 0x68] = 0;
        dword.[Addr + 0x6C] = 0;
        dword.[Addr + 0x70] = 0;
        dword.[Addr + 0x74] = 0;
        dword.[Addr + 0x78] = 0;
        dword.[Addr + 0x7C] = 0;                                
    }

    //
    // COP0
    //

    COP0.SR = 0;

    //
    // Read memory 8 times
    //

    t1 = dword.0xA0000000;
    t1 = dword.0xA0000000;
    t1 = dword.0xA0000000;
    t1 = dword.0xA0000000;
    t1 = dword.0xA0000000;
    t1 = dword.0xA0000000;
    t1 = dword.0xA0000000;
    t1 = dword.0xA0000000;

    //
    // BIU/Cache default configuration
    //

    dword.0xFFFE0130 = 0x1E988;         // D-Cache as Scratchpad
                                        // Enable D-Cache
                                        // Enable I-Cache 
                                        // Enable Bus Grant
                                        // No wait state
                                        // Enable Read Priority
                                        // Enable Load Scheduling

    //
    // Reset COP0 regs
    //

    COP0.Reg7 = 0;
    COP0.EntryLo1 = 0;
    COP0.PageMask = 0;
    COP0.Wired = 0;
    COP0.Count = 0;
    COP0.Compare = 0;
    COP0.SR = 0;
    COP0.Cause = 0;

    //
    // Clear 0xA0009000
    //

    memset ( 0xA0009000, 0, 0x3160 );

    //
    // Set initial context
    //

    CPU.SP = 0x801FFF00;
    CPU.GP = 0xA0010FF0;
    CPU.FP = CPU.SP;

    //
    // ram_size ?
    //

    dword.0x1F801060 = 0xB88;

    dword.0x60 = 2;
    dword.0x64 = 0;
    dword.0x68 = 0xff;

    //
    // Mute SPU
    //

    word.[0x1F801C00 + 0x180] = 0;      // Mainvolume Left
    word.[0x1F801C00 + 0x182] = 0;      // Mainvolume Right
    word.[0x1F801C00 + 0x184] = 0;      // Reverb depth left
    word.[0x1F801C00 + 0x186] = 0;      // Reverb depth right

    goto Reset3;
}

//
// Following boot code written on C for sure (has C prolog/epilog in code)
//

Reset3 ()   // 0xBFC06EC4
{
    int Present;

    TraceStep (0xF);

    word.[0x1F801C00 + 0x186] = 0;
    word.[0x1F801C00 + 0x184] = 0;
    word.[0x1F801C00 + 0x182] = 0;
    word.[0x1F801C00 + 0x180] = 0;

    Present = CheckPIO ();

    if ( Present )
        ResetPIO ();

    TraceStep (0xE);

    dword.0xA000B9B0 = 0;

    StartKernel ();
}

TraceStep (a0)      // 0xBFC01A60
{
    Bogus1 ();

    byte.0x1F802041 = a0 & 0xFF;
}

Bogus1 ()       // 0xBFC03990
{
    dword.0xA000B068 = 0;
    dword.0xA000B068 = 0;
    dword.0xA000B068 = 0;
    dword.0xA000B068 = 0;
}

//
// PIO Support
//

char Licensed[] = "Licensed by Sony Computer Entertainment Inc.";       // 0xBFC0E288

int CheckPIO (void)         // 0xBFC0703C
{
    char * Source = Licensed;
    char * Dest = 0x1F000084;       // PIO Header

    while ( *Source )
    {
        if ( *Source != *Dest ) break;

        Source++;
        Dest++;
    }

    if ( *Source ) return 0;
    else return 1;
}

void ResetPIO (void)        // 0xBFC0711C
{
    //
    // Run init code in PIO Space
    //

    dword.0x1F000080 ();
}

void StartKernel ()      // 0xBFC06784
{
    char Config[0x50];
    char Exec[0x50];

    strcpy ( Config, "cdrom:" );
    strcat ( Config, "SYSTEM.CNF;1" );

    strcpy ( Exec, "cdrom:" );
    strcat ( Exec, "PSX.EXE;1" );

    Main ( Config, Exec );
}
```

## Bootrom Main

The Main procedure works as follows:

- A resident kernel image is copied into memory and its initialization procedure is run
- Kernel Traps are installed (handlers of exceptions, interrupts and system calls)
- Installing device drivers (TTY, CDROM and MemCard).
- Initialized Kernel Executive: Event handlers, threads, events and system counters
- Unpack and start SHELL, which decides whether to start the menus or to exit back to the kernel to load the disk
- If SHELL exits, Main continues to load the disk
- The main procedure of the PIO device (PIO Shell) is started, which either intercepts control or returns control back to Main.
- The SYSTEM.CNF configuration is read and the kernel reinitializes the system tables according to the settings
- Executable file is loaded.
- Executable file is run for execution

```c
//
// Bootrom Main
//

// Sometimes we need to re-mute SPU (why?)

#define MUTE_SPU() \
	word.[0x1F801C00 + 0x186] = 0; 	\
	word.[0x1F801C00 + 0x184] = 0;	\
	word.[0x1F801C00 + 0x182] = 0;	\
	word.[0x1F801C00 + 0x180] = 0;

typedef struct _SYSTEM_CONFIG
{
    SIZE_T      Tcb;            // Max number of threads
    SIZE_T      Event;          // Max number of events
    PVOID       Stack;          // Initial stack pointer
} SYSTEM_CONFIG, *PSYSTEM_CONFIG;

SYSTEM_CONFIG DefaultConfig = {         // 0xBFC0E14C
    4, 
    16,
    0x801FFF00
};

void Main (char *Config, char *Exec )       // 0xBFC067E8
{
    int File;
    long Bytes;

	//
	// Disable all interupts, External (level 3) interrupts are disabled
	//

	TraceStep (1);

	SetSr ( GetSr () & 0xFFFFFBFE );

	MUTE_SPU ();

	//
	// Copy kernel image from Bootrom and run its initialization code.
	//

	TraceStep (2);

	LoadInitKernel ();

	//
	// Initialize kernel traps
	//

	TraceStep (3);

	CopyA0Table ();       // Copy BIOS A0 Table at 0x200

	InitSyscall ();        // Copy 0xA0, 0xB0 and 0xC0 syscall stubs from kernel image

	PatchA0Table ();

	InstallExceptionHandlers ();

	ResetEntryInt ();

	//
	// Init device drivers
	//

	TraceStep (4);

	MUTE_SPU ();

	dword.0x1F801074 = 0;      // int_mask
	dword.0x1F801070 = 0;      // int_reg

	InstallDevices ( dword_B9B0 );

	//
	// Shout out first printf
	//

	TraceStep (5);

	printf ( "\n" 
			 "PS-X Realtime Kernel Ver.2.5\n"
			 "Copyright 1993,1994 (C) Sony Computer Entertainment Inc. \n" );

	//
	// Init Kernel executive
	//

	TraceStep (6);

	MUTE_SPU ();

	memcpy ( 0xA000B940, &DefaultConfig, sizeof(SYSTEM_CONFIG) );

	printf ( "KERNEL SETUP!\n" );

	SysInitMemory ( 0xA000E000, 0x2000 );      // Kernel heap

	InitEventHandlers (4);

	InitException (0)

	InitDefInt (3);

	InitEvents ( dword_B944 );

	InitThreads ( 1, dword_B940 );

	InitRCnt (1);

	MUTE_SPU ();

	if ( setjmp ( 0xA000B980 ) )
		SystemHalt (901);

	//
	// Run Shell
	//

	TraceStep (7);

	LoadRunShell ();

	//
	// 8
	//

	TraceStep (8);

	dword.0x1F801074 = 0;      // int_mask
	dword.0x1F801070 = 0;      // int_reg

	sub_BFC073A0 ();

	if ( setjmp ( 0xA000B980 ) )
		SystemHalt (921);

    //
    // PIO Shell
    //

	if ( CheckPIO2 () == 1 )
		BootPIO ();

	printf ( "\n" 
			 "BOOTSTRAP LOADER Type C Ver 2.1   03-JUL-1994\n"
			 "Copyright 1993,1994 (C) Sony Computer Entertainment Inc.\n" );

	if ( setjmp ( 0xA000B980 ) )
		SystemHalt (902);

	//
	// Load executable from external media.
	//

	TraceStep (9);

	if ( setjmp ( 0xA000B980) )
		SystemHalt (903);

    //
    // Try to open system config file.
    // If config file is missing fall back to plain EXE loading
    //

	File = open ( Config, O_RDONLY );

	if ( File >= 0 )
	{
        //
        // Read and parse config file parameters
        //

		printf ( "setup file    : %s\n", Config );

		if ( setjmp ( 0xA000B980 ) )
			SystemHalt (911);

		Bytes = read ( File, 0xA000B070, 0x800 );

		if ( Bytes )
		{
			0xA000B070[Bytes] = 0;   // Terminate string

			close ( File );

			if ( setjmp ( 0xA000B980 ) )
				SystemHalt ( 912 );

			ParseConfig ( 0xA000B070, 0xA000B940, 0xA000B8B0 );
		}
		else
		{
            //
            // Set default config, fall back to plain EXE loading
            //

			memcpy ( 0xA000B940, &DefaultConfig, sizeof(SYSTEM_CONFIG) );
			strcpy ( 0xA000B8B0, Exec );
		}
	}
	else
	{
        //
        // Use plain executable
        //

		if ( setjmp ( 0xA000B980 ) )
			SystemHalt (913);

		byte.0x180 = 0;   // Version number
		memcpy ( 0xA000B940, &DefaultConfig, sizeof(SYSTEM_CONFIG) );
		strcpy ( 0xA000B8B0, Exec );
	}

	//
	// Load executable. Reinit kernel according to new Config settings.
	//

	if ( setjmp ( 0xA000B980 ) )
		SystemHalt (904);

	ReinitKernel ();

	printf ( "boot file     : %s\n", 0xA000B8B0 );

	if ( setjmp ( 0xA000B980 ) )
		SystemHalt (905);

	ClearStack ();

	if ( Load (0xA000B8B0, 0xA000B870 ) == 0 )
		SystemHalt (906);

	printf ( "EXEC:PC0(%08x)  T_ADDR(%08x)  T_SIZE(%08x)\n", dword_B870, dword_B878, dword_B87C );

	printf ( "boot address  : %08x %08x\n"
			 "Execute !\n\n", dword_B870, dword_B948 );

	dword_B890 = dword_B948;
	dword_B894 = 0;

	printf ( "                S_ADDR(%08x)  S_SIZE(%08)\n", dword_B948, 0 );

	EnterCriticalSection ();

	if ( setjmp ( 0xA000B980 ) )
		SystemHalt (907);

	Exec (0xA000B870, 1, 0 );

	printf ( "End of Main\n" );

	SystemHalt (908);
}

//
// Support
//

// Written in asm.
int setjmp ( jmp_buf * Buffer )       // 0xBFC02240
{
    Buffer[JB_PC] = ra;
    Buffer[JB_GP] = gp;
    Buffer[JB_SP] = sp;
    Buffer[JB_FP] = fp;
    Buffer[JB_S0] = s0;
    Buffer[JB_S1] = s1;
    Buffer[JB_S2] = s2;
    Buffer[JB_S3] = s3;
    Buffer[JB_S4] = s4;
    Buffer[JB_S5] = s5;
    Buffer[JB_S6] = s6;
    Buffer[JB_S7] = s7;
    return 0;
}

void SystemHalt (int Code)      // 0xBFC06FA4
{
    TraceStep (0xF);
    SystemError ( 'B', Code );
}

ULONG GetSr (void)          // 0xBFC03968
{
    return COP0.SR;
}

void SetSr (ULONG Value)        // 0xBFC03978
{
    COP0.SR = Value;
}

void LoadInitKernel ()         // 0xBFC00420
{
    memcpy ( 0xA0000500, 0xBFC10000, 0x8BF0 );

    0xA0000500 ();      // Run kernel initialization (SysInitKMem)
}

CopyA0Table ()         // 0xBFC042D0
{
    memcpy ( 0x200, 0xBFC04300, 0x304 );
}

InitSyscall ()         // 0xBFC042A0
{
    //
    // Copy 0xA0, 0xB0 and 0xC0 syscall stubs from kernel image
    //

    memcpy ( 0xA0, 0xA0000510, 0x30 );
}

ClearStack ()         // 0xBFC0D850
{
    PVOID StartAddress;
    PVOID EndAddress;
    StartAddress = 0xA0010000;
    EndAddress = sp | 0xA0000000;
    memset ( StartAddress, 0, EndAddress - StartAddress );
}

ReinitKernel ()     // 0xBFC06F28
{
    printf ( "KERNEL SETUP!\n" );

    SysInitMemory ( 0xA000E000, 0x2000 );       // Kernel heap

    InitEventHandlers (4);

    InitException (0);

    InitDefInt (3);

    InitEvents ( dword_B944 );

    InitThreads ( 1, dword_B940 );

    InitRCnt (1);

    sub_BFC071A0 ();
}

char Licensed[] = "Licensed by Sony Computer Entertainment Inc.";       // 0xBFC0E288

int CheckPIO2 ()         // 0xBFC070AC
{
    char * Source = Licensed;
    char * Dest = 0x1F000004;       // PIO Header 2

    while ( *Source )
    {
        if ( *Source != *Dest ) break;

        Source++;
        Dest++;
    }

    if ( *Source ) return 0;
    else return 1;    
}

BootPIO ()      // 0xBFC07148
{
    printf ( "PIO SHELL for PlayStation(tm)\n" );

    printf ( "%s\n", 0x1F000004 );

    dword.0x1F000000 ();        // Jump by pointer.
}

void ParseConfig (char *Text, PSYSTEM_CONFIG Config, char *Exec )     // 0xBFC008A0
{
    Config->Tcb = 0;
    Config->Event = 0;
    Config->Stack = 0;

    Exec[0] = '\0';
    byte.0x180 = 0;         // Version number

    ParseConfigInt ( Text, &Config->Tcb, "TCB" );

    ParseConfigInt ( Text, &Config->Event, "EVENT" );

    ParseConfigInt ( Text, &Config->Stack, "STACK" );

    ParseConfigString ( Text, Exec, 0x180, "BOOT" );
}

LoadRunShell ()     // 0xBFC06FF0
{
    memcpy ( 0x80030000, 0xBFC18000, 0x67FF0 );

    FlushCache ();

    0x80030000 ();
}

//
// Executive init
//

int InitEventHandlers (int Num)   // 0xBFC04610
{
    int Bytes = Num * 8;
    Pointer = SysMalloc ( Bytes );

    if ( Pointer )
    {
        bzero ( Pointer, Bytes );
        dword_100 = Pointer;
        dword_104 = Bytes;
        return Bytes;
    }
    else return 0;
}

PVOID InitEvents (int Num)    // 0xBFC04678
{
    int Bytes;
    int n;
    PVOID Pointer;
    EvCB * Event;

    printf ( "\nConfiguration : EvCB\t0x%02x\t\t", Num );

    Bytes = Num * sizeof (EvCB);
    Pointer = SysMalloc (Bytes);

    if ( Pointer )
    {
        dword_124 = Pointer;
        dword_120 = Bytes;

        //
        // Clear "status" field for all event CBs.
        //

        for (n=0; n<Num; n++)
        {
            Event = &dword_124[n];
            Event->status = 0;
        }

        return Pointer;
    }
    else return 0;
}

int InitThreads (int Tcbh, int Tcb)     // 0xBFC0472C
{
    TCBH * TCBH_Ptr;
    TCB * TCB_Ptr;
    TCBH * TcbhEntry;
    TCBH * TcbEntry;
    int n;

    printf ( "TCB\t0x%02x\n", Num );

    dword_10C = Tcbh * 4;       // BUGCHECK: Should be actually 8, since TCBH has additional "flag" field defined in KERNEL.H
    dword_114 = Tcb * 192;

    TCBH_Ptr = SysMalloc ( dword_10C );
    if ( TCBH_Ptr == NULL )
        return 0;

    TCB_Ptr = SysMalloc ( dword_114 );
    if ( TCB_Ptr == NULL )
        return 0;

    //
    // Clear TCBH
    //

    for (n=0; n<Tcbh; n++)
    {
        TcbhEntry = &TCBH_Ptr[n];
        TcbhEntry->entry = NULL;
    }

    //
    // Clear TCB
    //

    for (n=0; n<Tcb; n++)
    {
        TcbEntry = &TCB_Ptr[n];
        TcbEntry->status = TcbStUNUSED;
    }

    //
    // Set active first TCB entry
    //

    TCB_Ptr[0].status = TcbStACTIVE;
    TCBH_Ptr[0].entry = TCB_Ptr;

    //
    // Update ToT
    //

    dword_108 = TCBH_Ptr;
    dword_110 = TCB_Ptr;

    return dword_10C + dword_114;       // Total size of TCBH and TCB tables
}
```

## Kernel (PlayStation OS)

The PS OS kernel is resident in memory. The kernel procedures are accessed through special tables (which are located at addresses 0xA0, 0xB0, 0xC0).

The second way to call some kernel mechanisms is through the Syscall instruction (but its function set is limited, in fact it is only used for EnterCriticalSection / ExitCriticalSection)

Applications also have access to a special kernel "Table of Tables" (ToT), through which the program can get various system descriptors, etc.

Execution of user programs is done in CPU Kernel Mode, because only one "process" (executable game file) can be running at a time.

## Kernel memory map

SCPH-1001 Kernel map:

- 0x500: Startup code and system tables (Startup.asm)
- 0xC80: Exception handling (Exception.asm)
- 0x1030: Memory manager (Memory.c)
- 0x1420: Interrupt queue control (Int.c)
- 0x1508: Root counters (RCnt.c)
- 0x1794: Events (Event.c)
- 0x1F88: Threads (Thread.c)
- 0x27C0: Device drivers (IO Subsystem)
- 0x43D0: PAD driver
- 0x47C0: CARD driver
- 0x609C: Another PAD stuff (PAD_init and PAD_dr)
- 0x6A50: Syscall stubs
- 0x6C60: Various pre-initialzed data and strings (.sdata)
- 0x7460: KernelData struct (zeroed by startup code)
- 0xE000: Kernel Heap (0x2000 bytes)

## Kernel Startup

```c
//
// Kernel Startup and system tables.
// Kernel image is hardcopied at 0xA0000500
//

//
// This stuff is written on asm.
//

.org 0x500

SysInitKMem ()         // bios:BFC10000, kernel:0x500
{
    goto ClearKernelData;
}

SysStubA0:      // 0x510
    la      $t0, TableA0Handler
    jr      $t0
    nop

SysStubB0:      // 0x520
    la      $t0, TableB0Handler
    jr      $t0
    nop

SysStubC0:      // 0x530
    la      $t0, TableC0Handler
    jr      $t0
    nop

PatchA0Table ()     // 0x540
{
    memcpy ( 0x200, Patch_A0_1, sizeof(Patch_A0_1) );       // Patch from A0:00

    memcpy ( 0x2EC, Patch_A0_2, sizeof(Patch_A0_2) );       // Patch from A0:3C
}

ClearKernelData ()         // bios:0xBFC10098, kernel:0x598
{
    //
    // Clear kernel data.
    //

    memset ( 0xA0007460, 0, 0x14C0 );
}

TableA0Handler (t1)       // 0x5C4
{
    dword.0x200[t1] ();
}

TableB0Handler (t1)     // 0x5E0
{
    dword.B0_Table[t1] ();
}

TableC0Handler (t1)     // 0x600
{
    dword.C0_Table[t1] ();
}

ULONG GetCause ()       // 0x620
/* UNUSED */
{
    return COP0.Cause;
}

ULONG GetSr ()      // 0x630
/* UNUSED */
{
    return COP0.SR;
}

void SetSr (ULONG Value)    // 0x640
/* UNUSED */
{
    COP0.SR = Value;
}

void SwitchThread ()        // 0x650
{
    Syscall (0);    // Execute syscall instruction
}

PVOID GetC0Table ()         // 0x65C
{
    return C0_Table;
}

PVOID GetB0Table ()     // 0x668
{
    return B0_Table;
}

//
// 0x674
//

C0_Table:       .word InitRCnt
                .word InitException
                .word SysEnqIntRP
                .word SysDeqIntRP
                .word get_free_EvCB_slot
                .word get_free_TCB_slot
                .word ExceptionHandler
                .word InstallExceptionHandlers
                .word SysInitMemory
                .word SysInitKMem
                .word ChangeClearRCnt
                .word SystemError
                .word InitDefInt
                .word ChangeClearDefInt
                .word dev_stub
                .word dev_stub
                .word dev_stub
                .word dev_stub
                .word InstallDevices
                .word FlushStdInOutPut
                .word dev_stub
                .word _cdevinput
                .word _cdevscan
                .word _circgetc
                .word _circputc
                .word ioabort
                .word sub_3E5C
                .word KernelRedirect
                .word PatchA0Table
                .word sub_3E68
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

//
// 0x874
//

B0_Table:       .word SysMalloc
                .word sub_1408
                .word SetRCnt
                .word GetRCnt
                .word StartRCnt
                .word StopRCnt
                .word ResetRCnt
                .word DeliverEvent
                .word OpenEvent
                .word CloseEvent
                .word WaitEvent
                .word TestEvent
                .word EnableEvent
                .word DisableEvent
                .word OpenTh
                .word CloseTh
                .word ChangeTh
                .word 0
                .word InitPad
                .word StartPad
                .word StopPAD
                .word PAD_init
                .word PAD_dr
                .word ReturnFromException
                .word ResetEntryInt
                .word HookEntryInt
                .word Unk_B0_1A
                .word Unk_B0_1B
                .word Unk_B0_1C
                .word Unk_B0_1D
                .word Unk_B0_1E
                .word Unk_B0_1F
                .word UnDeliverEvent
                .word Unk_B0_21
                .word Unk_B0_22
                .word Unk_B0_23
                .word 0, 0, 0, 0, 0, 0
                .word Unk_B0_2A
                .word Unk_B0_2B
                .word 0, 0, 0, 0, 0, 0

//
// 0x93C
// Patch from A0:00
//

Patch_A0_1:     .word open
                .word lseek
                .word read
                .word write
                .word close
                .word ioctl
                .word exit
                .word Unk_B0_39
                .word getc
                .word putc

//
// 0x964
// Patch from A0:3C
//

Patch_A0_2:     .word getchar
                .word putchar
                .word gets
                .word puts

//
// 0x974 - Never patched A0 callbacks
//

off_974:        .word cd
                .word format
                .word firstfile
                .word nextfile
                .word rename
                .word delete
                .word undelete
                .word AddDevice
                .word RemoveDevice
                .word PrintInstalledDevices
                .word InitCARD
                .word StartCARD
                .word StopCARD
                .word _card_write
                .word _card_read
                .word _new_card
                .word Krom2RawAdd
                .word sub_65E0
                .word Unk_B0_52
                .word sub_6670
                .word get_errno
                .word get_error
                .word GetC0Table
                .word GetB0Table
                .word _card_chan
                .word sub_3678
                .word SysHalt
                .word ChangeClearPad
                .word _card_status
                .word _card_wait
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
```

## Kernel Exception handling

```c
//
// Kernel exception handling.
// Written on asm.
//

//
// Interrupt callback queue
// dword_100 (ToT[0]) contain pointer on 4 interrupt callback queues :
// 		Queue 0 : CPU exception handlers (syscalls, overflow etc.)
// 		Queue 1 : Root counter interrupt handlers (4)
//		Queue 2 : Not used by kernel
//		Queue 3 : Hardware interrupt handlers
//

typedef struct _IntRP 
{
	struct _IntRP * next; 		// NULL terminated
	int  	(*func2)(int);
	int 	(*func1)(void);
	long 	flag; 				// Always 0
} IntRP;

typedef struct _ExCB
{
	IntRP 	*head; 		// NULL if queue is empty
	long 	flag;
} ExCB;

//
// Variables
//

PVOID 	KernelSp = 0x85D8; 			// 0x6CF0

jmp_buf * EntryInt; 			// 0x75D0 (somewhere in KernelData)

//
// Default return from exception route
//

jmp_buf DefaultEntryInt = {  		// 0x6CF4
	ReturnFromException,
	0x85D4, 		// sp, KernelData + sizeof(KernelData) + 0x1200
	0, 		// fp
	0, 0, 0, 0, 0, 0, 0, 0,  		// S0 - S7
	0  		// gp
};

VOID ExceptionHandler (VOID) 		// 0xC80
{
	TCHB * TCBH_Entry;
	TCBH * TCB_Entry;
	ULONG Cause, Epc;
	int Opcode;
	ExCB * Head;
	IntRP * Entry;
	int n;
	int Result;

	nop x4; 		// 4 Nops (?)

	TCBH_Entry = dword_108; 		// TCBH
	TCB_Entry = TCBH_Entry->entry; 		// Current TCB (k0)

	//
	// Save context in current TCB
	// k0 is never saved and used as temp kernel pointer.
	//

	TCB_Entry->reg[R_AT] = at;
	TCB_Entry->reg[R_V0] = v0;
	TCB_Entry->reg[R_V1] = v1;
	TCB_Entry->reg[R_RA] = ra;

	GetCauseEpc ( &Cause, &Epc );

	if ( (Cause & 0x3f) == 0 )
	{
		Opcode = (*Epc >> 24) & 0xFE;
		if ( Opcode == 0x4A ) 
			Epc += 4;
	}

	TCB_Entry->reg[R_EPC] = Epc;

	nop x16; 		// 16 Nops (Reset pipeline?)

	TCB_Entry->reg[R_A0] = a0;
	TCB_Entry->reg[R_A1] = a1;
	TCB_Entry->reg[R_A2] = a2;
	TCB_Entry->reg[R_A3] = a3;

	TCB_Entry->reg[R_SR] = COP0.SR;
	TCB_Entry->reg[R_CAUSE] = COP0.Cause;

	TCB_Entry->reg[R_K1] = k1;

	TCB_Entry->reg[R_S0] = s0;
	TCB_Entry->reg[R_S1] = s1;
	TCB_Entry->reg[R_S2] = s2;
	TCB_Entry->reg[R_S3] = s3;
	TCB_Entry->reg[R_S4] = s4;
	TCB_Entry->reg[R_S5] = s5;
	TCB_Entry->reg[R_S6] = s6;
	TCB_Entry->reg[R_S7] = s7;

	TCB_Entry->reg[R_T0] = t0;
	TCB_Entry->reg[R_T1] = t1;
	TCB_Entry->reg[R_T2] = t2;
	TCB_Entry->reg[R_T3] = t3;
	TCB_Entry->reg[R_T4] = t4;
	TCB_Entry->reg[R_T5] = t5;
	TCB_Entry->reg[R_T6] = t6;
	TCB_Entry->reg[R_T7] = t7;
	TCB_Entry->reg[R_T8] = t8;
	TCB_Entry->reg[R_T9] = t9;

	TCB_Entry->reg[R_GP] = gp;
	TCB_Entry->reg[R_SP] = sp;
	TCB_Entry->reg[R_FP] = fp;

	TCB_Entry->reg[R_HI] = HI;
	TCB_Entry->reg[R_LO] = LO;

	//
	// Invoke 4 interrupt callback queues (IntRPs)
	//

	sp = KernelSp;
	Head = dword_100;		// ToT[0]
	gp = 0xF450;
	fp = sp;

	for ( n=0; n<4; n++ )
	{
		Entry = Head[n].head;

		while ( Entry )
		{
			if ( Entry->func1 )
			{
				if ( Result = Entry->func1() && Entry->func2 )
				{
					Entry->func2 ( Result );
				}
			}
			Entry = Entry->next;
		}
	}

	//
	// Restore context from EntryInt
	//

	k0 = &TCBH_Entry->entry.reg;		// Unused

	a0 = dword_75D0;	// jmp_buf * EntryInt
	a1 = 1;

	ra = JmpBuf[JB_PC];
	gp = JmpBuf[JB_GP];
	sp = JmpBuf[JB_SP];
	fp = JmpBuf[JB_FP];
	s0 = JmpBuf[JB_S0];
	s1 = JmpBuf[JB_S1];
	s2 = JmpBuf[JB_S2];
	s3 = JmpBuf[JB_S3];
	s4 = JmpBuf[JB_S4];
	s5 = JmpBuf[JB_S5];
	s6 = JmpBuf[JB_S6];
	s7 = JmpBuf[JB_S7];

	v0 = 1;
	jr ra;
}

GetCauseEpc () 		// 0xEA0
{
	v0 = COP0.Cause;
	v1 = COP0.EPC;
}

InstallExceptionHandlers () 	// 0xEB0
{
	memcpy ( 0x80000080, ExceptionStub, sizeof(ExceptionStub) );    // General Exception

	memcpy ( 0x80000000, ExceptionStub, sizeof(ExceptionStub) );    // TLB Miss

	FlushCache ();
}

ExceptionStub () 		// 0xF0C
{
	goto ExceptionHandler; 		// k0
}

HookEntryInt (jmp_buf * Jmpbuf) 		// 0xF20
{
	EntryInt = Jmpbuf;
}

ResetEntryInt () 		// 0xF2C
{
	EntryInt = &DefaultEntryInt;
}

ReturnFromException () 		// 0xF40
{
	TCHB * TCBH_Entry;
	TCBH * TCB_Entry;

	TCBH_Entry = dword_108; 		// TCBH
	TCB_Entry = TCBH_Entry->entry; 		// Current TCB (k0)

	//
	// Restore context
	//

	LO = TCB_Entry->reg[R_LO];
	HI = TCB_Entry->reg[R_HI];

	COP0.SR = TCB_Entry->reg[R_SR];

	v0 = TCB_Entry->reg[R_V0];
	v1 = TCB_Entry->reg[R_V1];

	a1 = TCB_Entry->reg[R_A1];
	a2 = TCB_Entry->reg[R_A2];
	a3 = TCB_Entry->reg[R_A3];

	t0 = TCB_Entry->reg[R_T0];
	t1 = TCB_Entry->reg[R_T1];
	t2 = TCB_Entry->reg[R_T2];
	t3 = TCB_Entry->reg[R_T3];
	t4 = TCB_Entry->reg[R_T4];
	t5 = TCB_Entry->reg[R_T5];
	t6 = TCB_Entry->reg[R_T6];
	t7 = TCB_Entry->reg[R_T7];

	s0 = TCB_Entry->reg[R_S0];
	s1 = TCB_Entry->reg[R_S1];
	s2 = TCB_Entry->reg[R_S2];
	s3 = TCB_Entry->reg[R_S3];
	s4 = TCB_Entry->reg[R_S4];
	s5 = TCB_Entry->reg[R_S5];
	s6 = TCB_Entry->reg[R_S6];
	s7 = TCB_Entry->reg[R_S7];

	t8 = TCB_Entry->reg[R_T8];
	t9 = TCB_Entry->reg[R_T9];

	k1 = TCB_Entry->reg[R_K1];
	gp = TCB_Entry->reg[R_GP];
	sp = TCB_Entry->reg[R_SP];
	fp = TCB_Entry->reg[R_FP];
	ra = TCB_Entry->reg[R_RA];
	at = TCB_Entry->reg[R_AT];

	k0 = TCB_Entry->reg[R_EPC];
	a0 = TCB_Entry->reg[R_A0];

	RFE;

	goto k0;
}

PVOID GetKernelSp () 	// 0x1018
/* UNUSED */
{
	return KernelSp;
}
```

## Shell

The BIOS shell is a specially created executable file in PS-X EXE format, which is located inside the ROM (without a header)

The initial boot program (Main procedure) loads it into RAM before launching it.

The shell starts and intercepts control if no game disc is inserted in the drive. Otherwise control is transferred back to the kernel to load the EXE from the disk.

The Shell code contains many library calls, i.e. it has been built with PsyQ.
