workspace "Vi"
	architecture "x64" 
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"

	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architeture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Vi/vendor/GLFW/include"
IncludeDir["Glad"] = "Vi/vendor/Glad/include"
IncludeDir["ImGui"] = "Vi/vendor/imgui"
IncludeDir["glm"] = "Vi/vendor/glm"
IncludeDir["stb_image"] = "Vi/vendor/stb_image"
IncludeDir["PhysX"] = "Vi/vendor/PhysX/PhysX"

include "Vi/vendor/GLFW/"
include "Vi/vendor/Glad/"
include "Vi/vendor/ImGui/"
--include "Vi/vendor/PhysX/physx/compiler/vc17win64/sdk_source_bin/"


project "Vi"
	location "Vi"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"		
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	pchheader "Vipch.h"
	pchsource "Vi/src/Vipch.cpp"


	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",

		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",

		"%{prj.name}/vendor/PhysX/PhysX/include/**.h",
		"%{prj.name}/vendor/PhysX/PhysX/include/**.hpp", 

		"%{prj.name}/vendor/PhysX/PhysX/source/**.h",
		"%{prj.name}/vendor/PhysX/PhysX/source/**.hpp",

		"%{prj.name}/vendor/PhysX/PhysX/pvdruntime/**.h",
		"%{prj.name}/vendor/PhysX/PhysX/pvdruntime/**.hpp"
		
	}

	defines
	{
		"PX_PHYSX_STATIC_LIB",
		"_CRT_SECURE_NO_WARNINGS"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.PhysX}/include",
		"%{IncludeDir.PhysX}/source",
		"%{IncludeDir.PhysX}/pvdruntime",

	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"PhysX_static_64.lib",
		"PhysXExtensions_static_64.lib",
		"PhysXFoundation_static_64.lib",
		"PhysXCooking_static_64.lib",
		"PhysXCommon_static_64.lib",
		"PhysXPvdSDK_static_64.lib",
		"PVDRuntime_64.lib"
		
	}
	
	
	

	filter "system:windows"
		
		systemversion "latest"
		defines
		{
			"VI_PLATFORM_WINDOWS",
			"VI_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
	
	
	filter "configurations:Debug"

		libdirs { "%{prj.name}/vendor/PhysX/PhysX/libs/debug" }
		defines 
		{
			"VI_DEBUG",
			"_DEBUG"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"

		libdirs { "%{prj.name}/vendor/PhysX/PhysX/libs/checked" }
		defines 
		{ 
			"VI_RELEASE",
			"NDEBUG"
		}
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"

		libdirs { "%{prj.name}/vendor/PhysX/PhysX/libs/release" }
		defines 
		{ 
			"VI_DIST",
			"NDEBUG"
		}
		runtime "Release"
		optimize "on"
		symbols "off"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")	

	files
	{
		"%{prj.name}/src/**.h",	
		"%{prj.name}/src/**.cpp"
		
	}

	includedirs
	{
		"Vi/vendor/spdlog/include",
		"Vi/src",
		"Vi/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.PhysX}/include",
		"%{IncludeDir.PhysX}/source",
		"%{IncludeDir.PhysX}/pvdruntime"

	}

	links
	{
		"Vi"
	}



	filter "system:windows"
		systemversion "latest"
	 
		defines
		{

			"VI_PLATFORM_WINDOWS"	
	
		}
	


	filter "configurations:Debug"
		defines 
		{
			"VI_DEBUG",
			"_DEBUG"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines
		{ 
			"VI_RELEASE",
			"NDEBUG"
		}
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines 
		{ 
			"VI_DIST",
			"NDEBUG"
		}
		runtime "Release"
		optimize "on"
		symbols "off"