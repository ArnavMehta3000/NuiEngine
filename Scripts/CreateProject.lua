import("core.base.option")

projectName = nil
templatesPath = nil
scriptDir = nil
rootDir = nil
projectDir = nil


function CreateXmakeFile()
	-- Get file paths
    local templateFilePath = path.join(templatesPath, "XmakeProjectTemplate")
    local outputFilePath = path.join(projectDir, "xmake.lua")

    -- Get template file contents
    local templateFile = io.open(templateFilePath, "r")
    if not templateFile then
       	raise("Failed to open xmake template file. Expected path: " .. templateFilePath)
    end
    local templateContent = templateFile:read("*all")
    templateFile:close()

    -- Replace '%PROJECT_NAME%' with the actual project name
    local modifiedContent = templateContent:gsub("%%PROJECT_NAME%%", projectName)

    -- Create and write to the output file
    local outputFile = io.open(outputFilePath, "w")
    if not outputFile then
        error("Failed to write xmake project file. Expected path: " .. outputFilePath)
    end
    outputFile:write(modifiedContent)
    outputFile:close()

    cprint("Created project file: ${underline}%s", outputFilePath)
end

function CreateMainCppFile()
	-- Get file paths
    local templateFilePath = path.join(templatesPath, "MainCppProjectTemplate")
    local outputFilePath = path.join(projectDir, "Main.cpp")

    -- Get template file contents
    local templateFile = io.open(templateFilePath, "r")
    if not templateFile then
       	raise("Failed to open xmake template file. Expected path: " .. templateFilePath)
    end
    local templateContent = templateFile:read("*all")
    templateFile:close()

    -- Replace '%PROJECT_NAME%' with the actual project name
    local modifiedContent = templateContent:gsub("%%PROJECT_NAME%%", projectName)

    -- Create and write to the output file
    local outputFile = io.open(outputFilePath, "w")
    if not outputFile then
        error("Failed to write xmake project file. Expected path: " .. outputFilePath)
    end
    outputFile:write(modifiedContent)
    outputFile:close()

    cprint("Created main file: ${underline}%s", outputFilePath)
end

function CreateApplicationHeaderFile()
	-- Get file paths
    local templateFilePath = path.join(templatesPath, "ApplicationHeaderProjectTemplate")
    local outputFilePath = path.join(projectDir, path.join(projectName, projectName .. "App.h"))

    -- Get template file contents
    local templateFile = io.open(templateFilePath, "r")
    if not templateFile then
       	raise("Failed to open xmake template file. Expected path: " .. templateFilePath)
    end
    local templateContent = templateFile:read("*all")
    templateFile:close()

    -- Replace '%PROJECT_NAME%' with the actual project name
    local modifiedContent = templateContent:gsub("%%PROJECT_NAME%%", projectName)

    -- Create and write to the output file
    local outputFile = io.open(outputFilePath, "w")
    if not outputFile then
        error("Failed to write xmake project file. Expected path: " .. outputFilePath)
    end
    outputFile:write(modifiedContent)
    outputFile:close()

    cprint("Created application header file: ${underline}%s", outputFilePath)
end

function CreateApplicationCppFile()
	-- Get file paths
    local templateFilePath = path.join(templatesPath, "ApplicationCppProjectTemplate")
    local outputFilePath = path.join(projectDir, path.join(projectName, projectName .. "App.cpp"))

    -- Get template file contents
    local templateFile = io.open(templateFilePath, "r")
    if not templateFile then
       	raise("Failed to open xmake template file. Expected path: " .. templateFilePath)
    end
    local templateContent = templateFile:read("*all")
    templateFile:close()

    -- Replace '%PROJECT_NAME%' with the actual project name
    local modifiedContent = templateContent:gsub("%%PROJECT_NAME%%", projectName)

    -- Create and write to the output file
    local outputFile = io.open(outputFilePath, "w")
    if not outputFile then
        error("Failed to write xmake project file. Expected path: " .. outputFilePath)
    end
    outputFile:write(modifiedContent)
    outputFile:close()

    cprint("Created application cpp file: ${underline}%s", outputFilePath)
end

function main()
	-- Get parameter content and display information
    projectName = option.get("project")

    -- Scripts directory
    scriptDir = os.scriptdir()
    -- Root Nui directory
    rootDir = os.projectdir()
    -- User project directory
    projectDir = path.join(rootDir, projectName)
    -- Template files directory
    templatesPath = path.join(scriptDir, "Templates")

    -- Create project xmake file
    CreateXmakeFile()

    -- Create Main.cpp file
    CreateMainCppFile()

    -- Create <Project>App.cpp file
    CreateApplicationCppFile()

    -- Create <Project>App.h file
    CreateApplicationHeaderFile()

    cprint("${green}Done")
end
