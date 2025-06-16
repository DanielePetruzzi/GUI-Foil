import numpy as np
import tkinter as tk
import sys
from tkinter import filedialog, Button
import matplotlib.pyplot as plt
from matplotlib.pyplot import figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from scipy.interpolate import splprep, splev

def main():

    # Function to read tempfoil.dat and extract the x, y coordinates
    def read_airfoil(filename="tempfoil.dat"):
        data = np.loadtxt(filename)
        x, y = data[:, 0], data[:, 1]
        return x, y

    # Function to save the new airfoil coordinates to tempfoil.dat
    def save_airfoil(filename, x_new, y_new):
        np.savetxt(filename, np.column_stack([x_new, y_new]), fmt='%.6f')

    # Function to extract control points from the airfoil curve (denser near the leading edge)
    def get_control_points(x, y, num_points=15):
        tck, u = splprep([x, y], s=0)
        u_new = np.array([0,0.05,0.1,0.2,0.3,0.35,0.4,0.45,0.475,0.5,0.525,0.55,0.6,0.65,0.7,0.8,0.9,0.95,1])
        x_new, y_new = splev(u_new, tck)
        return np.array(x_new), np.array(y_new)

    # Class to handle draggable points in the plot
    class DraggablePoint:
        def __init__(self, point, index, control_points, line, update_bspline_callback):
            self.point = point
            self.index = index
            self.control_points = control_points
            self.line = line
            self.update_bspline_callback = update_bspline_callback
            self.press = None

        def connect(self):
            self.cidpress = self.point.figure.canvas.mpl_connect('button_press_event', self.on_press)
            self.cidrelease = self.point.figure.canvas.mpl_connect('button_release_event', self.on_release)
            self.cidmotion = self.point.figure.canvas.mpl_connect('motion_notify_event', self.on_motion)

        def on_press(self, event):
            if event.inaxes != self.point.axes: return
            contains, _ = self.point.contains(event)
            if contains:
                self.press = (self.point.get_xdata(), self.point.get_ydata())
            
        def on_release(self, event):
            self.press = None
            self.point.figure.canvas.draw()

        def on_motion(self, event):
            if self.press is None: return
            if event.inaxes != self.point.axes: return
            self.control_points[self.index, 0] = event.xdata
            self.control_points[self.index, 1] = event.ydata
            self.point.set_data(event.xdata, event.ydata)
            self.update_bspline_callback()
            self.line.figure.canvas.draw()

    # Function to update the B-spline curve when control points are moved
    def update_bspline(ax, line, control_points):
        def update():
            tck, _ = splprep([control_points[:, 0], control_points[:, 1]], s=0)
            u_fine = np.linspace(0, 1, 200)
            x_fine, y_fine = splev(u_fine, tck)
            line.set_data(x_fine, y_fine)
            ax.figure.canvas.draw()
        return update

    # Function to create the Tkinter window and display the plot
    def b_spline_editor(x, y):
        root_morph = tk.Tk()
        root_morph.title("B-Spline Editor")

        # Plot creation
        fig, ax = plt.subplots()
        ax.set_title('Drag the control points to modify the airfoil shape')
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        plt.axis('equal')
        ax.plot(x, y, '--', label='Original Airfoil')
        figure(figsize=(15, 15), dpi=80)

        # Generate control points
        control_x, control_y = get_control_points(x, y)
        control_points = np.column_stack([control_x, control_y])

        # Plot the initial B-spline curve
        tck, _ = splprep([control_x, control_y], s=0)
        u_fine = np.linspace(0, 1, 200)
        x_fine, y_fine = splev(u_fine, tck)
        line, = ax.plot(x_fine, y_fine, 'b-', lw=2, label="B-Spline morphed airfoil")

        # Create a list of draggable points
        draggable_points = []
        for i in range(len(control_points)):
            point, = ax.plot(control_points[i, 0], control_points[i, 1], 'ro', label='Control Points')
            if i == 0:
                ax.legend()
            draggable = DraggablePoint(point, i, control_points, line, update_bspline(ax, line, control_points))
            draggable.connect()
            draggable_points.append(draggable)

        canvas = FigureCanvasTkAgg(fig, master=root_morph)
        canvas.get_tk_widget().pack()

        # Save button to write the new airfoil shape
        def save_new_airfoil():
            tck, _ = splprep([control_points[:, 0], control_points[:, 1]], s=0)
            u_fine = np.linspace(0, 1, 200)
            x_new, y_new = splev(u_fine, tck)
            save_airfoil("tempfoil.dat", x_new, y_new)
            ax.legend().remove()
            plt.close()
            root_morph.quit()
            root_morph.destroy()

        save_button = Button(root_morph, text="Save", command=save_new_airfoil)
        save_button.pack()
        
        root_morph.mainloop()

    x, y = read_airfoil()
    b_spline_editor(x, y)
