// 2026-06-19

#ifndef PROFILING_H
#define PROFILING_H

////////////////////////////
//- Profiling Includes and Macros

#define TRACY_ENABLE 1
#include "tracy/tracy.h"
#pragma comment(lib, "TracyClient.lib")

#define ProfBegin(name) TracyCZoneN(ctx, (name), 1)
#define ProfEnd() TracyCZoneEnd(ctx)

#endif // PROFILING_H
