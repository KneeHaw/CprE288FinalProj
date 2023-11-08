import tkinter as tk
from tkinter import ttk, Tk
import matplotlib
matplotlib.use('TkAgg')
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt


class Application(ttk.Frame):
    
    def __init__(self, master=None):
        tk.Frame.__init__(self, master)
        self.master = master
        # self.pack()
        self.settings_info = ("W: Move Forward\nS: Move Backward\nA: Turn Left\nD: Turn Right\nQ: 360 Scan & Gen Preds\nG: Accept Generated Preds\nE: 180 Scan for Manual Drive\nO: Perform Servo Cal\nX: Clear OI")
        self.createWidgets(master, self.settings_info)

    def createWidgets(self, master, sett_info):
        self.winfo_toplevel().title("Cybot Socket UI - Ethan Rogers")

        self.entry = tk.LabelFrame(master, text='Entry Data', pady=4, padx=4, labelanchor='nw')
        self.entry.pack(side='top', expand=True, fill='both')

        self.quitButton = tk.Button(self.entry, text='Quit', command=self.quit)
        self.quitButton.config(bg="gray")
        self.quitButton.pack(side='top', fill='x')

        self.entrybox = tk.Entry(self.entry, text='Sample Application', borderwidth=2, relief='solid')
        self.entrybox.config(bg="white")
        self.entrybox.pack(side='left', expand=True, fill='both')

        self.keylog = tk.Entry(self.entry, text='Previously Pressed Keys', borderwidth=2, relief='solid')
        self.keylog.config(bg="white")
        self.keylog.pack(side='left', expand=True, fill='both')

        self.setts = tk.Label(self.entry, text=sett_info, borderwidth=2, relief='solid', padx=4, justify='left', font=('Arial', 20))
        self.setts.config(bg="white")
        self.setts.pack(side='left', fill='y')


        self.recv = tk.LabelFrame(master, text="Received Data", pady=4, padx=4, labelanchor='nw')
        self.recv.pack(side='top', expand=True, fill='both')

        self.logbox = tk.Text(self.recv, state='disabled', borderwidth=2, relief='solid')
        self.logbox.config(bg="#ffffff")
        self.logbox.pack(side='left', fill='both')

        fig = plt.figure(1)
        self.canvas = FigureCanvasTkAgg(fig, master=self.recv)
        self.plot_widget = self.canvas.get_tk_widget()
        self.plot_widget.pack(side='right', expand=True, fill='both')
        fig.canvas.draw()


if __name__ == '__main__':
    root = Tk()
    root.geometry("1920x1080")
    app = Application(root)
    app.mainloop()
