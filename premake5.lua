workspace "AOC2024"
    -- location
    architecture "x64"
    startproject "AOC"
    configurations
    {
        "Debug",
        "Release"
    }

GOL_PATH = "C:/Repos/gol"

include (GOL_PATH .. "/gollib.lua")

project "AOC"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"
		
	targetdir "build/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir "build/bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

	filter "action:vs2022"
		cppdialect "C++20"
		buildoptions  {"/Zc:preprocessor"}
	
	filter "configurations:Debug"
		defines "GOL_DEBUG"
		runtime "Debug"
		optimize "off"
		symbols "on"
	
	filter "configurations:Release"
		defines "GOL_RELEASE"
		runtime "Release"
		optimize "on"
		symbols "off"
	
	filter {}
	
	defines
	{
		--"GOL_USE_CPP_STD"
		"GOL_ENABLE_ASSERTS"
	}
	
	links
	{
		"Golem",
	}
	
	includedirs
	{
		"src/",
		(GOL_PATH.."/src/")
	}
	
	files
	{
		"premake5.lua",
		"src/**.cpp",
		"src/**.h",
		"res/**.txt",
	}
