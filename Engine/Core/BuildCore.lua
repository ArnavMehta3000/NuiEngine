
project "NuiCore"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    local baseDir = "%{wks.location}/Build/"

    targetdir(baseDir .. "%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/")
    objdir(baseDir .. "Intermediate/%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/")
    debugdir(baseDir .. "%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/")
    targetname "NuiCore"

    location "%{wks.location}/Engine/Core/"
    links
    {
        
    }

    includedirs 
    {
        "%{prj.location}/",
        "%{prj.location}/../",
    }

    files
    {
        "%{prj.location}/**.h",
        "%{prj.location}/**.hpp",
        "%{prj.location}/**.cpp",
        "%{prj.location}/**.inl",
        "%{prj.location}/**.ixx",
    }

    flags
    {
        "MultiProcessorCompile",
        "FatalWarnings"
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