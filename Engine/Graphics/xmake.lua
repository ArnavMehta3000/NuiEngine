target("NuiGraphics")
    set_default(false)
    set_kind("static")
    add_files("**.cpp")
    add_headerfiles("**.h")
    add_includedirs("$(scriptdir)/Engine/")
    add_links("User32.lib")
    set_group("Nui")
target_end()
