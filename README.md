A simple graphical interface for XFoil, with some extras!

Originally this graphical interface was developed in Python for a thesis project, the aim was to give students a simple and easy approach to XFoil.
Now I managed to convert it to a more fast and reliable version written in C++.\
This interface translate some of the basic xFoil funcions like NACA profile generation, repaneling and flap addition. It also adds some features like the direct comparison of -cp plots or the capability to manually morph the airfoil using a "b-spline like" interpolation to compute the shape and the control points.

For more info the thesis can be found here: https://www.politesi.polimi.it/handle/10589/227115

NOTES:

In order to run this software needs xfoil.exe in the same folder.\
Executable can be found here: https://web.mit.edu/drela/Public/web/xfoil/ 

The source files need to be compiled in order to have the executable running\
The software is written using Qt libraries that needs to be installed in order to correctly compile/run

FUNCTIONS:

Naca 4 digit airfoil generation\
Naca 5 digit airfoil generation\
CST parameterization airfoil generation\
IGP parameterization airfoil generation\
Import an airfoil from .dat file\
Airfoil re-paneling\
Manually modify airfoil shape\
Visual comparison between different airfoils\
Add a flap to the airfoil\
Solving the single point aerodynamic problem in viscous/inviscid and compressible/incompressible cases\
Visual comparison between different airfoils -cp distribution\
Visualizing polar plot, cl/alpha plot and cd/alpha plot

Thanks!
