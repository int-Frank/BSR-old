#ifndef OPTIONS_H
#define OPTIONS_H

#include "LogLevel.h"
#include "DgAssert.h"

#ifdef BSR_DEBUG
#define BSR_ASSERT(...) DG_ASSERT(__VA_ARGS__)
#else
#define BSR_ASSERT(...)
#endif

//----------------------------------------------------------------------------
// Switches
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Constants
//----------------------------------------------------------------------------
#define LOG_OUTPUT_FILE "log-output.txt"
#define CRASH_REPORT_FILE "crash-report.txt"


//----------------------------------------------------------------------------
// Logging
//----------------------------------------------------------------------------
#if defined(BSR_DEBUG)
#define LOGLEVEL LL_TRACE
#elif defined(BSR_RELEASE)
#define LOGLEVEL LL_WARN
#else
#define LOGLEVEL LL_OFF
#endif

#endif