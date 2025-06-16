from cx_Freeze import setup, Executable

base = None    

executables = [Executable("interface.py", base=base)]

packages = ["idna"]
options = {
    'build_exe': {    
        'packages':packages,
    },    
}

setup(
    name = "Interface",
    options = options,
    version = "0.1",
    description = 'test',
    executables = executables
)