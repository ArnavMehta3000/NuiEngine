-- Task to create a new Nui game project
task("nui-create")
	set_category("action")
	on_run("CreateProject")

    -- Set the command line options for the plugin. There are no parameter options here, just the plugin description.
    set_menu
    {
        -- Settings menu usage
        usage = "xmake nui-project [project-name]",
        -- Setup menu description
        description = "Create a new Nui project",

        -- Set menu options, if there are no options, you can set it to {}
        options =
        {
            -- Set kv as the key-value parameter and set the default value: black
            {'p', "project", "kv", "NuiGame", "Set the project name." }
        }
    }
task_end()

task("nui-packassets")
	set_category("action")
	on_run("PackAssets")

    -- Set the command line options for the plugin. There are no parameter options here, just the plugin description.
    set_menu
    {
        -- Settings menu usage
        usage = "xmake nui-packassets [project-name]",
        -- Setup menu description
        description = "Package assets for a Nui project",

        -- Set menu options, if there are no options, you can set it to {}
        options =
        {
            -- Set kv as the key-value parameter and set the default value: black
            {'p', "project", "kv", "NuiGame", "Set the project name." }
        }
    }
task_end()

task("nui-createdocs")
	set_category("action")
	on_run(function ()
        print("Running Doxygen...")
        
        local doxyfilePath = path.join(os.projectdir(), "Docs", "Doxygen", "Doxyfile")
        local outputPath = path.join("build", "Docs")

        -- Copy project doxyfile to temp location
        os.cp(doxyfilePath, os.tmpdir())
        local tempDoxyfilePath = path.join(os.tmpdir(), "Doxyfile")

        -- Check if copy was successful
        if not os.isfile(tempDoxyfilePath) then
            raise("Failed to copy Doxyfile to temp location: " .. tempDoxyfilePath)
            return
        end

        -- Change output directory
        local tempDoxyfile = io.open(tempDoxyfilePath, "a")
        if tempDoxyfile then
            tempDoxyfile:write("OUTPUT_DIRECTORY = " .. outputPath .. "\n")
            tempDoxyfile:close()
        end
        
        -- Run Doxygen
        print("Using Doxyfile: " .. doxyfilePath)
        os.run("doxygen " .. tempDoxyfilePath)
        
        cprint("Created documentation at: " .. outputPath)

        cprint("${green}Done!")
    end)

    -- Set the command line options for the plugin. There are no parameter options here, just the plugin description.
    set_menu
    {
        -- Settings menu usage
        usage = "xmake nui-createdocs",
        -- Setup menu description
        description = "Use Doxygen to create documentation",
    }
task_end()
