// 2026-06-19

#ifndef PROFILING_H
#define PROFILING_H

////////////////////////////
//- Profiling Macros

#ifndef PROFILE_TRACY
#define PROFILE_TRACY 0
#endif

////////////////////////////
//- Tracy Profiling Includes

#if PROFILE_TRACY
# define TRACY_ENABLE 1
# include "tracy/tracy.h"
# pragma comment(lib, "TracyClient.lib")
#endif

////////////////////////////
//- Tracy Profiling Macros

#if PROFILE_TRACY
# define ProfBegin(name) TracyCZoneN(ctx, (name), 1)
# define ProfEnd() TracyCZoneEnd(ctx)
# define ProfScopeBegin() 
# define ProfScopeEnd() TracyCFrameMark
#endif

////////////////////////////
//- Stub Profiling Macros

#if !defined(ProfBegin)
# define ProfBegin(name) (void)(0)
# define ProfEnd() (void)(0)
# define ProfScopeBegin() (void)(0) 
# define ProfScopeEnd() (void)(0)
#endif

#endif // PROFILING_H
