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

if is_mode("debug") then

    -- Add macros
    add_defines("DEBUG", "NUI_DEBUG")

    -- Enable debug symbols
    set_symbols("debug")

    -- Disable optimization
    set_optimize("none")

end

-- If the current compilation mode is release or debug?
if is_mode("release", "releasedbg") then
    if is_mode("release") then
   		-- Add macros
        add_defines("RELEASE", "NUI_RELEASE", "NDEBUG")

        -- Mark symbols visibility as hidden
        set_symbols("hidden")

        -- Strip all symbols
        set_strip("all")
    else
   		-- Add macros
        add_defines("RELEASE", "NUI_RELEASE")

        -- Enable debug symbols
        set_symbols("debug")
    end
end

-- Include all xmake projects
includes("**/xmake.lua")
