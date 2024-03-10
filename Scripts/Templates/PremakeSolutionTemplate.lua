workspace "{0}"
    configurations {{ "Debug", "Release", "Shipping" }}
    startproject "CloneEngineRuntime"
    architecture "x64"
    location "{1}"

    filter "system:windows"
        buildoptions {{ "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }}

group "Core"
    include "{1}/Runtime/BuildRuntime.lua"
    include "{1}/Engine/BuildEngine.lua"
group ""

group "Game"
    include "{1}/{0}/Build{0}.lua"
group ""

group "External"
    -- Add external projects here
group ""