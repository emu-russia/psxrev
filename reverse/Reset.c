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
    // Clear 0xA0009000 (BiosData)
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
