# GUI-Foil
A simple graphical interface for XFoil, with some extras!

This graphical interface was developed for a thesis project, the aim is to give students a simple and easy approach to xFoil. 
This interface translate some of the basic xFoil funcions like NACA profile generation, repaneling and flap addition. 
It also adds some features like the direct comparison of -cp plots or the capability to manually morph the airfoil using a "b-spline like" interpolation to compute the shape and the control points.

For more info the thesis can be found here:
https://www.politesi.polimi.it/handle/10589/227115

NOTE: In order to run this software needs xFoil.exe in the same folder. Executable can be found here: https://web.mit.edu/drela/Public/web/xfoil/
NOTE: In order to run this software needs Run.bat Run2.bat Run3.bat and Run4.bat in the same folder.

FUNCTIONS:
- Naca 4 digit airfoil generation
- Naca 5 digit airfoil generation
- CST parameterization airfoil generation
- Import an airfoil from .dat file
- Airfoil re-paneling
- Manually modify airfoil shape
- Visual comparison between different airfoils
- Add a flap to the airfoil
- Solving the aerodynamic problem in viscous/inviscid and compressible/incompressible cases
- Visual comparison between different airfoils -cp distribution

ADDITIONAL NOTE: This software can be compiled in a windows executable file (.exe) using PyInstaller. In this case also temporary files are needed in the folder.
