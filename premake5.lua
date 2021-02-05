include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Hyperion"
	architecture "x86_64"
	startproject "Hyperion"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["asio"] = "%{wks.location}/vendor/asio/asio/include"
IncludeDir["json"] = "%{wks.location}/vendor/json/single_include/nlohmann"
IncludeDir["spdlog"] = "%{wks.location}/vendor/spdlog/include"

group "Dependencies"
	include "vendor/premake"
group ""

project "Hyperion"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir)
	objdir ("%{wks.location}/bin-int/" .. outputdir)

	files 
	{ 
		"src/**.cpp",
		"src/**.h"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"src",
		"src/Hyperion",
		"%{IncludeDir.asio}",
		"%{IncludeDir.json}",
		"%{IncludeDir.spdlog}"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
