import numpy as np
import os
import subprocess
from Files_path import current_path

# Function to generate a NACA 4-digits profile using XFoil built-in generator
def main():

    # Input profile number from user 
    data = np.loadtxt('naca.dat')

    # Check and remove files from previous runs
    if os.path.exists("temp.dat"):
        os.remove("temp.dat")

    # Convert input
    m = int(data[0])
    p = int(data[1])
    ss = int(data[2])

    # Create profile generation instruction
    if ss < 10:
        with open("instruction2.txt", "w") as file1:            
            file1.write(f"naca\n{m}{p}0{ss}\nsave\ntemp.dat\n\n")

    else:
        with open("instruction2.txt", "w") as file1:            
            file1.write(f"naca\n{m}{p}{ss}\nsave\ntemp.dat\n\n")

    # Run xFoil for profile generation
    subprocess.run([f"{current_path}\\Run2.bat"])

    # Write "tempfoil.dat" as output
    with open('temp.dat', 'r') as f_in:
        lines = f_in.readlines()[1:]  # Skip the first row

    with open('tempfoil.dat', 'w') as f_out:
        f_out.writelines(lines)
