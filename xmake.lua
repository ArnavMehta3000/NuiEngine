local solution_name = "NuiTestbench"

-- XMake version
set_xmakever("2.9.2")

-- Build configs
add_rules("mode.debug", "mode.release", "mode.releasedbg")
set_allowedmodes("debug", "release", "releasedbg")

-- Platform and architecture
set_allowedplats("windows")
set_allowedarchs("windows|x64")

-- Set C/C++ language version
set_languages("c17", "cxx23")

-- Project name and version
set_project(solution_name)
set_version("0.0.1")

-- Generate clang compile commands
-- add_rules("plugin.compile_commands.autoupdate")

-- Update generated visual studio project files
-- add_rules("plugin.vsxmake.autoupdate")

-- Add defines
add_defines("UNICODE")

if is_mode("debug") then

    -- add macros
    add_defines("DEBUG", "NUI_DEBUG")

    -- enable debug symbols
    set_symbols("debug")

    -- disable optimization
    set_optimize("none")

end

-- if the current compilation mode is release or profile?
if is_mode("release", "releasedbg") then
    if is_mode("release") then
   		-- add macros
        add_defines("RELEASE", "NUI_RELEASE", "NDEBUG")

        -- mark symbols visibility as hidden
        set_symbols("hidden")

        -- strip all symbols
        set_strip("all")
    else
   		-- add macros
        add_defines("RELEASE", "NUI_RELEASE")

        -- enable debug symbols
        set_symbols("debug")
    end
end

-- Include all xmake projects
includes("**/xmake.lua")
