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

project "Vi"
	location "Vi"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.c"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include"

	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
	 
		defines
		{

			"VI_PLATFORM_WINDOWS",
			"VI_BUILD_DLL"
	
		}
	
	
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "Sandbox")
		}

	filter "configurations:Debug"
		defines "VI_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "VI_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "VI_DIST"
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
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.c"
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
		staticruntime "On"
		systemversion "latest"
	 
		defines
		{

			"VI_PLATFORM_WINDOWS"	
	
		}
	


	filter "configurations:Debug"
		defines "VI_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "VI_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "VI_DIST"
		optimize "On"
