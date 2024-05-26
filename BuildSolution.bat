@echo off
setlocal

rem Get the current directory
set "currentDirectory=%CD%"

rem Get the solution file name (assuming there's only one .sln file in the directory)
for %%F in ("%currentDirectory%\*.sln") do set "solutionFileName=%%~nxF"

rem Check if a solution file exists in the current directory
if "%solutionFileName%"=="" (
    echo "No solution file (.sln) found in the current directory."
    exit /b 1
)

rem Load VC
if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\" (
    echo Loading x64 Professional
    call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
) else (
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\" (
        echo Loading x64 Community
        call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    )
)

cls
rem Invoke the Visual Studio Developer Command Prompt and execute MSBuild with the specified configuration
msbuild "%solutionFileName%" -verbosity:minimal /p:Configuration=%~1
