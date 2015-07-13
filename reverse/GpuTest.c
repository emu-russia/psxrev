// Unused BIOS code for GPU testing.

int GpuTest ()     // 0xBFC00460
{
    //
    // Setup GPU disp/draw environment
    //

    SetGPUStatus (0);
    Bogus1 ();

    SetGPUStatus ( 0x3000000 );
    Bogus1 ();

    SetGPUStatus ( 0x6C60260 );
    Bogus1 ();

    SetGPUStatus ( 0x7040010 );
    Bogus1 ();

    GPU_cw ( 0xE1000400 );
    Bogus1 ();

    GPU_cw ( 0xE3000000 );
    Bogus1 ();

    GPU_cw ( 0xE407FFFF );
    Bogus1 ();

    GPU_cw ( 0xE5000000 );
    Bogus1 ();

    SetGPUStatus ( 0x8000000 );
    Bogus1 ();

    SetGPUStatus ( 0x8000000 );
    Bogus1 ();

    ClearVram ();

    SetGPUStatus ( 0x8000000 );
    Bogus1 ();

    ClearVram ();

    //
    // Draw NTSC color bars
    //

    PolyF4 ( 0, 0, 0x20, 0xF1, 
             0xFF, 0xFF, 0xFF );        // White

    PolyF4 ( 0x20, 0, 0x20, 0xF1,
             0xFF, 0xFF, 0 );           // Yellow

    PolyF4 ( 0x40, 0, 0x20, 0xF1, 
             0, 0xFF, 0xFF );           // Cyan

    PolyF4 ( 0x60, 0, 0x20, 0xF1, 
             0, 0xFF, 0 );              // Green

    PolyF4 ( 0x80, 0, 0x20, 0xF1, 
             0xFF, 0, 0xFF );           // Magenta

    PolyF4 ( 0xA0, 0, 0x20, 0xF1, 
             0xFF, 0, 0 );              // Red

    PolyF4 ( 0xC0, 0, 0x20, 0xF1, 
             0, 0, 0xFF );              // Blue

    PolyF4 ( 0xE0, 0, 0x20, 0xF1, 
             0, 0, 0 );                 // Black

    return 0;
}

BlueScreen ()     // 0xBFC00678
{
    WaitVsync ();

    FillColor ( 0, 0, 0x45 );
}

GreenScreen ()         // 0xBFC006A8
{
    WaitVsync ();

    FillColor ( 0, 0x45, 0 );
}

FillColor (UCHAR red, UCHAR green, UCHAR blue)   // 0xBFC006D8
{
    SetGPUStatus (0);

    SetGPUStatus ( 0x3000000 );

    SetGPUStatus ( 0x6C40240 );

    GPU_cw ( 0xE1000400 );

    GPU_cw ( 0xE3000000 );

    GPU_cw ( 0xE407FFFF );

    GPU_cw ( 0xE5000000 );

    SetGPUStatus ( 0x8000023 );

    PolyF4 ( 0, 0, 640, 240,
             red, green, blue );
}

ClearVram ()    // 0xBFC00778
{
    PolyF4 ( 0, 0, 0x201, 0x101, 
             0, 0, 0 );

    PolyF4 ( 0, 0x100, 0x201, 0x101, 
             0, 0, 0 );    

    PolyF4 ( 0x200, 0, 0x201, 0x101, 
             0, 0, 0 );

    PolyF4 ( 0x200, 0x100, 0x201, 0x101, 
             0, 0, 0 );
}

PolyF4 ( USHORT x0, USHORT y0, USHORT width, USHORT height, 
         UCHAR red, UCHAR green, UCHAR blue )         // 0xBFC00810
{
    ULONG Packet[5];

    // PolyF4
    Packet[0] = 0x28000000 | (blue << 16) | (green << 8) | red;

    Packet[1] = (y0 << 16) | x0;

    Packet[2] = (y0 << 16) | (x0 + width);

    Packet[3] = ((y0 + height) << 16) | x0;

    Packet[4] = ((y0 + height) << 16) | (x0 + width);

    GPU_cwb ( Packet, 5 );
}

WaitVsync ()        // 0xBFC03910
{
    while ( (int_reg & 1) == 0 ) ;  // Wait

    int_reg = ~1;
}
