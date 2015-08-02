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
