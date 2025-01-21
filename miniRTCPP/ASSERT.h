#pragma once

#include <intrin.h>

#ifdef _WIN64
	#define ASSERT(expr, ...) if(!(expr)) __debugbreak();
#elif  _WIN32
	#define ASSERT(expr, ...) if(!(expr)) __asm{ int 3 }
#else
	#define ASSERT(expr, ...) 
#endif // DEBUG

#define GOTO_BEGIN do {
#define GOTO_END } while(0);
#define GOTO_OUT break