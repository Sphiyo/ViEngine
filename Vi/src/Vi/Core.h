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

#ifdef HZ_ENABLE_ASSERTS
    #define VI_ASSERT(x, ...){ if(!(x)) { VI_ERROR("Assertion Failed : {0}" , __VA_ARGS__) ; __debugbreak(); } }
    #define VI_CORE_ASSERT(x, ...) { if (!(x)) { VI_CORE_ERROR("Assertion Failed : {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define VI_ASSERT(x, ...)
    #define VI_CORE_ASSERT(x, ...)
#endif 



#define BIT(x) (1 << x)


#define VI_BINDEVENT(x) std::bind(&x, this, std::placeholders::_1)