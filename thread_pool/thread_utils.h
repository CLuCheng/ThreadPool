#include <string>
#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#include <sys/prctl.h>
#endif

void setThreadName(const std::string& name);