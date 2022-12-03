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
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")
--	os.mkdir("../bin/" .. outputdir .. "%{prj.name}")
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
	
		prebuildcommands
		{
			"{mkdir} ../bin/" .. outputdir .. "Sandbox"

		}
		postbuildcommands
		{
			
		   "{COPYDIR} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "Sandbox"
		  
		}

	filter "configurations:Debug"
		defines "VI_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "VI_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "VI_DIST"
		runtime "Release"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "Off"
	
	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")
--	os.mkdir("bin/" .. "Debug-%{cfg.system}-%{cfg.architeture}" .. "%{prj.name}")
	

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
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "VI_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "VI_DIST"
		runtime "Release"
		optimize "On"
