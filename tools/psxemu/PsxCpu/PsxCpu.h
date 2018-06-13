// Common header

#pragma once

#include <iostream>

using namespace std;

#include "CommonTypes.h"
#include "CpuContext.h"
#include "BusIF.h"
#include "Dmac.h"
#include "Gte.h"
#include "GteFast.h"
#include "GteSlow.h"
#include "Intc.h"
#include "Interp.h"
#include "Jitc.h"
#include "Mdec.h"
#include "RootCounters.h"
#include "Sio.h"

class PsxCpu
{

public:

    PsxCpuContext cpuContext;
    BusUnit busUnit;

    PsxCpu();
    ~PsxCpu();

};

extern PsxCpu Cpu;
