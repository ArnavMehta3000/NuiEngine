
project "NuiTest"
	kind "SharedLib"
	language "C++"
	cppdialect "C++latest"

	local baseDir = "%{wks.location}/Build/"

	targetdir(baseDir .. "%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/")
	objdir(baseDir .. "Intermediate/%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/")
	debugdir(baseDir .. "%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/")
	targetname "NuiTest"

	location "%{wks.location}/Engine/Test/"
	links
	{
		"NuiCore",
		"NuiGraphics"
	}

	includedirs 
	{
		"%{wks.location}/Engine/",
		"$(VCInstallDir)UnitTest/include/"
	}

	libdirs
	{
		"$(VCInstallDir)UnitTest/lib",		
	}


	files
	{
		"%{prj.location}/**.h",
		"%{prj.location}/**.hpp",
		"%{prj.location}/**.cpp",
		"%{prj.location}/**.ixx",
	}

	flags
	{
		"MultiProcessorCompile"
	}

	filter "configurations:Debug"
		defines { "DEBUG", "_DEBUG", "NUI_DEBUG", "NUI_TEST" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG", "NUI_RELEASE", "NUI_TEST" }
		runtime "Release"
		optimize "On"
		symbols "On"
		  
	filter "configurations:Shipping"
		defines { "NDEBUG", "NUI_SHIPPING", "NUI_TEST" }
		runtime "Release"
		optimize "On"
		symbols "Off"