import numpy as np
import matplotlib.pyplot as plt

# Function to generate an airfoil using CST parameterization
def main(N1,N2,A_up,A_lo):

    x = np.linspace(0, 1, 200)

    C_x = (x**N1) * ((1 - x)**N2)
    S_x_up = sum(A_up[i] * x**i for i in range(len(A_up)))
    S_x_lo = sum(A_lo[i] * x**i for i in range(len(A_lo)))

    y_up = C_x * S_x_up
    y_lo = C_x * S_x_lo

    xr = np.flip(x)
    yr = np.flip(y_up)
    y_lo = np.delete(y_lo, 0)
    x = np.delete(x, 0)
    Xp = np.concatenate([xr,x])
    Yp = np.concatenate([yr,y_lo])

    with open('tempfoil.dat', 'w') as file:
            for x, y in zip(Xp, Yp):
                file.write(f"{x} {y}\n")
