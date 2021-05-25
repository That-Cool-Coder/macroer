#pragma once
#include <string>
#include <vector>

#if defined(WIN32) || defined(_WIN32)
    #include <Windows.h>
#else
    #include <unistd.h>
    #include <limits.h>
#endif

std::string getFullExecutableName();