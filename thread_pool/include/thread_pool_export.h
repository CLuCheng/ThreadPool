#pragma once
#ifdef EXPORT_DLL_SYM
	#ifdef _WIN32
	#define DLL_EXPORT extern"C" _declspec(dllexport)
	#else
#define DLL_EXPORT extern"C" __attribute__((visibility("hidden")))
	#endif 
#else

#ifdef _WIN32
	#define DLL_EXPORT extern"C" _declspec(dllimport)
#else
	#define DLL_EXPORT extern"C" __attribute__((visibility("default")))
	#endif
#endif

