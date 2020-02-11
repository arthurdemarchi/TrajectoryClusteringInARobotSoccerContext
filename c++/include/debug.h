#pragma once

// debug tool to facilitate printing
// debug messagens to console
// DEBUG is defined via cmakelist
// that said, when compiled for release
// debug(a) lines will be compiled as blank
//being ignored by the compiler
#ifdef DEBUG
#define debug(a) std::cout << a << std::endl
#else
#define debug(a)
#endif