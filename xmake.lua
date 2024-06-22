local solution_name = "Nui"

-- XMake version
set_xmakever("2.9.2")

-- Build configs
add_rules("mode.debug", "mode.release", "mode.releasedbg")
set_allowedmodes("debug", "release", "releasedbg")

-- Platform and architecture - only support x64 Windows
set_allowedplats("windows")
set_allowedarchs("windows|x64")

-- Set C/C++ language version
set_languages("c17", "cxx23")

-- Project name and version
set_project(solution_name)
set_version("0.0.1")

-- Set default build mode
set_defaultmode("debug")

-- Generate clang compile commands on build
-- add_rules("plugin.compile_commands.autoupdate")

-- Update generated visual studio project files on build
-- add_rules("plugin.vsxmake.autoupdate")

-- Add defines
add_defines("UNICODE")

if is_mode("debug", "releasedbg") then
	add_defines("NUI_DEBUG")
end

if is_mode("release") then
    add_defines("NUI_RELEASE")
end

-- Include all xmake projects
includes("**/xmake.lua")
