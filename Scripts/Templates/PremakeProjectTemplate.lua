
project "%PROJECT_NAME%"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++latest"

	-- Base output directory
	local baseDir = "%{wks.location}/Build/"
	
	debugDirectory = baseDir .. "%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/"

	targetdir(baseDir .. "%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/")
	objdir(baseDir .. "Intermediate/%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/")
	debugdir(debugDirectory)
	targetname "%PROJECT_NAME%"

	location "%PROJECT_DIR%"
	links 
	{
		"NuiCore",
		"NuiGraphics"
	}

	includedirs 
	{
		"%{wks.location}/Engine/",
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
		"MultiProcessorCompile",
		"FatalWarnings"
	}

	shadermodel ("5.0")
 
		shaderassembler("AssemblyCode")
		local shader_dir = debugDirectory .. "Shaders/"
 
		-- Treat warnings as errors
		shaderoptions({"/WX"})

	-- HLSL files that don't end with 'Extensions' will be ignored as they will be
	-- used as includes
		filter("files:**.hlsl")
			flags("ExcludeFromBuild")
			shaderobjectfileoutput(shader_dir.."%{file.basename}"..".cso")
			shaderassembleroutput(shader_dir.."%{file.basename}"..".asm")
 
		filter("files:**PS.hlsl")
			removeflags("ExcludeFromBuild")
			shadertype("Pixel")
 
		filter("files:**VS.hlsl")
			removeflags("ExcludeFromBuild")
			shadertype("Vertex")

		filter("files:**GS.hlsl")
			removeflags("ExcludeFromBuild")
			shadertype("Geometry")
		
		filter("files:**HS.hlsl")
			removeflags("ExcludeFromBuild")
			shadertype("Hull")
		
		filter("files:**DS.hlsl")
			removeflags("ExcludeFromBuild")
			shadertype("Domain")

	scanformoduledependencies "true"
	floatingpoint "Fast"

	filter { "system:windows", "action:vs2022"}
		buildoptions
		{
			"/EHsc",
			"/Zc:preprocessor",
			"/Zc:__cplusplus",
			"/experimental:module"
		}

	filter "configurations:Debug"
		defines { "DEBUG", "_DEBUG", "NUI_DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG", "NUI_RELEASE"}
		runtime "Release"
		optimize "On"
		symbols "On"
		  
	filter "configurations:Shipping"
		defines { "NDEBUG", "NUI_SHIPPING"}
		runtime "Release"
		optimize "On"
		symbols "Off"