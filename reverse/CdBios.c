// BIOS Cd Library (not used by PsyQ SDK)

PULONG int_reg_base = 0x1f801070;

#define int_reg int_reg_base[0]
#define int_mask int_reg_base[1]

CdrInit ()   // 0xBFC04850
{
    dword.0xA00091D0[8] = 0;
    dword.0xA00091D0[4] = CdIntr1_Exit;
    dword.0xA00091D0[8] = CdIntr1_Entry;

    SysEnqIntRP ( 0, 0xA00091D0 );

    dword.0xA00091E0[0] = 0;
    dword.0xA00091E0[4] = CdIntr2_Exit;
    dword.0xA00091E0[8] = CdIntr2_Entry;

    SysEnqIntRP ( 0, 0xA00091E0 );
}

CdrShutdown ()   // 0xBFC048D0
{
    SysDeqIntRP ( 0, 0xA00091D0 );

    SysDeqIntRP ( 0, 0xA00091E0 );
}

int BFC04910_A0_95 ()
{
    dword_91C4 = 0;
    dword_91C8 = 0;
    dword_91CC = 0;

    EnterCriticalSection ();

    int_mask &= ~4;         // CDROM
    int_mask &= ~8;         // DMA

    int_reg = ~4;   // CDROM
    int_reg = ~8;   // DMA

    dword_B93C = 1;
    dword_B938 = 1;

    dma_icr = 0x9099;
    dma_icr = (dma_icr & 0xFFFFFF) | 0x8800;    // CDROM, DMA

    dword_9158 = 0xFFFF;
    dword_9154 = 0xCCC;
    dword_9160 = 0;
    dword_915C = 0;

    sub_BFC05120 ();

    sub_BFC050FC ();

    int_reg = ~4;

    int_mask |= 4;      // CDROM
    int_mask |= 8;      // DMA

    ExitCriticalSection ();

    cdreg0 = 0;
    cdreg1 = 0xA;

    count = 30000;

    while ( count-- )
    {
        if ( dword_91C4 == 1 )
            return 1;

        if ( dword_91C4 == 2 )
            return 0;
    }

    return 0;
}

BFC04AA0_A0_94 (PUCHAR a0, PUCHAR a1)
{
    *a0 = byte_91B4;
    *a1 = byte_91B5;
}

sub_BFC05120 ()
{
    volatile int i;

    cdreg0 = 1;
    cdreg3 = 0x1F;

    for (i=0; i<4; i++) dword_0 = i;    // Small wait
}

sub_BFC050FC ()
{
    cdreg0 = 1;
    cdreg2 = 0x1F;
}
