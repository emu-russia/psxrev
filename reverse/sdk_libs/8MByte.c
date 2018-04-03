// PSYQ/LIB/8MBYTE.OBJ
// Pseudo C
//

__main ()
{
}

//
// Default entry point for SNSys Toolchain
//
// Written on asm
//

__SN_ENTRY_POINT ()
{

stup2:

    //
    // Clear .sbss
    //

    v0 = &.sbss;

    while ( v0 != &.bss)
    {
        *(uint32 *)v0 = 0;
        v0 += 4;
    }

stup1:

    v0 = 4;

    __nop ();       // Why 4 nops?
    __nop ();
    __nop ();
    __nop ();

    //
    // Setup stack (SP register)
    //

    a0 = loc_2B4;
    a0 += 4;
    v0 = *(uint32 *)a0;             // Stack base ?
    sp = 0x80000000 | v0;

    //
    // Heap, gp, fp
    //

    a0 = &.bss;               // .bss
    a0 <<= 3;       // Remove possible virtual addressing
    a0 >>= 3;

    a1 = v0 - _stacksize - a0;      // Heap size
    a0 |= 0x80000000;           // Heap base

    //
    // Save RA register in first .sbss word
    //

    *(uint32 *)(&.sbss) = ra;
    
    //
    // GP and FP registers
    //

    gp = .sdata;
    fp = sp;

    //
    // InitHeap
    //

    InitHeap (a0 + 4, a1);

    //
    // Restore RA register
    //

    ra = *(uint32 *)(&.sbss);

stup0:

    //
    // User entrypoint
    //

    main ();

    __break(1);     // Break instruction used as marker (?)
    
    //
    // Wtf ? Some compile-time hack parameters?
    //

loc_2B4:
    .word       0      // Stack base possible, filled by toolchain ??
    .word       0      // ???
    .word       0      // ???
    .word       0      // ???

}
