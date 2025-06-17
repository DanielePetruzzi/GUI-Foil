import numpy as np
import os
import subprocess
from Files_path import current_path

# Function to add a flap to an airfoil XFoil built-in ".gdes"
def main(XH,YH,HDelta):

    # Check and remove files from previous runs
    if os.path.exists("temp.dat"):
        os.remove("temp.dat")

    # Create airfoil paneling instruction
    with open("instruction4.txt", "w") as file:            
            file.write(f"load\ntempfoil.dat\n\ngdes\nflap\n{XH}\n{YH}\n{HDelta}\nx\n\nsave\ntemp.dat")

    # Run XFoil for airfoil paneling
    subprocess.run([f"{current_path}\\Run4.bat"])

    # Write "tempfoil.dat" as output
    with open('temp.dat', 'r') as f_in:
        lines = f_in.readlines()[1:]  # Skip the first row

    with open('tempfoil.dat', 'w') as f_out:
        f_out.writelines(lines)
