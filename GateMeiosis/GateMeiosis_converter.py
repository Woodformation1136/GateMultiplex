from tkinter import filedialog
from tkinter import *
import tkinter as tk
import tkinter.ttk as ttk
import math
import os
import csv
import re


def is_number(num):
    pattern = re.compile(r'^[-+]?[-0-9]\d*\.\d*|[-+]?\.?[0-9]\d*$')
    result = pattern.match(num)
    if result:
        return 1
    else:
        return 0

root = tk.Tk()
root.path_name = filedialog.askdirectory()
path_name = root.path_name
root.destroy()

# f = open(path_name + '/' +'List_for_Converter.csv', 'r')
f = open('List_for_Converter.csv', 'r')
allTitle_list = []
for row in csv.reader(f):
    for title in row:
        allTitle_list.append(title)
    break

    
    
window = tk.Tk()
window.title('Information for converter')
# window.geometry('400x200')
window.configure(background='white')

def Converter_setting():
    if is_number(row_entry.get()) and is_number(col_entry.get()) and int(float(row_entry.get())) == float(row_entry.get()) and int(float(col_entry.get())) == float(col_entry.get()) and int(row_entry.get()) > 0 and int(col_entry.get()) > 0:
        o = open('parametersConverter.txt', 'w')
        o.write('Path:' + path_name + '\n')
        o.write('Title of Signal column:' + signal_col_btn.get() + '\n')
        o.write('Coordinate of row in Signal file:' + str(int(row_entry.get())-1) + '\n')
        o.write('Coordinate of column in Signal file:' + str(int(col_entry.get())-1) + '\n')
        o.close()
        window.destroy()
        exe = 'Converter.exe'
        os.system(exe)
    else:
        warning_window = tk.Toplevel()
        warning_window.title('Warning')
        warning_label = tk.Label(warning_window, text='Row and column in Signal file should both be positive intergers.', relief = SUNKEN)
        warning_label.pack()
        def close_warning():
            warning_window.destroy()
        OK_btn = tk.Button(warning_window, text='OK', command=close_warning)
        OK_btn.pack(side = tk.BOTTOM)
        warning_window.mainloop
    
signal_col_label = tk.Label(window, text='Signal column in List_for_converter.csv', width = 40, relief = SUNKEN)
signal_col_label.grid(row=0, column=0, sticky='EW', pady = 3)
signal_col_btn = ttk.Combobox(window, value=allTitle_list, width = 40)
signal_col_btn.current(0)
signal_col_btn.grid(row=0, column=1, sticky='EW', pady = 3)

Coordinate_label = tk.Label(window, text='Coordinates of upper left hand corner in signal file', width = 40, relief = SUNKEN)
Coordinate_label.grid(row=1, column=0, columnspan=2, sticky='EW', pady = 3)

row_label = tk.Label(window, text='Row (start from 1)', width = 40, relief = SUNKEN)
row_label.grid(row=2, column=0, sticky='EW', pady = 3)
row_entry = tk.Entry(window)
row_entry.grid(row=2, column=1, sticky='EW', pady = 3)

col_label = tk.Label(window, text='Column (start from 1)', width = 40, relief = SUNKEN)
col_label.grid(row=3, column=0, sticky='EW', pady = 3)
col_entry = tk.Entry(window)
col_entry.grid(row=3, column=1, sticky='EW', pady = 3)


OK_btn = tk.Button(window, text='OK', command=Converter_setting, width = 40)
OK_btn.grid(row=4, column=0, columnspan =2, pady = 3)

f.close()
window.mainloop()