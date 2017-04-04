// Main module

#include "PsxCpu.h"

PsxCpu Cpu;

PsxCpu::PsxCpu()
{
    cout << "PsxCpu Init" << endl;
}

PsxCpu::~PsxCpu()
{
    cout << "PsxCpu Shutdown" << endl;
}

//
// Interop API
//

extern "C"
{
    __declspec(dllexport) void PsxCpuReset()
    {
        for (int i = 0; i < 32; i++)
        {
            Cpu.cpuContext.regs[i] = 0;
            Cpu.cpuContext.gteControl[i] = 0;
            Cpu.cpuContext.gteData[i] = 0;
        }
        Cpu.cpuContext.pc = 0xbfc00000;
        Cpu.cpuContext.hi = 0;
        Cpu.cpuContext.lo = 0;
    }

    __declspec(dllexport) void PsxCpuExecute()
    {

    }

    __declspec(dllexport) void PsxCpuRunSlow()
    {

    }

    __declspec(dllexport) void PsxCpuRunFast()
    {

    }

    __declspec(dllexport) void PsxCpuBreak()
    {

    }

}
