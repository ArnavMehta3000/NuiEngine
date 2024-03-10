
project "CloneEngineRuntime"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++latest"
    nuget { "directxtk_desktop_win10:2023.10.31.1" }

    targetdir "%{wks.location}/build/bin/%{cfg.architecture}-%{cfg.buildcfg}"
    objdir "%{wks.location}/build/int/%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}"
    targetname "%USER_PROJECT_NAME%"

    location "%{wks.location}/Runtime/"
    links { "CloneEngineCore", "%USER_PROJECT_NAME%" }

    includedirs 
    {
        "%{wks.location}/Engine/",
        "%{wks.location}/%USER_PROJECT_NAME%/src/",
        "%{wks.location}/External/XWin/include/"
    }

    files
    {
        "%{prj.location}/**.h",
        "%{prj.location}/**.hpp",
        "%{prj.location}/**.cpp",
        "%{prj.location}/**.ixx",
        "%{prj.location}/**.hlsl",
        "%{prj.location}/**.hlsli",
    }

    flags
    {
        "MultiProcessorCompile"
    }

    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG", "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"
        
    filter "configurations:Shipping"
        defines { "NDEBUG", "SHIPPING" }
        runtime "Release"
        optimize "On"
        symbols "Off"