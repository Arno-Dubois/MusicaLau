#pragma once

// Debug tracing. Compiled out unless MUSICALAU_DEBUG is defined
// (configure with -DMUSICALAU_DEBUG=ON).
#ifdef MUSICALAU_DEBUG
#include <iostream>
#define MUSICALAU_LOG(expr) (std::cout << expr << std::endl)
#else
#define MUSICALAU_LOG(expr) ((void)0)
#endif