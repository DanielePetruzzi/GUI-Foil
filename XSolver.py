import numpy as np
import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import os
import tkinter as tk
import re
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from Files_path import current_path

# Function to call XFoil solver
def main():

    # Check and remove files from previous runs
    if os.path.exists("clxfoil.txt"):
        os.remove("clxfoil.txt")
    if os.path.exists("cpxfoil.txt"):
        os.remove("cpxfoil.txt")

    # Load stream condition data
    streamdata = np.loadtxt('tempstream.dat')

    AoA = streamdata[0]
    Ma = streamdata[1]
    Re = streamdata[2]

    # Construct XFoil instruction file for viscous/inviscid compressible/incompressible cases

    # inviscid-incompressible:
    if Ma == 0 and Re == 0:
        with open("instruction.txt", "w") as file1:            
            file1.write(f"load\ntempfoil.dat\ntempfoil\noper\niter\n500\nalfa\n{AoA}\ncpwr\ncpxfoil.txt\npacc\nclxfoil.txt\n\naseq\n{AoA}\n{AoA}\n1\n")

    # viscous-incompressible:
    elif Ma == 0 and Re != 0:
        with open("instruction.txt", "w") as file1:            
            file1.write(f"load\ntempfoil.dat\ntempfoil\noper\nvisc\n{Re}\niter\n500\nalfa\n{AoA}\ncpwr\ncpxfoil.txt\npacc\nclxfoil.txt\n\naseq\n{AoA}\n{AoA}\n1\n")

    # inviscid-compressible:
    elif Ma !=0 and Re == 0:
        with open("instruction.txt", "w") as file1:            
            file1.write(f"load\ntempfoil.dat\ntempfoil\noper\nmach\n{Ma}\niter\n500\nalfa\n{AoA}\ncpwr\ncpxfoil.txt\npacc\nclxfoil.txt\n\naseq\n{AoA}\n{AoA}\n1\n")

    # viscous-compressible:
    elif Ma !=0 and Re != 0:
        with open("instruction.txt", "w") as file1:            
            file1.write(f"load\ntempfoil.dat\ntempfoil\noper\nmach\n{Ma}\nvisc\n{Re}\niter\n500\nalfa\n{AoA}\ncpwr\ncpxfoil.txt\npacc\nclxfoil.txt\n\naseq\n{AoA}\n{AoA}\n1\n")

    # Run XFoil
    pattern = re.compile(r'\s+')
    command = f"{current_path}\\Run.bat"
    command = re.sub(pattern,' ',command)
    subprocess.run([command])


    # Write "clxfoil.txt" file
    with open(f"{current_path}\\clxfoil.txt", "r") as fp:
        lines = fp.readlines()
    with open(f"{current_path}\\clxfoil.txt", "w") as fp:
        for number, line in enumerate(lines):
            if number not in [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]:
                fp.write(line)

    # Write "cpxfoil.txt" file
    with open(f"{current_path}\\cpxfoil.txt", "r") as fp:
        lines = fp.readlines()
    with open(f"{current_path}\\cpxfoil.txt", "w") as fp:
        for number, line in enumerate(lines):
            if number not in [0, 1, 2]:
                fp.write(line)
