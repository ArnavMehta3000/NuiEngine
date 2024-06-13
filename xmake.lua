set_xmakever("2.9.2")

add_rules("mode.debug", "mode.release", "mode.releasedbg")
set_allowedmodes("debug", "release", "releasedbg")

set_allowedplats("windows")
set_allowedarchs("windows|x64")

set_languages("clatest", "cxxlatest")

set_project("NuiXmake")
set_version("0.0.1")

add_defines("UNICODE")

add_rules("plugin.compile_commands.autoupdate")

includes("**/xmake.lua")