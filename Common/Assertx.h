#pragma once

#include "SePlatForm.h"
#include <cassert>
#ifdef SF_PLATFORM_WIN
#include <crtdbg.h>
#endif

void __assert__(const char* file, uint32_t line, const char* func, const char* expr);
void __assertex__(const char* file, uint32_t line, const char* func, const char* expr, const char* msg);

#ifdef NDEBUG
#define Assert(expr) ((void)0)
#define AssertEx(expr, msg) ((void)0)
#else
#define Assert(expr) {if(!(expr)){ __assert__(__FILE__, __LINE__, __FUNCTION__, #expr);}}
#define AssertEx(expr, msg) {if(!(expr)){ __assertex__(__FILE__, __LINE__, __FUNCTION__, #expr, msg);}}
#endif