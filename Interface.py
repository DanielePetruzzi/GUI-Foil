import tkinter as tk
import os
import shutil
import webbrowser
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from tkinter import messagebox
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from Morph import main as main_morph
from XNaca_4_generator import main as main_XNaca_4
from XNaca_5_generator import main as main_XNaca_5
from CST import main as main_CST
from File_selector import main as main_fs
from Paneling import main as main_panels
from Flap import main as main_flap
from XSolver import main as main_solver

# BUTTON CLICK HANDLER FUNCTION
def button_click(button_num):

    # GENERATE AIRFOIL button
    if button_num == 1:

        if os.path.exists("tempfoil.dat"):
            shutil.copyfile('tempfoil.dat', 'tempfoilold.dat')
            os.remove("tempfoil.dat")

        # NACA 4 digits series
        def naca4():

            if os.path.exists("naca.dat"):
                os.remove("naca.dat")

            # Open a new window for parameters input
            input_window = tk.Toplevel(generator_window)
            input_window.title("NACA 4 digit")
            x_offset = root.winfo_x() + 3
            y_offset = root.winfo_y() + 200
            input_window.geometry("+{}+{}".format(x_offset, y_offset))
            
            # Create labels and entry fields for M, P, and SS
            tk.Label(input_window, text="M [Max camber (0-9)]:").grid(row=0, column=0)
            m_entry = tk.Entry(input_window)
            m_entry.grid(row=0, column=1)
            
            tk.Label(input_window, text="P [Max camber position (0-9)]:").grid(row=1, column=0)
            p_entry = tk.Entry(input_window)
            p_entry.grid(row=1, column=1)
            
            tk.Label(input_window, text="SS [Thickness (1-30)]:").grid(row=2, column=0)
            ss_entry = tk.Entry(input_window)
            ss_entry.grid(row=2, column=1)
            
            # Function to save airfoil to a .dat file and plot the airfoil on the GUI
            def save_profile():
                m = m_entry.get()
                p = p_entry.get()
                ss = ss_entry.get()
                with open("naca.dat", "w") as file:
                    file.write(f"{m}\n{p}\n{ss}")
                
                main_XNaca_4()

                input_window.destroy()
                generator_window.destroy()

                airfoildata = np.loadtxt('tempfoil.dat')
                xR = airfoildata[:,0]
                yR = airfoildata[:,1]

                if os.path.exists("tempfoilold.dat"):
                    airfoildataold = np.loadtxt('tempfoilold.dat')
                    xRold = airfoildataold[:,0]
                    yRold = airfoildataold[:,1]

                ax.clear()
                if os.path.exists("tempfoilold.dat"):
                    ax.plot(xRold, yRold,'--', color='#ff7f0e', label="Previous airfoil")
                ax.plot(xR, yR, linewidth=2, color='#1f77b4',label="Current airfoil")
                plt.axis('equal')
                ax.legend(loc='upper left')
                canvas.draw()

            # Button to save airfoil
            save_button = tk.Button(input_window, text="Generate", command=save_profile)
            save_button.grid(row=5, column=0, columnspan=2)
        
        # NACA 5 digits series
        def naca5():

            if os.path.exists("naca.dat"):
                os.remove("naca.dat")

            # Open a new window for parameters input
            input_window = tk.Toplevel(generator_window)
            input_window.title("NACA 5 digit")
            x_offset = root.winfo_x() + 3
            y_offset = root.winfo_y() + 200
            input_window.geometry("+{}+{}".format(x_offset, y_offset))
            
            # Create labels and entry fiellds for Cl, P, Q and XX
            tk.Label(input_window, text="L [Cl*3/2]").grid(row=0, column=0)
            l_entry = tk.Entry(input_window)
            l_entry.grid(row=0, column=1)
            
            tk.Label(input_window, text="P [Max camber position (1-5)]:").grid(row=1, column=0)
            p_entry = tk.Entry(input_window)
            p_entry.grid(row=1, column=1)
            
            tk.Label(input_window, text="Q [Simple=0 Reflex=1]:").grid(row=2, column=0)
            q_entry = tk.Entry(input_window)
            q_entry.grid(row=2, column=1)

            tk.Label(input_window, text="XX [Thickness (1-30)]:").grid(row=3, column=0)
            xx_entry = tk.Entry(input_window)
            xx_entry.grid(row=3, column=1)
            
            # Function to save airfoil to a .dat file and plot the airfoil on the GUI
            def save_profile():
                l = l_entry.get()
                p = p_entry.get()
                q = q_entry.get()
                xx = xx_entry.get()
                with open("naca.dat", "w") as file:
                    file.write(f"{l}\n{p}\n{q}\n{xx}")

                main_XNaca_5()

                input_window.destroy()
                generator_window.destroy()

                airfoildata = np.loadtxt('tempfoil.dat')
                xR = airfoildata[:,0]
                yR = airfoildata[:,1]

                if os.path.exists("tempfoilold.dat"):
                    airfoildataold = np.loadtxt('tempfoilold.dat')
                    xRold = airfoildataold[:,0]
                    yRold = airfoildataold[:,1]

                ax.clear()
                if os.path.exists("tempfoilold.dat"):
                    ax.plot(xRold, yRold,'--', color='#ff7f0e', label="Previous airfoil")
                ax.plot(xR, yR, linewidth=2, color='#1f77b4',label="Current airfoil")
                plt.axis('equal')
                ax.legend(loc='upper left')
                canvas.draw()

            # Button to save airfoil
            save_button = tk.Button(input_window, text="Generate", command=save_profile)
            save_button.grid(row=5, column=0, columnspan=2)

        # CST airfoil
        def cst():

            # Open a new window for parameters input
            input_window = tk.Toplevel(generator_window)
            input_window.title("CST")
            x_offset = root.winfo_x() + 3
            y_offset = root.winfo_y() + 200
            input_window.geometry("+{}+{}".format(x_offset, y_offset))
            
            # Create labels and entry fields for N0,N1,A_up,A_lo
            tk.Label(input_window, text="N1:").grid(row=0, column=0)
            N1_entry = tk.Entry(input_window)
            N1_entry.grid(row=0, column=1)
            
            tk.Label(input_window, text="N2:").grid(row=1, column=0)
            N2_entry = tk.Entry(input_window)
            N2_entry.grid(row=1, column=1)
            
            tk.Label(input_window, text="A1 (upper):").grid(row=2, column=0)
            A1_up_entry = tk.Entry(input_window)
            A1_up_entry.grid(row=2, column=1)

            tk.Label(input_window, text="A2 (upper):").grid(row=3, column=0)
            A2_up_entry = tk.Entry(input_window)
            A2_up_entry.grid(row=3, column=1)

            tk.Label(input_window, text="A3 (upper):").grid(row=4, column=0)
            A3_up_entry = tk.Entry(input_window)
            A3_up_entry.grid(row=4, column=1)
            
            tk.Label(input_window, text="A4 (upper):").grid(row=5, column=0)
            A4_up_entry = tk.Entry(input_window)
            A4_up_entry.grid(row=5, column=1)

            tk.Label(input_window, text="A1 (lower):").grid(row=2, column=2)
            A1_lo_entry = tk.Entry(input_window)
            A1_lo_entry.grid(row=2, column=3)

            tk.Label(input_window, text="A2 (lower):").grid(row=3, column=2)
            A2_lo_entry = tk.Entry(input_window)
            A2_lo_entry.grid(row=3, column=3)

            tk.Label(input_window, text="A3 (lower):").grid(row=4, column=2)
            A3_lo_entry = tk.Entry(input_window)
            A3_lo_entry.grid(row=4, column=3)
            
            tk.Label(input_window, text="A4 (lower):").grid(row=5, column=2)
            A4_lo_entry = tk.Entry(input_window)
            A4_lo_entry.grid(row=5, column=3)

            # Function to save airfoil to a .dat file and plot the airfoil on the GUI
            def save_profile():
                N1 = float(N1_entry.get())
                N2 = float(N2_entry.get())
                A1_up = float(A1_up_entry.get())
                A2_up = float(A2_up_entry.get())
                A3_up = float(A3_up_entry.get())
                A4_up = float(A4_up_entry.get())
                A1_lo = float(A1_lo_entry.get())
                A2_lo = float(A2_lo_entry.get())
                A3_lo = float(A3_lo_entry.get())
                A4_lo = float(A4_lo_entry.get())

                A_up = np.array([A1_up,A2_up,A3_up,A4_up])
                A_lo = np.array([A1_lo,A2_lo,A3_lo,A4_lo])

                main_CST(N1,N2,A_up,A_lo)

                input_window.destroy()
                generator_window.destroy()

                airfoildata = np.loadtxt('tempfoil.dat')
                xR = airfoildata[:,0]
                yR = airfoildata[:,1]

                if os.path.exists("tempfoilold.dat"):
                    airfoildataold = np.loadtxt('tempfoilold.dat')
                    xRold = airfoildataold[:,0]
                    yRold = airfoildataold[:,1]

                ax.clear()
                if os.path.exists("tempfoilold.dat"):
                    ax.plot(xRold, yRold,'--', color='#ff7f0e', label="Previous airfoil")
                ax.plot(xR, yR, linewidth=2, color='#1f77b4',label="Current airfoil")
                plt.axis('equal')
                ax.legend(loc='upper left')
                canvas.draw()

            # Button to save airfoil
            save_button = tk.Button(input_window, text="Generate", command=save_profile)
            save_button.grid(row=6, column=1, columnspan=2)

        generator_window = tk.Toplevel(root)
        generator_window.title("Choose airfoil type")
        x_offset = root.winfo_x() + 3
        y_offset = root.winfo_y() + 70
        generator_window.geometry("200x80+{}+{}".format(x_offset, y_offset))

        NACA4 = tk.Button(generator_window, text="NACA 4 digits", command=naca4)
        NACA4.pack()

        NACA5 = tk.Button(generator_window, text="NACA 5 digits", command=naca5)
        NACA5.pack()

        CST = tk.Button(generator_window, text="CST", command=cst)
        CST.pack()


    # LOAD AIRFOIL FROM FILE button
    elif button_num == 2:
        
        if os.path.exists("tempfoil.dat"):
            shutil.copyfile('tempfoil.dat', 'tempfoilold.dat')

        main_fs()
        
        # Plot the airfoil on the GUI
        airfoildata = np.loadtxt('tempfoil.dat')
        xR = airfoildata[:,0]
        yR = airfoildata[:,1]

        if os.path.exists("tempfoilold.dat"):
            airfoildataold = np.loadtxt('tempfoilold.dat')
            xRold = airfoildataold[:,0]
            yRold = airfoildataold[:,1]

        ax.clear()
        if os.path.exists("tempfoilold.dat"):
            ax.plot(xRold, yRold,'--', color='#ff7f0e', label="Previous airfoil")
        ax.plot(xR, yR, linewidth=2, color='#1f77b4', label="Current airfoil")
        plt.axis('equal')
        ax.legend(loc='upper left')
        canvas.draw()


    # AIRFOIL PANELING button
    elif button_num == 3:

        if os.path.exists("tempfoil.dat"):
            shutil.copyfile('tempfoil.dat', 'tempfoilold.dat')

        input_window = tk.Toplevel(root)
        input_window.title("Airfoil Paneling")
        x_offset = root.winfo_x() + 243
        y_offset = root.winfo_y() + 70
        input_window.geometry("+{}+{}".format(x_offset, y_offset))
        
        # Entry field for number of panels
        tk.Label(input_window, text="Number of panels:").grid(row=0, column=0)
        NP_entry = tk.Entry(input_window)
        NP_entry.grid(row=0, column=1)

        def save_NP():
            NP = NP_entry.get()
            
            main_panels(NP)

            input_window.destroy()

            # Plot the airfoil on the GUI
            airfoildata = np.loadtxt('tempfoil.dat')
            xR = airfoildata[:,0]
            yR = airfoildata[:,1]

            if os.path.exists("tempfoilold.dat"):
                airfoildataold = np.loadtxt('tempfoilold.dat')
                xRold = airfoildataold[:,0]
                yRold = airfoildataold[:,1]

            ax.clear()
            if os.path.exists("tempfoilold.dat"):
                ax.plot(xRold, yRold,'--', color='#ff7f0e', label="Previous airfoil")
            ax.plot(xR, yR, linewidth=2, color='#1f77b4', label="Current airfoil")
            plt.axis('equal')
            ax.legend(loc='upper left')
            canvas.draw()

        save_button = tk.Button(input_window, text="Set", command=save_NP)
        save_button.grid(row=1, column=0, columnspan=2)


    # MODIFY SHAPE button 
    elif button_num == 4:

        if os.path.exists("tempfoil.dat"):
            shutil.copyfile('tempfoil.dat', 'tempfoilold.dat')
        
        main_morph()

        # Plot the airfoil on the GUI
        airfoildata = np.loadtxt('tempfoil.dat')
        xR = airfoildata[:,0]
        yR = airfoildata[:,1]

        airfoildataold = np.loadtxt('tempfoilold.dat')
        xRold = airfoildataold[:,0]
        yRold = airfoildataold[:,1]
 
        ax.clear()
        if os.path.exists("tempfoilold.dat"):
            ax.plot(xRold, yRold,'--', color='#ff7f0e', label="Previous airfoil")
        ax.plot(xR, yR, linewidth=2, color='#1f77b4', label="Current airfoil")
        plt.axis('equal')
        ax.legend(loc='upper left')
        canvas.draw()


    # FLAP button
    elif button_num == 5:

        if os.path.exists("tempfoil.dat"):
            shutil.copyfile('tempfoil.dat', 'tempfoilold.dat')

        # Plot the airfoil on the GUI with grid
            airfoildata = np.loadtxt('tempfoil.dat')
            xR = airfoildata[:,0]
            yR = airfoildata[:,1]
            
            ax.clear()
            ax.plot(xR, yR, linewidth=2, color='#1f77b4', label="Current airfoil")
            plt.axis('equal')
            plt.grid(True, which='both', linestyle='--', linewidth=0.5)
            plt.minorticks_on()
            ax.legend(loc='upper left')
            canvas.draw()

        input_window = tk.Toplevel(root)
        input_window.title("Insert Flap")
        x_offset = root.winfo_x() + 450
        y_offset = root.winfo_y() + 70
        input_window.geometry("+{}+{}".format(x_offset, y_offset))

        tk.Label(input_window, text="NOTE: Hinge must be inside airfoil geometry!").grid(row=0, column=0, columnspan=2)
        # Entry field for hinge position and flap deflection
        tk.Label(input_window, text="Hinge X position:").grid(row=1, column=0)
        XH_entry = tk.Entry(input_window)
        XH_entry.grid(row=1, column=1)

        tk.Label(input_window, text="Hinge Y position:").grid(row=2, column=0)
        YH_entry = tk.Entry(input_window)
        YH_entry.grid(row=2, column=1)

        tk.Label(input_window, text="Flap deflection (degrees):").grid(row=3, column=0)
        HDelta_entry = tk.Entry(input_window)
        HDelta_entry.grid(row=3, column=1)

        def save_Flap():
            XH = XH_entry.get()
            YH = YH_entry.get()
            HDelta = HDelta_entry.get()
            
            main_flap(XH,YH,HDelta)

            input_window.destroy()

            # Plot the airfoil on the GUI
            airfoildata = np.loadtxt('tempfoil.dat')
            xR = airfoildata[:,0]
            yR = airfoildata[:,1]

            if os.path.exists("tempfoilold.dat"):
                airfoildataold = np.loadtxt('tempfoilold.dat')
                xRold = airfoildataold[:,0]
                yRold = airfoildataold[:,1]
            
            xh=float(XH)
            yh=float(YH)
            ax.clear()
            ax.plot(xh,yh, 'ro', label='Hinge')
            if os.path.exists("tempfoilold.dat"):
                ax.plot(xRold, yRold,'--', color='#ff7f0e', label="Previous airfoil")
            ax.plot(xR, yR, linewidth=2, color='#1f77b4', label="Current airfoil")
            plt.axis('equal')
            ax.legend(loc='upper left')
            canvas.draw()

        save_button = tk.Button(input_window, text="Set", command=save_Flap)
        save_button.grid(row=4, column=0, columnspan=2)


    # SET STREAM CONDITIONS button
    elif button_num == 6:

        # Open window for stream input
        input_window = tk.Toplevel(root)
        input_window.title("Stream conditions")
        x_offset = root.winfo_x() + 515
        y_offset = root.winfo_y() + 70
        input_window.geometry("+{}+{}".format(x_offset, y_offset))
        
        # Entry fields for AoA, U and Reynolds
        tk.Label(input_window, text="AoA (degrees):").grid(row=0, column=0)
        AoA_entry = tk.Entry(input_window)
        AoA_entry.grid(row=0, column=1)
        
        tk.Label(input_window, text="Mach (0 for incompressible):").grid(row=1, column=0)
        Ma_entry = tk.Entry(input_window)
        Ma_entry.grid(row=1, column=1)
        
        tk.Label(input_window, text="Reynolds (0 for inviscid):").grid(row=2, column=0)
        Re_entry = tk.Entry(input_window)
        Re_entry.grid(row=2, column=1)
      
        # Save stream to tempstream.dat file
        def save_stream():
            AoA = AoA_entry.get()
            Ma = Ma_entry.get()
            Re = Re_entry.get()
            with open("tempstream.dat", "w") as file:
                file.write(f"{AoA}\n{Ma}\n{Re}")
            input_window.destroy()

        def save_AoA():
            AoA = AoA_entry.get()
            with open("tempstream.dat", 'r') as file:
                lines = file.readlines()
            # Replace the AoA
            lines[0] = f"{AoA}\n"
            with open("tempstream.dat", 'w') as file:
                file.writelines(lines)
            input_window.destroy()
        
        # Button to save stream
        save_button = tk.Button(input_window, text="Set", command=save_stream)
        save_button.grid(row=5, column=0, columnspan=2)
        AoAsave_button = tk.Button(input_window, text="Update AoA only", command=save_AoA)
        AoAsave_button.grid(row=0, column=2)
    

    # XFOIL SOLVER button
    elif button_num == 7:

        # Error message (Airfoil)
        if not os.path.exists("tempfoil.dat"):
            messagebox.showinfo("Warning", "No Airfoil inserted!")
            return
        
        # Error messages (Stream)
        if not os.path.exists("tempstream.dat"):
            messagebox.showinfo("Warning", "No Stream condition set!")
            return
        
        with open("tempstream.dat", 'r') as file:
            lines = file.readlines()
            check=len(lines)
        if check < 3:
            messagebox.showinfo("Warning", "Incomplete Stream condition set!")
            return
        
        main_solver()

        # Import the obtained results and plot on the GUI
        clresult = np.loadtxt('clxfoil.txt')
        cpresult = np.loadtxt('cpxfoil.txt')

        Cl = clresult[1]
        Cd = clresult[2]
        Cm = clresult[4]
        Xtrup = clresult[5]
        Xtrlo = clresult[6]

        L=float(Cl)
        if L > -0.002 and L < 0.002:
            L = 0
        M=float(Cm)

        streamdata = np.loadtxt('tempstream.dat')

        AoA = streamdata[0]
        Ma = streamdata[1]
        Re = streamdata[2]

        if os.path.exists("clxfoilold.txt"):
            clresultold = np.loadtxt('clxfoilold.txt')
            cpresultold = np.loadtxt('cpxfoilold.txt')

            Clold = clresultold[1]
            Cmold = clresultold[4]
            Lold=float(Clold)
            if Lold > -0.002 and Lold < 0.002:
                Lold = 0
            Mold=float(Cmold)
     
        ax.clear()
        if os.path.exists("clxfoilold.txt"):
            ax.plot(cpresultold[:,0],cpresultold[:,1],"--",linewidth=0.8, color='#1f77b4', label="Previous airfoil")
            ax.plot(cpresultold[:,0],-cpresultold[:,2],"--",linewidth=1, color='#ff7f0e', label="Previous -Cp distribution")
            if Lold != 0 and Lold != -0:
                XCPold = -(Mold/Lold)
                if XCPold>0 and XCPold<1:
                    arr = mpatches.FancyArrowPatch((XCPold, 0), (XCPold, Lold), arrowstyle='->,head_width=.15', linestyle=':', mutation_scale=20)
                    ax.add_patch(arr)

        ax.plot(cpresult[:,0],cpresult[:,1],linewidth=1.5, color='#1f77b4', label="Current airfoil")
        ax.plot(cpresult[:,0],-cpresult[:,2],linewidth=2, color='#ff7f0e', label="Current -Cp distribution")
    
        ax.text(1.1,0.4,'AoA=%s'%(AoA))
        ax.text(1.1,0.3,'Ma=%s'%(Ma))
        ax.text(1.1,0.2,'Re=%s'%(Re))
        ax.text(1.1,0.1,'Cl=%s'%(Cl))
        ax.text(1.1,0,'Cd=%s'%(Cd))
        ax.text(1.1,-0.1,'Cm=%s'%(Cm))
        if Xtrup != 0 and Xtrlo != 0:
            ax.text(1.1,-0.2,'Top Xtr=%s'%(Xtrup))
            ax.text(1.1,-0.3,'Bot Xtr=%s'%(Xtrlo))

        if L !=0 and L != -0:
            XCP = -(M/L)
            if XCP>0 and XCP<1:
                arr = mpatches.FancyArrowPatch((XCP, 0), (XCP, L), arrowstyle='->,head_width=.15', mutation_scale=20)
                ax.add_patch(arr)
                if L > 0:
                    ax.annotate("Lift", (0, 1.05), xycoords=arr, ha='left', va='bottom')
                elif L<0:
                    ax.annotate("Lift", (0, -0.05), xycoords=arr, ha='left', va='bottom')

        plt.axis('equal')
        plt.ylabel('- Cp')
        plt.xlabel('x')
        ax.legend(loc='upper left')
        canvas.draw()

        shutil.copyfile('clxfoil.txt', 'clxfoilold.txt')
        shutil.copyfile('cpxfoil.txt', 'cpxfoilold.txt')
    

    # INFO button
    elif button_num == 8:

        def callback(url):
            webbrowser.open_new(url)

        info_window = tk.Toplevel(root)
        info_window.title("Info")
        x_offset = root.winfo_x() + 520
        y_offset = root.winfo_y() + 70
        info_window.geometry("+{}+{}".format(x_offset, y_offset))
        
        # Display information text
        label1 = tk.Label(info_window, text="GUI-Foil © 2024 by Daniele Petruzzi")
        label1.pack()
        label2 = tk.Label(info_window, text="is licensed under CC BY-SA 4.0")
        label2.pack()
        label3 = tk.Label(info_window, text="")
        label3.pack()
        label4 = tk.Label(info_window, text="To view a copy of this license visit:")
        label4.pack()
        link1 = tk.Label(info_window, text="https://creativecommons.org/licenses/by-sa/4.0/", fg="blue", cursor="hand2")
        link1.pack()
        link1.bind("<Button-1>", lambda e: callback("https://creativecommons.org/licenses/by-sa/4.0/"))
        label5 = tk.Label(info_window, text="")
        label5.pack()
        label6 = tk.Label(info_window, text="This software is intended to work and interact with")
        label6.pack()
        label7 = tk.Label(info_window, text="XFoil by Mark Drela and Harold Youngren")
        label7.pack()
        label8 = tk.Label(info_window, text="")
        label8.pack()
        label9 = tk.Label(info_window, text="For any additional information:")
        label9.pack()
        link2 = tk.Label(info_window, text="https://linkedin.com/in/daniele-petruzzi", fg="blue", cursor="hand2")
        link2.pack()
        link2.bind("<Button-1>", lambda e: callback("https://linkedin.com/in/daniele-petruzzi-74425a292"))


        #linkedin.com/in/daniele-petruzzi-74425a292
         
# MAIN INTERFACE

# Create main window
root = tk.Tk()
root.title("GUI-Foil")
root.geometry("1000x800")

# Remove old temporary files
if os.path.exists("temp.dat"):
    os.remove("temp.dat")

if os.path.exists("tempfoil.dat"):
    os.remove("tempfoil.dat")

if os.path.exists("tempfoilold.dat"):
    os.remove("tempfoilold.dat")

if os.path.exists("clxfoil.txt"):
    os.remove("clxfoil.txt")

if os.path.exists("cpxfoil.txt"):
    os.remove("cpxfoil.txt")

if os.path.exists("clxfoilold.txt"):
    os.remove("clxfoilold.txt")

if os.path.exists("cpxfoilold.txt"):
    os.remove("cpxfoilold.txt")

if os.path.exists("tempstream.dat"):
    os.remove("tempstream.dat")

if os.path.exists("instruction.txt"):
    os.remove("instruction.txt")

if os.path.exists("instruction2.txt"):
    os.remove("instruction2.txt")

if os.path.exists("instruction3.txt"):
    os.remove("instruction3.txt")

if os.path.exists("instruction4.txt"):
    os.remove("instruction4.txt")

# Top row buttons (run by button handler function)
button1 = tk.Button(root, text="Generate airfoil", command=lambda: button_click(1))
button2 = tk.Button(root, text="Import airfoil from .dat file", command=lambda: button_click(2))
button3 = tk.Button(root, text="Airfoil paneling", command=lambda: button_click(3))
button4 = tk.Button(root, text="Modify airfoil shape", command=lambda: button_click(4))
button5 = tk.Button(root, text="Add a flap", command=lambda: button_click(5))
button6 = tk.Button(root, text="Set stream conditions", command=lambda: button_click(6))
button7 = tk.Button(root, text="Calculate", command=lambda: button_click(7))
button8 = tk.Button(root, text="Info", command=lambda: button_click(8))

# Plot space
fig, ax = plt.subplots()
canvas = FigureCanvasTkAgg(fig, master=root)
canvas.get_tk_widget().pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True)

# Top buttons allignment
button1.pack(side=tk.LEFT)
button2.pack(side=tk.LEFT)
button3.pack(side=tk.LEFT)
button4.pack(side=tk.LEFT)
button5.pack(side=tk.LEFT)
button6.pack(side=tk.LEFT)
button7.pack(side=tk.LEFT)
button8.pack(side=tk.RIGHT)

root.protocol("WM_DELETE_WINDOW", lambda: (root.quit(), root.destroy()))

# Start the GUI
root.mainloop()
