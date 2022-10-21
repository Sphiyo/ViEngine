#pragma once



#include <memory>



#include "Core.h"
#include "spdlog/spdlog.h"


namespace Vi {

	class VI_API Log
	{
	public:
		
		static void Init();

		inline static std::shared_ptr < spdlog::logger >& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr < spdlog::logger >& GetClientLogger() { return s_ClientLogger; }

	private:

		static std::shared_ptr < spdlog::logger > s_CoreLogger;
		static std::shared_ptr < spdlog::logger > s_ClientLogger;


	};

}

// CORE LOGS MACROS
#define VI_CORE_TRACE(...)        Vi::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VI_CORE_INFO(...)         Vi::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VI_CORE_WARN(...)         Vi::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VI_CORE_ERROR(...)        Vi::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VI_CORE_CRITICAL(...)     Vi::Log::GetCoreLogger()->critical(__VA_ARGS__)

// CLIENT LOGS MACROS
#define VI_TRACE(...)             Vi::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VI_INFO(...)              Vi::Log::GetClientLogger()->info(__VA_ARGS__)
#define VI_WARN(...)              Vi::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VI_ERROR(...)             Vi::Log::GetClientLogger()->error(__VA_ARGS__)
#define VI_CRITICAL(...)          Vi::Log::GetClientLogger()->critical(__VA_ARGS__)
