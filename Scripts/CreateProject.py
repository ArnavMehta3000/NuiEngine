import os, sys, subprocess
import argparse
from pathlib import Path

g_project_name = ""

def create_dir(dir):
    if not os.path.exists(dir):
        os.makedirs(dir)

def join_dir(dir1, dir2):
    out_path = os.path.join(dir1, dir2)
    out_path = out_path.replace('\\', '/')
    return out_path

def get_root_dir():
    out_path = Path(os.getcwd()).parent.absolute().as_posix()
    out_path = out_path.replace('\\', '/')
    return out_path

def get_project_dir():
    return join_dir(get_root_dir(), g_project_name)

def get_engine_dir():
    return join_dir(get_root_dir(), "Engine")

def get_engine_core_dir():
    return join_dir(get_engine_dir(), "Core")

def get_engine_console_dir():
    return join_dir(get_engine_dir(), "Console")

def get_engine_graphics_dir():
    return join_dir(get_engine_dir(), "Graphics")

def get_engine_audio_dir():
    return join_dir(get_engine_dir(), "Audio")

def get_engine_test_dir():
    return join_dir(get_engine_dir(), "Test")



def parse_cmd_args():
    try:
        # Create a parser
        parser = argparse.ArgumentParser(description="Get the project name")
    
        # Add a command-line argument for the project name
        parser.add_argument("-projectname", type=str, required=True, help="Name of the project")

        # Parse the command-line arguments
        args = parser.parse_args()

        # Access the project name as a string
        global g_project_name
        g_project_name = args.projectname
    except:
        os.system("pause")

def create_project():
    prj_root_dir = get_project_dir()
    prj_src_dir = join_dir(prj_root_dir, g_project_name)

    # Create root source folder
    create_dir(prj_src_dir)

    # Create project premake file
    # Open premake solution file template
    prj_template_file = "Templates/PremakeProjectTemplate.lua"
    with open(prj_template_file, "r") as file:
        content = file.read()

    # Replace placeholder with valid names
    new_content = content.replace("%PROJECT_NAME%", g_project_name)\
                         .replace("%PROJECT_DIR%", get_project_dir())

    # Write premake file
    filename = "Build" + g_project_name + ".lua"
    out_file = join_dir(get_project_dir(), filename)
    with open(out_file, "w") as file:
        file.write(new_content)

def create_solution():
    # Open premake solution file template
    sln_template_file = "Templates/PremakeSolutionTemplate.lua"
    with open(sln_template_file, "r") as file:
        content = file.read()

    # Replace placeholder with valid names
    new_content = content.replace("%SOLUTION_NAME%", g_project_name)\
                         .replace("%PROJECT_NAME%", g_project_name)\
                         .replace("%ROOT_DIR%", get_root_dir())\
                         .replace("%PROJECT_DIR%", get_project_dir())

    # Write premake file
    out_file = join_dir(get_root_dir(), "premake5.lua")
    with open(out_file, "w") as file:
        file.write(new_content)

if __name__ == "__main__":
    parse_cmd_args()

    create_project()
    create_solution()

    os.system("cd .. && GenerateProjectFile.bat")