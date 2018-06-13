// CPU Context

#pragma once

typedef struct
{
    uint32 regs[32];
    uint32 pc;
    uint32 hi, lo;
    uint32 gteControl[32];
    uint32 gteData[32];
} PsxCpuContext;
