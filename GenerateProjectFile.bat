@echo off

REM Run premake5 to generate Visual Studio 2022 project files
call Scripts\premake5.exe vs2022

REM Check if an argument was provided
if "%1" == "" (
    REM Generating Clang JSON Database without any configuration specified
    powershell ".\Scripts\clang-build.ps1 -export-jsondb"
) else (
    REM Generating Clang JSON Database with the provided configuration
    powershell ".\Scripts\clang-build.ps1 -aVcxprojConfigPlatform '%1|x64' -export-jsondb"
)
