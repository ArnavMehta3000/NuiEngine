# About The Project

Nui Engine is a simple editor-less, code-first focused game engine/framework written in C++ 23. The entire journey of making it is being documented on my website [here](https://arnavmehta3000.github.io/posts/mage0/). Active development happpens on the `dev` branch and changes are merged into `main` when stable and tested. 

# Requirements

- Premake5 (included in repository)
- Visual Studio 2022 (solution file generated using Premake)
- Python (to create project files)
- C++ 23

# Using The Engine / Creating a Project

- Clone/Fork the repository
- Delete the `Testbench` folder _(it contains the sandbox project where I test the functionality of the engine. You may keep it for reference purposes)_
- Create a project (follow below)
  - Open commandline
  - Navigate the `Scripts/` folder
  - Run `CreateProject.py -projectname=<PrjName>`
    - Where _<PrjName>_ is the name of your project (and the solution file).
    - The project name does not have to be quoted
      - Usage example: `Scripts/CreateProject.py -projectname=MyGame`
- In the root directory, you will see the generated visual studio solution file along with the game project folder.
- Run `GenerateprojectFiles.bat` from the root whenever any files are added or removed from the engine/game
- You may also delete the following from the `.gitignore` file so that you don't have to create a new project every single time. You can do so by navigating to the bottom of the gitignore file and deleting the marked lines:
  - *.sln
  - *.vcxproj
  - *.vcxproj.filters
  - premake5.lua
- Build the solution using VS2022. The build output will be in the `<Root>/Build/<BuildConfig>/<ProjectName>/` directory

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
- [ ] Graphics
  - [ ] --- TBD ---
- [ ] Audio
  - [ ] --- TBD ---
- [ ] Console
  - [ ] --- TBD ---
- [ ] Test
  - [x] Window tests
