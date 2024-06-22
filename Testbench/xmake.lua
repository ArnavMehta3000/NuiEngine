target("Testbench")
    set_default(true)
    set_kind("windows")
    add_rules("win.sdk.application")
    add_files("**.cpp")
    add_headerfiles("**.h")
    add_includedirs("$(projectdir)/Engine/")
    add_links("NuiCore", "NuiGraphics", "User32.lib")
    add_deps("NuiCore", "NuiGraphics")
    add_linkorders("User32.lib", "NuiCore", "NuiGraphics")
    set_group("Games")

    after_build(function (target)
        -- Import task module
        import("core.base.task")

    	print("Packaging assets...")
        task.run("nui-packassets", { project="Testbench" })
    end)
target_end()
