#ifndef DEBUG_PRINT
#define DEBUG_PRINT

#ifdef DEBUG
#include <iostream>
#define debug(a) std::cout << a << std::endl
#else
#define debug(a)
#endif

#endif
