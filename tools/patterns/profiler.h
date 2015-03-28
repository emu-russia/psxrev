
typedef struct _PROFILER_ENTRY
{
    LIST_ENTRY  ListEntry;

    char        ProcName[128];

    LARGE_INTEGER   StartTime;

    LARGE_INTEGER   StopTime;

    LARGE_INTEGER   ExecutionTime;

} PROFILER_ENTRY, *PPROFILER_ENTRY;

BOOLEAN PerfRunning(void);

void PerfInit(void);

void PerfShutdown(void);

void PerfRegisterEntity(char *ProcName);

PPROFILER_ENTRY PerfGetEntry(char *ProcName);

void PerfStart(char *ProcName);

void PerfStop(char *ProcName);

void PerfUpdateStats(HDC hdc);

//
// Quick macros for production build
//

#define PERF_START(x)  PerfStart(x)

#define PERF_STOP(x)   PerfStop(x)
