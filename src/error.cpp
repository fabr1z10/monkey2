#include "error.h"
#include <iostream>

static void setConsoleColorRed() {
    std::cerr << "\033[1;31m";
}  // ANSI escape code for red

static void resetConsoleColor() {
    std::cerr << "\033[0m";
}      // Reset color

void __M_Assert(const char* expr_str, bool expr, const char* file, int line, const std::string& msg) {
	if (!expr)
	{
        setConsoleColorRed();
		std::cerr << "[ERROR]:\t" << msg << "\n"
				  << "Expected:\t" << expr_str << "\n"
				  << "Source:\t\t" << file << ", line " << line << "\n";
        resetConsoleColor();
		abort();
	}
}