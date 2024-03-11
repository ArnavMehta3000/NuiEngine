
project "Testbench"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++latest"

    targetdir "%{wks.location}/Build/%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/"
    objdir "%{wks.location}/Build/Intermediate/%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/"
    targetname "Testbench"

    location "D:/ARNAV/DEV/CPP/NuiEngine/Testbench"
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