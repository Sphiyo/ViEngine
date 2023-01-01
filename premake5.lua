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

include "Vi/vendor/GLFW/"
include "Vi/vendor/Glad/"
include "Vi/vendor/ImGui/"

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

	}

	defines
	{
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
		"%{IncludeDir.stb_image}"

	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
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
		defines "VI_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "VI_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "VI_DIST"
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
		"%{IncludeDir.glm}"

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
		defines "VI_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "VI_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "VI_DIST"
		runtime "Release"
		optimize "on"
		symbols "off"