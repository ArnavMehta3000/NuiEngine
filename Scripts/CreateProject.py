import os, sys, subprocess
import argparse
from pathlib import Path

root_dir = ""
project_dir = ""
project_name = ""

def parse_cmd_args():
    try:
        # Create a parser
        parser = argparse.ArgumentParser(description="Get the project name")
    
        # Add a command-line argument for the project name
        parser.add_argument("-projectname", type=str, required=True, help="Name of the project")

        # Parse the command-line arguments
        args = parser.parse_args()

        # Access the project name as a string
        global project_name
        project_name = args.projectname
    except:
        os.system("pause")

def create_directories():
    # Set root directory and project directory
    global root_dir
    global project_dir
    root_dir = Path(os.getcwd()).parent.absolute().as_posix()
    project_dir = os.path.join(root_dir, project_name)
    project_src_dir = os.path.join(project_dir, "src")
    if not os.path.exists(project_src_dir):
        os.makedirs(project_src_dir)

    if not os.path.exists(project_dir):
        os.makedirs(project_dir)
        print(f"Directory '{project_dir}' created.")
    else:
        print(f"Directory '{project_dir}' already exists.")

def create_solution_premake_file():
    solution_template_file = "Templates/PremakeSolutionTemplate.lua"
    with open(solution_template_file, "r") as file:
        lua_template = file.read()

    # Replace placeholders with the project name
    formatted_lua = lua_template.format(project_name, root_dir)

    # Create premake5 file in the project directory
    premake5_file = os.path.join(root_dir, f"premake5.lua")
    with open(premake5_file, "w") as file:
        file.write(formatted_lua)
    
    print(f"Created file {premake5_file} in root directory")

def create_premake_files():
    project_template_file = "Templates/PremakeProjectTemplate.lua"
    with open(project_template_file, "r") as file:
        lua_template = file.read()

    # Replace placeholders with the project name
    formatted_lua = lua_template.format(project_name)

    # Create premake5 file in the project directory
    premake_file = os.path.join(project_dir, f"Build{project_name}.lua")
    with open(premake_file, "w") as file:
        file.write(formatted_lua)
    
    print(f"Created file {premake_file} in project directory")


def update_runtime_links():
    runtime_template_file = "Templates/PremakeRuntimeTemplate.lua"
    
    with open(runtime_template_file, 'r') as file:
        content = file.read()

    # Replace the placeholder with the custom project name
    modified_content = content.replace('%USER_PROJECT_NAME%', project_name)
    
    # Reerite the file
    premake_file = os.path.join(root_dir, "Runtime")
    premake_file = os.path.join(premake_file, f"BuildRuntime.lua")
    with open(premake_file, 'w') as file:
        file.write(modified_content)
        
    print(f"Created file {premake_file} in runtime directory")
    


if __name__ == "__main__":
    # Get project name
    parse_cmd_args()

    # Create directories
    create_directories()

    # Create premake files using templates
    create_solution_premake_file()
    create_premake_files()
    
    update_runtime_links()
    
    os.system("cd .. && GenerateProjectFile.bat")