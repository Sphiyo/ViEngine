#pragma once

#include <memory>

#ifdef VI_PLATFORM_WINDOWS
    #if HZ_DYNAMIC_LINK
        #ifdef VI_BUILD_DLL
            #define VI_API __declspec(dllexport)
        #else
            #define VI_API __declspec(dllimport)
        #endif
    #else
        #define VI_API
    #endif
#else
    #error Vi is only supported for windows
#endif // VI_PLATFORM_WINDOWS

#ifdef VI_DEBUG
    #define VI_ENABLE_ASSERTS
#endif

#ifdef VI_ENABLE_ASSERTS
    #define VI_ASSERT(x, ...){ if(!(x)) { VI_ERROR("Assertion Failed : {0}" , __VA_ARGS__) ; __debugbreak(); } }
    #define VI_CORE_ASSERT(x, ...) { if (!(x)) { VI_CORE_ERROR("Assertion Failed : {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define VI_ASSERT(x, ...)
    #define VI_CORE_ASSERT(x, ...)
#endif 



#define BIT(x) (1 << x)


#define VI_BINDEVENT(x) std::bind(&x, this, std::placeholders::_1)


namespace Vi {

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;



}