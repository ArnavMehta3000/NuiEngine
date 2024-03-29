
project "Testbench"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++latest"

    local baseDir = "%{wks.location}/Build/"

    targetdir(baseDir .. "%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/")
    objdir(baseDir .. "Intermediate/%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/")
    debugdir(baseDir .. "%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/")
    targetname "Testbench"

    location "E:/Dev/NuiEngine/Testbench"
    links
    {
        "NuiCore"
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
        "MultiProcessorCompile"
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