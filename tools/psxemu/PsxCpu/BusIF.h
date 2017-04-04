// Bus Unit.

#pragma once

#define DRAM_SIZE 2*1024*1024
#define BIOS_SIZE 512*1024
#define SCRATCHPAD_SIZE 0x1000

typedef uint32(*HW_READ_CB)();
typedef void(*HW_WRITE_CB)(uint32 value);

enum HwAccessType
{
    ByteAccess = 0,
    HalfwordAccess,
    WordAccess,
    MaxAccessType,
};

class BusUnit
{

    HW_READ_CB * hwReadTraps[MaxAccessType];
    HW_WRITE_CB * hwWriteTraps[MaxAccessType];

    uint8 * Dram;
    uint8 * Bios;
    uint8 * ScratchPad;

    static uint32 DummyHwRead();
    static void DummyHwWrite(uint32 value);

public:

    BusUnit();
    ~BusUnit();

    //
    // Atomic operations
    //

    uint8 ReadByte(uint32 Address);
    void WriteByte(uint32 Address, uint8 value);
    uint16 ReadHalfword(uint32 Address);
    void WriteHalfword(uint32 Address, uint16 value);
    uint32 ReadWord(uint32 Address);
    void WriteWord(uint32 Address, uint32 value);

    //
    // External interface
    //

    void MapHw(uint32 Address, HW_READ_CB ReadCb, HW_WRITE_CB WriteCb, HwAccessType AccessType);

};
