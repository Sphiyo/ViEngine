#pragma once

#ifdef VI_PLATFORM_WINDOWS
    #ifdef VI_BUILD_DLL
        #define VI_API __declspec(dllexport)
    #else
        #define VI_API __declspec(dllimport)
    #endif
#else
    #error Vi is only supported for windows
#endif // VI_PLATFORM_WINDOWS

#define BIT(x) (1 << x)