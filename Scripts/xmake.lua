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
        description = "Create a new Nui game project",

        -- Set menu options, if there are no options, you can set it to {}
        options =
        {
            -- Set kv as the key-value parameter and set the default value: black
            {'p', "project", "kv", "NuiGame", "Set the project name." }
        }
    }
task_end()
