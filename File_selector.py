import tkinter as tk
from tkinter import filedialog

# Function to open the file requester
def select_dat_file():
    root = tk.Tk()
    root.withdraw()
    file_path = filedialog.askopenfilename(filetypes=[("DAT files", "*.dat")])
    return file_path

# Function to modify the selected .dat file removing the first line (title) 
def copy_dat_file_without_first_row(input_file, output_file):
    with open(input_file, 'r') as f_in:
        lines = f_in.readlines()[1:]  # Skip the first row

    with open(output_file, 'w') as f_out:
        f_out.writelines(lines)

# Main function
def main():
    
    # Select the .dat file
    dat_file_path = select_dat_file()

    # Copy the .dat file content without the first row to tempfoil.dat
    if dat_file_path:
        copy_dat_file_without_first_row(dat_file_path, 'tempfoil.dat')
        print("Profile selected!")
    else:
        print("No file selected.")
