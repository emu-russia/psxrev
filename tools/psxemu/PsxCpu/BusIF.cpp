#include "PsxCpu.h"

BusUnit::BusUnit()
{
    //
    // Allocate and init trap handlers
    //

    for (int accessType = 0; accessType < MaxAccessType; accessType++)
    {
        hwReadTraps[accessType] = new HW_READ_CB[0x10000];
        hwWriteTraps[accessType] = new HW_WRITE_CB[0x10000];

        for (int i = 0; i < 0x10000; i++)
        {
            hwReadTraps[i][accessType] = DummyHwRead;
            hwWriteTraps[i][accessType] = DummyHwWrite;
        }
    }

}

BusUnit::~BusUnit()
{
    //
    // Release trap handlers
    //

    for (int accessType = 0; accessType < MaxAccessType; accessType++)
    {
        delete hwReadTraps[accessType];
        delete hwWriteTraps[accessType];
    }
}

uint32 BusUnit::DummyHwRead()
{
    return 0;
}

void BusUnit::DummyHwWrite(uint32 value)
{

}

//
// Atomic operations
//

uint8 BusUnit::ReadByte(uint32 Address)
{
    return 0;
}

void BusUnit::WriteByte(uint32 Address, uint8 value)
{

}

uint16 BusUnit::ReadHalfword(uint32 Address)
{
    return 0;
}

void BusUnit::WriteHalfword(uint32 Address, uint16 value)
{

}

uint32 BusUnit::ReadWord(uint32 Address)
{
    return 0;
}

void BusUnit::WriteWord(uint32 Address, uint32 value)
{

}

//
// External interface
//

void BusUnit::MapHw(uint32 Address, HW_READ_CB ReadCb, HW_WRITE_CB WriteCb, HwAccessType AccessType)
{

}
