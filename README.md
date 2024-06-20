# About The Project

Nui Engine is a simple editor-less, code-first focused game engine/framework written in C++ 23. The entire journey of making it is being documented on my website [here](https://arnavmehta3000.github.io/posts/mage0/). Active development happpens on the `dev` branch and changes are merged into `main` when stable and tested. 

# Documentation

Nui Engine is documented using Doxygen using a [Doxyfile](./Docs/Doxygen/Doxyfile). You can read the auto generated project documentation [here](https://arnavmehta3000.github.io/NuiEngine/).

# Requirements

Nui is written using C++23 and uses [Xmake](https://xmake.io/#/) as its build system (Lua based). You may also use the provided [CMakeLists.txt](./CMakeLists.txt) which is auto generated using a [Github Action](.github/workflows/xmake-to-cmake.yml) which is only up to date for the main branch. The project is currently configured to only run on **Windows x64**.

# Building Testbench

The engine comes with a sandbox project called `Testbench`. This is where I test the engine features. You may delete this project folder or keep it for reference purposes.

> If you plan to keep the `Testbench` project, make sure to delete [`Testbench/xmake.lua`](./Testbench/xmake.lua) file or rename the file to something else so that it does not interfere with your custom project.

## Building Testbench using Xmake

Assuming you have Xmake installed, navigate to the root project directory and open a terminal.

Run the `xmake` command to build the project

```bash
xmake
```

### Choosing a Build Configuration

Optionally you may select a configuration to build for before running the above build command. 

```bash
xmake f -m <Config Mode>
```

Where `<Config Mode>` may be one of the following:
- `debug`
  - To build the debug config (default)
- `release`
  - To build the release config
- `releasedbg`
  - To build the release config with debug info

After setting the build config, [build](#building-testbench-using-xmake) the project again. A build should be created in the `./build/` directory

### Generating Compile Commands

You may also wish to generate `compile_commands.json`. This can be done in two ways

#### Using The Command Line

See [xmake documentation](https://xmake.io/#/plugin/builtin_plugins?id=generate-compiler_commands) for more info.

```bash
xmake project -k compile_commands
```

#### Generate On Build

If you want `compile_commands.json` to be built every time you build using xmake. Open the [xmake.lua](/xmake.lua) from the root directory and uncomment the following line.

```lua
-- Generate clang compile commands
add_rules("plugin.compile_commands.autoupdate")
```

### Generate CMakeLists.txt

To generate CMakeLists.txt file you can run the following command

```bash
xmake project -k cmakelists
```

### Generate IDE project files

You can make use of xmake to generate IDE files such as visual studio solution files.

```bash
xmake project -k vsxmake2022
```

Additional details on how to generate IDE files can be checked out on [Xmake's docs](https://xmake.io/#/plugin/builtin_plugins?id=generate-visualstudio-project)

You can also change the solution file name by changing the `solution_name` variable at the top of the [root xmake.lua](./xmake.lua) file

```lua
local solution_name = "Nui"  -- Change this string to your liking
```


# Using The Engine / Creating a Project

Clone/Fork the repository
```bash
git clone https://github.com/ArnavMehta3000/NuiEngine.git
```

> Delete the included `Testbench` folder (if you choose to keep it: [**SEE THIS**](#building-testbench`))

## Create a New Project

To make a custom Nui Engine game project, we will make use of a [custom xmake action](./Scripts/xmake.lua).

```bash
xmake nui-create -p YOUR_PROJECT_NAME
```

To get additional information on how to use this command you may use the help flag

```bash
xmake nui-create -h
```

This will print out the help message. Info on how to use this command will be at the bottom of the message. _The first half of the message contains common command flags which can be ignored in this case_

After creating a new project, you should have a folder in the root directory with you project name and some default template files. You may try to [build](#building-testbench-using-xmake) your project now

## Allow Generation of CMakeLists.txt

Currently CMakeLists.txt (for Testbench) is [automatically generated](#requirements) whenever the dev branch is merged onto the main branch. But it only happens when a file is changed in the `./Engine/` or `./Testbench/` directory. If you have forked the project and want the github action to run for your project as well (which will not be in the Testbench directory). You will have to make a small modification to the [xmake-to-cmake.yml](./.github/workflows/xmake-to-cmake.yml) workflow file.

In the file make the following change

```yml
name: Generate CMakeLists.txt with xmake

on:
    push:
        branches:
            - main
        paths:
            - "Engine/**"
            - "Testbench/**" # Change 'Testbench' this to your project name
```

# Project Roadmap

The project roadmap will keep on expanding and changing as I keep working on the project. But it gives an overview as to what I may be working on at the moment

- [ ] Generate project files
  - [x] Create premake lua build scripts
  - [x] Create project directory and set up links
  - [ ] Create a project entry point (cpp) file + template
- [ ] Engine
  - [x] Logging
  - [x] Assertion
  - [x] Filesystem
  - [x] Application
    - [x] Windowing
    - [x] Input
    - [x] Framework
  - [x] ECS
  - [ ] Asset pipeline
  - [x] Upgrade build system
  - [x] Transform system
  - [x] Render system
- [ ] Graphics
  - [x] Initialize DX11
  - [x] Support window resizing
  - [ ] Renderer design
- [ ] Audio
  - [ ] --- TBD ---
- [ ] Test
  - [x] Window tests

# Broken Tests

Due to the build pipeline being changed from Premake5 to XMake, the Tests are not compiling and therefor not included when compiling the project. Although the source code for it still exists in the [Test directory](./Engine/Test/).
