#include "include/thread_utils.h"
#ifdef _WIN32
#include <Windows.h>
#elif defined(__APPLE__)
#include <pthread.h>
#else
#include <sys/prctl.h>
#endif

DLL_EXPORT void setThreadName(const std::string& name)
{
#if defined(__APPLE__)
  pthread_setname_np(name.c_str());
#elif defined(__linux__)
  prctl(PR_SET_NAME,
        reinterpret_cast<unsigned long>(name.c_str()));  // NOLINT
#elif defined(_WIN32)
#pragma pack(push, 8)
  typedef struct tagTHREADNAME_INFO {
    DWORD dwType;      // Must be 0x1000.
    LPCSTR szName;     // Pointer to name (in user addr space).
    DWORD dwThreadID;  // Thread ID (-1=caller thread).
    DWORD dwFlags;     // Reserved for future use, must be zero.
  } THREADNAME_INFO;
#pragma pack(pop)
  const DWORD MS_VC_EXCEPTION = 0x406D1388;
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = name.c_str();
  info.dwThreadID = -1;
  info.dwFlags = 0;

  __try {
    RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(DWORD),
                   reinterpret_cast<ULONG_PTR*>(&info));
  } __except (EXCEPTION_CONTINUE_EXECUTION) {  // NOLINT
  }
#endif
}
