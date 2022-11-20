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

include "Vi/vendor/GLFW/"
include "Vi/vendor/Glad/"
include "Vi/vendor/ImGui/"

project "Vi"
	location "Vi"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	pchheader "Vipch.h"
	pchsource "Vi/src/Vipch.cpp"



	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"

	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}



	filter "system:windows"
		cppdialect "C++17"
		
		systemversion "latest"
	 
		defines
		{

			"VI_PLATFORM_WINDOWS",
			"VI_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
	
	
		postbuildcommands
		{
		   "{COPYDIR} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "Sandbox"
		}

	filter "configurations:Debug"
		defines "VI_DEBUG"
		staticruntime "Off"
		symbols "On"

	filter "configurations:Release"
		defines "VI_RELEASE"
		staticruntime "Off"
		optimize "On"

	filter "configurations:Dist"
		defines "VI_DIST"
		staticruntime "Off"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	
	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",	
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Vi/vendor/spdlog/include",
		"Vi/src"
		
	}

	links
	{
		"Vi"
	}



	filter "system:windows"
		cppdialect "C++17"

		systemversion "latest"
	 
		defines
		{

			"VI_PLATFORM_WINDOWS"	
	
		}
	


	filter "configurations:Debug"
		defines "VI_DEBUG"
		staticruntime "Off"
		symbols "On"

	filter "configurations:Release"
		defines "VI_RELEASE"
		staticruntime "On"
		optimize "On"

	filter "configurations:Dist"
		defines "VI_DIST"
		staticruntime "On"
		optimize "On"
