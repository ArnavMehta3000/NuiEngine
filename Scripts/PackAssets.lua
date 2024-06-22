-- Asset pack format
-- - Header:
--   - File type: NUIASSETPAK
--   - Version: 1
--   - Total Count: <number of files>
-- - Files (binary data):
--   - Filepath (relative to the asset directory)
--   - File contents

import("core.base.option")
import("core.base.global")

projectName = nil
projectDir = nil
assetDir = nil

function ReadFiles(directory)
    local files = os.files(path.join(directory, "**"))
    return files
end

function CreatePak(files, pakFileName, baseDir)
    local file = io.open(pakFileName, "wb")
    if not file then
        raise("Failed to create .pak file: " .. pakFileName)
    end

    -- Write the header
    file:write("NUIASSETPAK") -- File type
    file:write(string.char(1)) -- Version
    file:write(string.pack("<I4", #files)) -- Total Count (little-endian uint32)

    cprint("${green}Found assets: ${default}%u", #files)

    for _, filepath in ipairs(files) do
        -- Get the relative path
        local relativePath = path.relative(filepath, baseDir)
        local fileHandle = io.open(filepath, "rb")
        local fileContent = fileHandle:read("*all")
        local fileSize = #fileContent
        fileHandle:close()

        -- Write the file path length and path
        file:write(string.pack("<I4", #relativePath)) -- Path length (little-endian uint32)
        file:write(relativePath) -- Path

        -- Write the file content length, file size and content
        file:write(string.pack("<I4", fileSize)) -- File size (little-endian uint32)
        file:write(fileContent) -- Content

        cprint("${green}Added asset: ${default}%s", relativePath)
    end

    file:close()
end

function main()
    -- Get parameter content and display information
    projectName = option.get("project")

    -- User project directory
    projectDir = path.join(os.projectdir(), projectName)
    -- Project assets directory
    assetDir = path.join(projectDir, "Assets")

    -- Check if the asset directory exists
    if not os.isdir(assetDir) then
        cprint("${yellow}Assets directory does not exist: ${default}%s", assetDir)
        cprint("${blue}Skipping pak file generation")
        return
    end

    local files = ReadFiles(assetDir)

    -- Check if there are any files to pack
    if #files == 0 then
        cprint("${yellow}No files found in assets directory: ${default}%s", assetDir)
        cprint("${blue}Skipping pak file generation")
        return
    end

    -- Create a .pak file from the assets
    local pakFilePath = path.join(path.join("$(buildir)", os.host()), os.arch())

    if is_mode("debug") then
    	pakFilePath = path.join(pakFilePath, "debug")
    elseif is_mode("release") then
    	pakFilePath = path.join(pakFilePath, "release")
    else
    	pakFilePath = path.join(pakFilePath, "releasedbg")
    end

    local pakFileName = path.join(pakFilePath, projectName .. ".pak")
    print("Creating .pak file: " .. pakFileName)

    CreatePak(files, pakFileName, assetDir)

    print("Packaged assets into " .. pakFileName)
end
