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

rem Invoke the Visual Studio Developer Command Prompt and execute MSBuild with the specified configuration
msbuild "%solutionFileName%" -verbosity:minimal /p:Configuration=%~1
