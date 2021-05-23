#include "getExecutableName.hpp"

#if defined(WIN32) || defined(_WIN32)

std::string getFullExecutableName()
{
    WCHAR path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);
    std::wstring ws(path);
    return std::string(ws.begin(), ws.end());
}
#else

std::string getFullExecutableName()
{
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
}

#endif
