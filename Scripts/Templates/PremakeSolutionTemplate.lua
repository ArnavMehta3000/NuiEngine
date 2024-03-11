workspace "%SOLUTION_NAME%"
    configurations { "Debug", "Release", "Shipping" }
    startproject "%PROJECT_NAME%"
    architecture "x64"
    location "%ROOT_DIR%"

    filter "system:windows"
        buildoptions {{ "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }}

group "Engine"
    include "%ROOT_DIR%/Engine/Core/BuildCore.lua"
    --include "%ROOT_DIR%/Engine/Audio/BuildAudio.lua"
    include "%ROOT_DIR%/Engine/Console/BuildConsole.lua"
    --include "%ROOT_DIR%/Engine/Graphics/BuildGraphics.lua"
    --include "%ROOT_DIR%/Engine/Test/BuildTest.lua"
group ""

group "Game"
    include "%PROJECT_DIR%/Build%PROJECT_NAME%.lua"
group ""
