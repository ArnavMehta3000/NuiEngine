
project "{0}"
    kind "SharedLib"
    language "C++"
    cppdialect "C++latest"
    nuget {{ "directxtk_desktop_win10:2023.10.31.1" }}

    targetdir "%{{wks.location}}/build/bin/%{{cfg.architecture}}-%{{cfg.buildcfg}}"
    objdir "%{{wks.location}}/build/int/%{{cfg.architecture}}-%{{cfg.buildcfg}}/%{{prj.name}}"
    targetname "GameDLL"

    location "%{{wks.location}}/{0}/"
    links {{ "CloneEngineCore" }}

    includedirs 
    {{
        "%{{wks.location}}/Engine/",
        "%{{prj.location}}/src/"
    }}

    files
    {{
        "%{{prj.location}}/**.h",
        "%{{prj.location}}/**.hpp",
        "%{{prj.location}}/**.cpp",
        "%{{prj.location}}/**.ixx",
        "%{{prj.location}}/**.hlsl",
        "%{{prj.location}}/**.hlsli",
    }}

    flags
    {{
        "MultiProcessorCompile"
    }}

    filter "configurations:Debug"
        defines {{ "DEBUG", "_DEBUG", "GAME" }}
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines {{ "NDEBUG", "GAME" }}
        runtime "Release"
        optimize "On"
        symbols "On"
        
    filter "configurations:Shipping"
        defines {{ "NDEBUG", "SHIPPING", "GAME" }}
        runtime "Release"
        optimize "On"
        symbols "Off"