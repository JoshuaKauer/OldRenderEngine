#ifndef HEADER
#define HEADER

#include "Logger.h"


#define ASSERTS_ON

#ifdef ASSERTS_ON

#define ASSERT(expr, ...) {const char* msg = #expr##" "##__VA_ARGS__##; if (!(expr)) {LOG(Severe, msg); END_LOG DebugBreak(); exit(1);}}

#else
#define ASSERT(expr, )3

#endif

#endif