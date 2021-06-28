from tkinter import filedialog
from tkinter import *
import tkinter as tk
import tkinter.ttk as ttk
import math
import os
import csv


# def is_number(num):
# 	pattern = re.compile(r'^[-+]?[-0-9]\d*\.\d*|[-+]?\.?[0-9]\d*$')
# 	result = pattern.match(num)
# 	if result:
# 		return 1
# 	else:
# 		return 0
def is_number(num):
	try:
	    float(num)
	    return 1
	except Exception:
		return 0

root = tk.Tk()
root.path_name = filedialog.askdirectory()
path_name = root.path_name
root.destroy()

listFileName = os.listdir(path_name)
setTitle = set([])
for fileName in listFileName:
	f = open(path_name + '/' + fileName, 'r')
	for row in f:
		setTitle.add(row)
		break
	f.close()

# if os.listdir(path_name)[0] != 'ExpDesign.txt':
#	 f = open(path_name + '/' + os.listdir(path_name)[0], 'r')
# else:
#	 f = open(path_name + '/' + os.listdir(path_name)[1], 'r')
f = open(path_name + '/' + listFileName[0], 'r')

allTitle_list = []
allTitle_column_list = []
available_column_list = []
for row in csv.reader(f):
	i=0
	for title in row:
		allTitle_list.append(title)
		allTitle_column_list.append(i)
		available_column_list.append(i)
		i = i + 1
	break

f.close()

#allTitle_wFile_list = ['Filename']
#allTitle_wFile_list.extend(allTitle_list)

single_selection_list = ['']
available_single_selection_list = ['']
#single_selection_list.extend(allTitle_list)
indexCount = 0
for title in allTitle_list:
	single_selection_list.append(str(indexCount) + ', ' + title)
	available_single_selection_list.append(str(indexCount) + ', ' + title)
	indexCount = indexCount + 1

bottom_window = tk.Tk()
bottom_window.title('GateMultiplex_advanced_version (Total ' + str(len(listFileName)) + ' input files)')
# window.geometry('1024x800')
bottom_window.configure(background='white')
window = tk.Frame(bottom_window)

if_no_Toplevel= True

def on_closing(extra_window):
	global if_no_Toplevel
	if_no_Toplevel = True
	extra_window.destroy()

if len(setTitle) >= 2:
	warningTitleInconsistent = tk.Frame(bottom_window)
	warningTitleInconsistent.pack()
	bottom_window.title('GateMultiplex_warning')
	def mainWindowDestroy():
		warningTitleInconsistent.destroy()
		bottom_window.destroy()
	def keepGoing():
		warningTitleInconsistent.destroy()
		window.pack()
	input_file_information_label = tk.Label(warningTitleInconsistent, text='Inconsistent titles in input files!', width = 40, relief = SUNKEN)
	input_file_information_label.grid(row=0, column=0, columnspan = 2, sticky='EW')
	close_btn = tk.Button(warningTitleInconsistent, text='Close', command=mainWindowDestroy)
	close_btn.grid(row=1, column=0, padx=2, pady=2)
	keepgo_btn = tk.Button(warningTitleInconsistent, text='Keep going', command=keepGoing)
	keepgo_btn.grid(row=1, column=1, padx=2, pady=2)
	warningTitleInconsistent.mainloop
else:
	window.pack()

input_file_information_frame = tk.Frame(window)
input_file_information_frame.grid(row=0, column=0, sticky='NS')
input_file_information_label = tk.Label(input_file_information_frame, text='Input file information', width = 40, relief = SUNKEN)
input_file_information_label.grid(row=0,column=0 , sticky='EW')

cutoff_setting_frame = tk.Frame(window)
cutoff_setting_frame.grid(row=0, column=1, sticky='NS')
cutoff_setting_label = tk.Label(cutoff_setting_frame, text='Cutoff setting', width = 40, relief = SUNKEN)
cutoff_setting_label.grid(row=0,column=0 , sticky='EW')

output_file_selection_frame = tk.Frame(window)
output_file_selection_frame.grid(row=0, column=2, sticky='NS')
output_file_selection_label = tk.Label(output_file_selection_frame, text='Output file selection', width = 40, relief = SUNKEN)
output_file_selection_label.grid(row=0, column=0, sticky='EW')


frame_entry_list = []
HL_entry_list = []
# frame_text_list = ['Technical replicate cutoff\n(Default = 1)' , \
#					 'Biological replicate cutoff\n(Default = 1)'
#					 ]

signal_list = []
plateorder_list = []
plateorder_available_list = []
plateorder_option_list = ['']
# parameters_list = []
wellorder_list = []
orderlist = []
parametercutoff_list = []
reference_label_list = []
fixed_reference_list = []
fixed_reference_HL_list = []
reference_percentile = []
background_value_list = []
background_HL_list = []
if_signal_set = 0 
if_parameter_set = 0
foldChangeValue = ['']
foldChangeHL = ['']
topPercentage = ['']
lowerPercentage = ['']
bio_col = [-1]
bio_cutoff = ['']
tech_cutoff = ['']
nameIC = ['']
titleIC = ['']
cutoffIC =['']
cutoffHLIC = ['']

def Signal_setting():
	global if_no_Toplevel
	if if_no_Toplevel:
		extra_window = tk.Toplevel()
		extra_window.protocol("WM_DELETE_WINDOW", lambda arg=extra_window: on_closing(arg))
		extra_window.title('Signal column setting')
		extra_window.geometry('350x300')
		extra_window.wm_attributes('-topmost',1)
		if_no_Toplevel = False
		Signal_option_list = []
		Signal_option_list.extend(signal_list)
		Signal_option_list.extend(available_column_list)
		Signal_option_list.sort()
		check_list = []
		def Signal_setting_selection():
			global if_no_Toplevel
			signal_list.clear()
			indexcount = 0
			for checkTitle in check_list:
				if(checkTitle.get() == 1):
					signal_list.append(Signal_option_list[indexcount])
				indexcount = indexcount + 1
			extra_window.destroy()
			if_no_Toplevel = True
			bufferStr = 'Signal column:'
			available_column_list.clear()
			available_column_list.extend(Signal_option_list)
			for column in signal_list:
				bufferStr = bufferStr + '\n' + allTitle_list[column]
				available_column_list.remove(column)
			signal_col_label.config(text=bufferStr)
		def myfunction(event): 
			canvas.configure(scrollregion=canvas.bbox("all"),width=350,height=260)
		list_frame=tk.Frame(extra_window)
		list_frame.pack()
		canvas=tk.Canvas(list_frame)
		frame=tk.Frame(canvas)
		xscrollbar=Scrollbar(list_frame,orient="horizontal",command=canvas.xview)
		myscrollbar=Scrollbar(list_frame,orient="vertical",command=canvas.yview)
		canvas.configure(xscrollcommand=xscrollbar.set, yscrollcommand=myscrollbar.set)
		xscrollbar.pack(side="bottom",fill="x")
		myscrollbar.pack(side="right",fill="y")
		canvas.pack(side="left")
		canvas.create_window((0,0),window=frame,anchor='nw')
		frame.bind("<Configure>",myfunction)
		for title in Signal_option_list:
			check_list.append(tk.IntVar())
			chkExample = tk.Checkbutton(frame, text=allTitle_list[title], variable=check_list[-1], onvalue=1, offvalue=0)
			chkExample.pack(anchor='w')
		close_btn = tk.Button(extra_window, text='OK', command=Signal_setting_selection)
		close_btn.pack()
		extra_window.mainloop

def PlateOrder_setting():
	global if_no_Toplevel
	if if_no_Toplevel:
		extra_window = tk.Toplevel()
		extra_window.title('Treatment column setting')
		extra_window.geometry('350x300')
		extra_window.protocol("WM_DELETE_WINDOW", lambda arg=extra_window: on_closing(arg))
		extra_window.wm_attributes('-topmost',1)
		if_no_Toplevel = False
		PlateOrder_option_list = []
		PlateOrder_option_list.extend(plateorder_list)
		PlateOrder_option_list.extend(available_column_list)
		if bio_col[0] != -1:
			PlateOrder_option_list.extend(bio_col)
		PlateOrder_option_list.sort()
		check_list = []
		def PlateOrder_setting_selection():
			global if_no_Toplevel
			plateorder_list.clear()
			indexcount = 0
			for checkTitle in check_list:
				if(checkTitle.get() == 1):
					plateorder_list.append(PlateOrder_option_list[indexcount])
				indexcount = indexcount + 1
			extra_window.destroy()
			parametercutoff_list.clear()
			plateorder_available_list.clear()
			plateorder_available_list.append('')
			bufferStr = 'Treatment column:'
			available_column_list.clear()
			available_column_list.extend(PlateOrder_option_list)
			for column in plateorder_list:
				bufferStr = bufferStr + '\n' + allTitle_list[column]
				available_column_list.remove(column)
				plateorder_available_list.append(str(column) + ', ' + allTitle_list[column])
			plateorder_col_label.config(text=bufferStr)
			bio_window = tk.Toplevel()
			bio_window.protocol("WM_DELETE_WINDOW", lambda arg=bio_window: on_closing(arg))
			bio_window.wm_attributes('-topmost',1)
			bio_window.title('Bio-replicate Treatment setting')
			bio_window.geometry('350x300')
			def myfunction(event): 
				canvas.configure(scrollregion=canvas.bbox("all"),width=350,height=260)
			list_frame=tk.Frame(bio_window)
			list_frame.pack()
			canvas=tk.Canvas(list_frame)
			frame=tk.Frame(canvas)
			xscrollbar=Scrollbar(list_frame,orient="horizontal",command=canvas.xview)
			myscrollbar=Scrollbar(list_frame,orient="vertical",command=canvas.yview)
			canvas.configure(xscrollcommand=xscrollbar.set, yscrollcommand=myscrollbar.set)
			xscrollbar.pack(side="bottom",fill="x")
			myscrollbar.pack(side="right",fill="y")
			canvas.pack(side="left")
			canvas.create_window((0,0),window=frame,anchor='nw')
			frame.bind("<Configure>",myfunction)
			bio_mode = IntVar()
			bio_mode.set(-1)
			Radiobutton(frame, text="Without bio-replicate treament", variable=bio_mode, value=-1).pack(anchor='w')
			for column in available_column_list:
				Radiobutton(frame, text=allTitle_list[column], variable=bio_mode, value=column).pack(anchor='w')
			def Bio_setting():
				global if_no_Toplevel
				bio_col.clear()
				bio_col.append(bio_mode.get())
				if bio_mode.get() != -1:
					available_column_list.remove(bio_mode.get())
					bio_col_label.config(text='Bio-replicate Treatment: '+ allTitle_list[int(bio_col[0])])
				bio_window.destroy()
				if_no_Toplevel = True
	#		 parameter_col_label.config(text=(str(parameters_list)+'\n'+str(parametercutoff_list)))
			OK_btn = tk.Button(bio_window, text='OK', command=Bio_setting)
			OK_btn.pack()
			bio_window.mainloop
		def myfunction(event): 
			canvas.configure(scrollregion=canvas.bbox("all"),width=350,height=260)
		list_frame=tk.Frame(extra_window)
		list_frame.pack()
		canvas=tk.Canvas(list_frame)
		frame=tk.Frame(canvas)
		xscrollbar=Scrollbar(list_frame,orient="horizontal",command=canvas.xview)
		myscrollbar=Scrollbar(list_frame,orient="vertical",command=canvas.yview)
		canvas.configure(xscrollcommand=xscrollbar.set, yscrollcommand=myscrollbar.set)
		xscrollbar.pack(side="bottom",fill="x")
		myscrollbar.pack(side="right",fill="y")
		canvas.pack(side="left")
		canvas.create_window((0,0),window=frame,anchor='nw')
		frame.bind("<Configure>",myfunction)
		for title in PlateOrder_option_list:
			check_list.append(tk.IntVar())  
			chkExample = tk.Checkbutton(frame, text=allTitle_list[title], variable=check_list[-1], onvalue=1, offvalue=0)
			chkExample.pack(anchor='w')
		close_btn = tk.Button(extra_window, text='Next', command=PlateOrder_setting_selection)
		close_btn.pack()
		extra_window.mainloop

def WellOrder_setting():
	global if_no_Toplevel
	if if_no_Toplevel:
		extra_window = tk.Toplevel()
		extra_window.protocol("WM_DELETE_WINDOW", lambda arg=extra_window: on_closing(arg))
		extra_window.title('SampleName column')
		extra_window.geometry('350x300')
		extra_window.wm_attributes('-topmost',1)
		if_no_Toplevel = False
		WellOrder_option_list = []
		WellOrder_option_list.extend(wellorder_list)
		WellOrder_option_list.extend(available_column_list)
		WellOrder_option_list.sort()
		check_list = []
		def WellOrder_setting_selection():
			global if_no_Toplevel
			wellorder_list.clear()
			reference_label_list.clear()
			indexcount = 0
			for checkTitle in check_list:
				if(checkTitle.get() == 1):
					wellorder_list.append(WellOrder_option_list[indexcount])
				indexcount = indexcount + 1
			extra_window.destroy()
			if_no_Toplevel = True
			bufferStr = 'SampleName column:'
			available_column_list.clear()
			available_column_list.extend(WellOrder_option_list)
			for column in wellorder_list:
				bufferStr = bufferStr + '\n' + allTitle_list[column]
				available_column_list.remove(column)
			wellorder_col_label.config(text=bufferStr)
		def myfunction(event): 
			canvas.configure(scrollregion=canvas.bbox("all"),width=350,height=260)
		list_frame=tk.Frame(extra_window)
		list_frame.pack()
		canvas=tk.Canvas(list_frame)
		frame=tk.Frame(canvas)
		xscrollbar=Scrollbar(list_frame,orient="horizontal",command=canvas.xview)
		myscrollbar=Scrollbar(list_frame,orient="vertical",command=canvas.yview)
		canvas.configure(xscrollcommand=xscrollbar.set, yscrollcommand=myscrollbar.set)
		xscrollbar.pack(side="bottom",fill="x")
		myscrollbar.pack(side="right",fill="y")
		canvas.pack(side="left")
		canvas.create_window((0,0),window=frame,anchor='nw')
		frame.bind("<Configure>",myfunction)
		for title in WellOrder_option_list:
			check_list.append(tk.IntVar())  
			chkExample = tk.Checkbutton(frame, text=allTitle_list[title], variable=check_list[-1], onvalue=1, offvalue=0)
			chkExample.pack(anchor='w')
		close_btn = tk.Button(extra_window, text='OK', command=WellOrder_setting_selection)
		close_btn.pack()
		extra_window.mainloop

def Reference_reset():
	reference_label_list.clear()
	reference_percentile.clear()
	fixed_reference_list.clear()
	fixed_reference_HL_list.clear()
	reference_settings_label.config(text='')
	fixed_reference_label.config(text='')
	reference_mode_label.config(text='')

def Reference_setting():
	global if_no_Toplevel
	if if_no_Toplevel:
		extra_window = tk.Toplevel()
		extra_window.protocol("WM_DELETE_WINDOW", lambda arg=extra_window: on_closing(arg))
		extra_window.wm_attributes('-topmost',1)
		if_no_Toplevel = False
		if reference_mode.get() == 'A':
			extra_window.title('Manual reference setting')
			labelentry_list=[]
			def Reference_label_setting_selection():
				global if_no_Toplevel
				Reference_reset()
				warning_message = ''
				warning_switch = 1
				if len(labelentry_list) == 0:
					warning_switch = 0
				for label in labelentry_list:
					if label.get() != '':
						warning_switch = 0
					reference_label_list.append(label.get())
				if is_number(str((buffer_entry.get()))) == 0 and buffer_entry.get() != '':
					warning_message = warning_message + 'Fold change value should be a numeric value.\n\n'
				if (is_number(top_entry.get()) == 0 and top_entry.get() != '') or (is_number(bottom_entry.get()) == 0 and bottom_entry.get() != ''):
					warning_message = warning_message + 'Percentages of bottom and top bound should both be numeric values.\n\n'
				elif is_number(top_entry.get()) == 1 and is_number(bottom_entry.get()) == 1:
					if float(top_entry.get()) > 100 or float(top_entry.get()) < 0 or float(bottom_entry.get()) > 100 or float(bottom_entry.get()) < 0:
						warning_message = warning_message + 'Percentages of bottom and top bound should both be between 0 and 100.\n\n'
					elif float(top_entry.get()) < float(bottom_entry.get()):
						warning_message = warning_message + 'Percentage of bottom bound cannot be higher than percentage of top bound.\n\n'
				if warning_switch == 1 or warning_message != '':
					if warning_switch == 1:
						warning_message = 'Please fill at least one SampleName condition.\n\n' + warning_message
					warning_window = tk.Toplevel()
					warning_window.title('Warning')
					warning_label = tk.Label(warning_window, text=warning_message, relief = SUNKEN)
					warning_window.wm_attributes('-topmost',1)
					warning_label.pack()
					def close_warning():
						warning_window.destroy()
					OK_btn = tk.Button(warning_window, text='OK', command=close_warning)
					OK_btn.pack(side = tk.BOTTOM)
					warning_window.mainloop
				else:
					foldChangeValue.clear()
					if buffer_entry.get() == '':
						foldChangeValue.append('1')
					else:
						foldChangeValue.append(str(buffer_entry.get()))
					foldChangeHL.clear()
					foldChangeHL.append(str(HL_btn.get()))
					topPercentage.clear()
					if top_entry.get() == '':
						topPercentage.append('100')
					else:
						topPercentage.append(str(top_entry.get()))
					lowerPercentage.clear()
					if bottom_entry.get() == '':
						lowerPercentage.append('0')
					else:
						lowerPercentage.append(str(bottom_entry.get()))
					reference_settings_string = 'Reference group:'
					for label in reference_label_list:
						reference_settings_string = reference_settings_string + '\n' + str(label)
					reference_settings_label.config(text=reference_settings_string)
					reference_settings_string = 'Fold change value: ' + foldChangeValue[0] + \
												'\nReference cutoff higher/lower: ' + foldChangeHL[0] + \
												'\nPercentages of bottom and top bound: ' + lowerPercentage[0] + ' - ' + topPercentage[0]
					reference_mode_label.config(text=reference_settings_string)
					extra_window.destroy()
					if_no_Toplevel = True
			def Destroy():
				global if_no_Toplevel
				extra_window.destroy()
				if_no_Toplevel = True
			if len(wellorder_list) == 0:
				nosignal_label = tk.Label(extra_window, text='Please set SampleName column first.', relief = SUNKEN)
				nosignal_label.pack()
				OK_btn = tk.Button(extra_window, text='OK', command=Destroy)
				OK_btn.pack()
			else:
				def myfunction(event):
					canvas.configure(scrollregion=canvas.bbox("all"),width=200,height=100)
				list_frame=tk.Frame(extra_window)
				list_frame.pack()
				canvas=tk.Canvas(list_frame)
				frame=tk.Frame(canvas) 
				xscrollbar=Scrollbar(list_frame,orient="horizontal",command=canvas.xview)
				myscrollbar=Scrollbar(list_frame,orient="vertical",command=canvas.yview)
				canvas.configure(xscrollcommand=xscrollbar.set, yscrollcommand=myscrollbar.set)
				xscrollbar.pack(side="bottom",fill="x")
				myscrollbar.pack(side="right",fill="y")
				canvas.pack(side="left")
				canvas.create_window((0,0),window=frame,anchor='nw')
				frame.bind("<Configure>",myfunction)
				for i in range(0, len(wellorder_list)):
					extra_buffer_label = tk.Label(frame, text = allTitle_list[wellorder_list[i]], relief = SUNKEN)
					extra_buffer_label.grid(row=i, column=0, sticky='EW')
					extra_buffer_entry = tk.Entry(frame)
					extra_buffer_entry.grid(row=i, column=1, sticky='EW')
					labelentry_list.append(extra_buffer_entry)
				FC_frame = tk.Frame(extra_window)
				FC_frame.pack()
				buffer_label = tk.Label(FC_frame, text = 'Fold change value\n(Default = 1)', width = 20, relief = SUNKEN)
				buffer_label.grid(row=1, column=0, sticky='E')
				buffer_entry = tk.Entry(FC_frame)
				buffer_entry.grid(row=1, column=1, sticky='EW')
				HL_Label = tk.Label(FC_frame, text='Higher/Lower\n(Default = Higher)', width = 20, relief = SUNKEN)
				HL_Label.grid(row=2, column=0, sticky='E')
				HL_btn = ttk.Combobox(FC_frame, value = ['Higher', 'Higher or equal', 'Lower', 'Lower or equal'])
				HL_btn.current(0)
				HL_btn.grid(row=2, column=1, sticky='W')
				top_label = tk.Label(FC_frame, text = 'Percentage of\ntop bound\n(default = 100)', width = 20, relief = SUNKEN)
				top_label.grid(row=3, column=0, sticky='E')
				top_entry = tk.Entry(FC_frame)
				top_entry.grid(row=3, column=1, sticky='EW')
				bottom_label = tk.Label(FC_frame, text = 'Percentage of\nbottom bound\n(default = 0)', width = 20, relief = SUNKEN)
				bottom_label.grid(row=4, column=0, sticky='E')
				bottom_entry = tk.Entry(FC_frame)
				bottom_entry.grid(row=4, column=1, sticky='EW')
				OK_btn = tk.Button(extra_window, text='OK', command=Reference_label_setting_selection)
				OK_btn.pack()
		
		elif reference_mode.get() == 'B':
			extra_window.title('Percentile reference setting')
			def Reference_percentile_setting_selection():
				global if_no_Toplevel
				Reference_reset()
				warning_message = ''
				percentile_reference = extra_buffer_entry.get()
				FCvalue_reference = FCvalue_entry.get()
				if percentile_reference == '':
					percentile_reference = '50'
				if FCvalue_reference == '':
					FCvalue_reference = '1'
				# if extra_buffer_entry.get() == '' and FCvalue_entry.get() == '':
				# 	foldChangeHL.clear()
				# 	foldChangeHL.append(str(HL_btn.get()))
				# 	foldChangeValue.append('1')
				# 	reference_percentile.append('50')
				# 	reference_mode_label.config(text='Percentile of reference group is '+ str(reference_percentile[0]) + '%\n' + 'Fold change value is 1\n' + foldChangeHL[0])
				# 	extra_window.destroy()
				# 	if_no_Toplevel = True
				# 	return
				if is_number(percentile_reference) == 0 or float(percentile_reference) > 100 or float(percentile_reference) < 0:
					warning_message = warning_message + 'Reference percentile should be a numeric value between 0 and 100.\n\n'
				if is_number(FCvalue_reference) == 0:
					warning_message = warning_message + 'Fold change value should be a numeric value.\n\n'
				if warning_message != '':
					warning_window = tk.Toplevel()
					warning_window.title('Warning')
					warning_label = tk.Label(warning_window, text=warning_message, relief = SUNKEN)
					warning_window.wm_attributes('-topmost',1)
					warning_label.pack()
					def close_warning():
						warning_window.destroy()
					OK_btn = tk.Button(warning_window, text='OK', command=close_warning)
					OK_btn.pack(side = tk.BOTTOM)
					warning_window.mainloop
				else:
					foldChangeValue.clear()
					foldChangeValue.append(str(FCvalue_reference))
					foldChangeHL.clear()
					foldChangeHL.append(str(HL_btn.get()))
					reference_percentile.append(percentile_reference)
					reference_mode_label.config(text='Percentile of reference group is '+ str(reference_percentile[0]) + '%\nFold change value: ' + foldChangeValue[0] + '\n' + foldChangeHL[0])
					extra_window.destroy()
					if_no_Toplevel = True
			extra_buffer_frame = tk.Frame(extra_window)
			extra_buffer_frame.pack()
			extra_buffer_label = tk.Label(extra_buffer_frame, text = 'Percentile (Default = 50)', relief = SUNKEN)
			extra_buffer_label.grid(row=0, column=0, sticky='EW')
			extra_buffer_entry = tk.Entry(extra_buffer_frame)
			extra_buffer_entry.grid(row=0, column=1, sticky='EW')
			FCvalue_label = tk.Label(extra_buffer_frame, text = 'Fold change value (Default = 1)', relief = SUNKEN)
			FCvalue_label.grid(row=1, column=0, sticky='EW')
			FCvalue_entry = tk.Entry(extra_buffer_frame)
			FCvalue_entry.grid(row=1, column=1, sticky='EW')
			HL_Label = tk.Label(extra_buffer_frame, text='Higher/Lower\n(Default = Higher)', width = 20, relief = SUNKEN)
			HL_Label.grid(row=2, column=0, sticky='E')
			HL_btn = ttk.Combobox(extra_buffer_frame, value = ['Higher', 'Higher or equal', 'Lower', 'Lower or equal'])
			HL_btn.current(0)
			HL_btn.grid(row=2, column=1, sticky='W')
			OK_btn = tk.Button(extra_window, text='OK', command=Reference_percentile_setting_selection)
			OK_btn.pack()
		
		elif reference_mode.get() == 'C':
			extra_window.title('Fixed reference value setting')
			labelentry_list=[]
			HLentry_list=[]
			def Fixed_reference_setting_selection():
				global if_no_Toplevel
				Reference_reset()
				warning_switch = 0
				for label in labelentry_list:
					if is_number(label.get()) == 0:
						warning_switch = 1
						break
					fixed_reference_list.append(str(label.get()))
					warning_switch = 0
				for HL in HLentry_list:
					fixed_reference_HL_list.append(str(HL.get()))
				if warning_switch == 1:
					warning_window = tk.Toplevel()
					warning_window.title('Warning')
					warning_window.wm_attributes('-topmost',1)
					warning_label = tk.Label(warning_window, text='Fixed reference values should be numeric values.', relief = SUNKEN)
					warning_label.pack()
					def close_warning():
						warning_window.destroy()
					OK_btn = tk.Button(warning_window, text='OK', command=close_warning)
					OK_btn.pack(side = tk.BOTTOM)
					warning_window.mainloop
				elif warning_switch == 0:
					str_label = 'Fixed reference value:'
					index_count = 0
					for text_frame in signal_list:
						str_label = str_label + '\n' + str(fixed_reference_list[index_count]) + ', ' + str(fixed_reference_HL_list[index_count])
						index_count = index_count + 1
					fixed_reference_label.config(text=str_label)
					extra_window.destroy()
					if_no_Toplevel = True
			if len(signal_list) == 0:
				nosignal_label = tk.Label(extra_window, text='Please set Signal column first.', relief = SUNKEN)
				nosignal_label.pack()
			def myfunction(event):
				canvas.configure(scrollregion=canvas.bbox("all"),width=350,height=300)
			list_frame=tk.Frame(extra_window)
			list_frame.pack()
			canvas=tk.Canvas(list_frame)
			frame=tk.Frame(canvas) 
			xscrollbar=Scrollbar(list_frame,orient="horizontal",command=canvas.xview)
			myscrollbar=Scrollbar(list_frame,orient="vertical",command=canvas.yview)
			canvas.configure(xscrollcommand=xscrollbar.set, yscrollcommand=myscrollbar.set)
			xscrollbar.pack(side="bottom",fill="x")
			myscrollbar.pack(side="right",fill="y")
			canvas.pack(side="left")
			canvas.create_window((0,0),window=frame,anchor='nw')
			frame.bind("<Configure>",myfunction)
			for i in range(0,len(signal_list)):
				buffer_frame = tk.Frame(frame)
				buffer_frame.grid(row=i, column=0, sticky='EW', pady = 3)
				buffer_label = tk.Label(buffer_frame, text = allTitle_list[signal_list[i]], width = 40, relief = SUNKEN)
				buffer_label.grid(row=0, column=0, columnspan = 2, sticky='EW')
				buffer_label = tk.Label(buffer_frame, text = 'Value', width = 20, relief = SUNKEN)
				buffer_label.grid(row=1, column=0, sticky='E')
				buffer_entry = tk.Entry(buffer_frame)
				buffer_entry.grid(row=1, column=1, sticky='EW')
				labelentry_list.append(buffer_entry)
				HL_Label = tk.Label(buffer_frame, text='Higher/Lower\n(Default = Higher)', width = 20, relief = SUNKEN)
				HL_Label.grid(row=2, column=0, sticky='E')
				HL_btn = ttk.Combobox(buffer_frame, value = ['Higher', 'Higher or equal', 'Lower', 'Lower or equal'])
				HL_btn.current(0)
				HL_btn.grid(row=2, column=1, sticky='W')
				HLentry_list.append(HL_btn)
			OK_btn = tk.Button(extra_window, text='OK', command=Fixed_reference_setting_selection)
			OK_btn.pack()
		extra_window.mainloop


def Parametercutoff_setting():
	global if_no_Toplevel
	if if_no_Toplevel:
		extra_window = tk.Toplevel()
		extra_window.title('Positive cutoff setting')
		extra_window.protocol("WM_DELETE_WINDOW", lambda arg=extra_window: on_closing(arg))
		extra_window.wm_attributes('-topmost',1)
		if_no_Toplevel = False
		Parametercutoff_option_list = []
		for index_column in plateorder_list:
			Parametercutoff_option_list.append(str(index_column))
		if len(signal_list) > 1:
			Parametercutoff_option_list.append('Signal_type')
		check_list = []
		bufferOrderlist =[]
		orderlabellist = []
		pcentrylist = []
		pHELElist = []
		buffergetlist = []
		previousgetlist = []
		def myfunction(event):
			canvas.configure(scrollregion=canvas.bbox("all"),width=400,height=100)
		list_frame=tk.Frame(extra_window)
		list_frame.pack()
		canvas=tk.Canvas(list_frame)
		frame=tk.Frame(canvas) 
		xscrollbar=Scrollbar(list_frame,orient="horizontal",command=canvas.xview)
		myscrollbar=Scrollbar(list_frame,orient="vertical",command=canvas.yview)
		canvas.configure(xscrollcommand=xscrollbar.set, yscrollcommand=myscrollbar.set)
		xscrollbar.pack(side="bottom",fill="x")
		myscrollbar.pack(side="right",fill="y")
		canvas.pack(side="left")
		canvas.create_window((0,0),window=frame,anchor='nw')
		frame.bind("<Configure>",myfunction)
		# check_frame = tk.Frame(frame)
		# check_frame.grid(row=0, column=0)
		# order_frame = tk.Frame(frame)
		# order_frame.grid(row=0, column=1)
		# entry_frame = tk.Frame(frame)
		# entry_frame.grid(row=0, column=2)
		def settingOrder():
			buffergetlist = []
			for check in check_list:
				buffergetlist.append(check.get())
			diffGet = list(set(buffergetlist).symmetric_difference(set(previousgetlist)) - set(['']))[0]
			if diffGet not in bufferOrderlist:
				bufferOrderlist.append(diffGet)
				pcentrylist[Parametercutoff_option_list.index(diffGet)].config(bg = 'White')
			else:
				bufferOrderlist.pop(bufferOrderlist.index(diffGet))
				pcentrylist[Parametercutoff_option_list.index(diffGet)].config(bg = 'Gray')
			previousgetlist.clear()
			previousgetlist.extend(buffergetlist)
			for label in orderlabellist:
				label.config(text='')
			for i in range(0,len(bufferOrderlist)):
				orderlabellist[Parametercutoff_option_list.index(bufferOrderlist[i])].config(text=str(i+1))
		def Parametercutoff_setting_filled():
			global if_no_Toplevel
			parametercutoff_list.clear()
			for title in bufferOrderlist:
				parametercutoff_list.append(pcentrylist[Parametercutoff_option_list.index(title)].get())
			warning_switch = 0
			for parametercutoff in parametercutoff_list:
				if is_number(parametercutoff) == 0:
					warning_switch = 1
					break
				elif (int(float(parametercutoff)) - float(parametercutoff)) != 0 or float(parametercutoff) <= 0:
					warning_switch = 1
					break
				warning_switch = 0
			if warning_switch == 1:
				warning_window = tk.Toplevel()
				warning_window.title('Warning')
				warning_label = tk.Label(warning_window, text='Positive cutoffs should be positive integers.', relief = SUNKEN)
				warning_window.wm_attributes('-topmost',1)
				warning_label.pack()
				def close_warning():
					warning_window.destroy()
				OK_btn = tk.Button(warning_window, text='OK', command=close_warning)
				OK_btn.pack(side = tk.BOTTOM)
				warning_window.mainloop
			elif warning_switch == 0:
				extra_window.destroy()
				if_no_Toplevel = True
	#			 parametercutoff_window.destroy()
				# text_parameter_title_label=''
				# text_parameter_cutoff_label=''
				text_PC = ''
				orderlist.clear()
				orderlist.extend(bufferOrderlist)
				for i in range(0, len(orderlist)):
					if orderlist[i] != 'Signal_type':
						text_PC = text_PC + str(i+1) + '. ' + allTitle_list[int(orderlist[i])] + ': '+ parametercutoff_list[i] + '\n'
						# text_parameter_title_label = text_parameter_title_label + allTitle_list[int(orderlist[i])] + ', '
						# text_parameter_cutoff_label = text_parameter_cutoff_label + parametercutoff_list[i] + ', '
					else:
						text_PC = text_PC + str(i+1) + '. Signal_type: '+ parametercutoff_list[i] + '\n'
						# text_parameter_title_label = text_parameter_title_label + 'Signal_type' + ', '
						# text_parameter_cutoff_label = text_parameter_cutoff_label + parametercutoff_list[i] + ', '
				# parameter_col_label.config(text=text_parameter_title_label[0:-2] + '\n' + text_parameter_cutoff_label[0:-2])
				parameter_col_label.config(text=text_PC[0:-1])
		row_count=0
		for title in Parametercutoff_option_list:
			if title != 'Signal_type':
				check_list.append(tk.StringVar())
				chkExample = tk.Checkbutton(frame, text=allTitle_list[int(title)], variable=check_list[-1], onvalue=title, offvalue='', command=settingOrder)
				chkExample.grid(row=row_count, column=0, sticky = 'W')
				chkOrder = tk.Label(frame, relief = SUNKEN)
				orderlabellist.append(chkOrder)
				chkOrder.grid(row=row_count, column=1, sticky = 'W')
				pc_entry = tk.Entry(frame, bg = 'Gray')
				pcentrylist.append(pc_entry)
				pc_entry.grid(row=row_count, column=2, sticky = 'W')
				# chkExample.pack(side = tk.BOTTOM)
				# chkOrder.pack(side = tk.BOTTOM)
				# pc_entry.pack(side = tk.BOTTOM)
			else:
				check_list.append(tk.StringVar())
				chkExample = tk.Checkbutton(frame, text='Signal_type', variable=check_list[-1], onvalue='Signal_type', offvalue='', command=settingOrder)
				chkExample.grid(row=row_count, column=0, sticky = 'W')
				chkOrder = tk.Label(frame, relief = SUNKEN)
				orderlabellist.append(chkOrder)
				chkOrder.grid(row=row_count, column=1, sticky = 'W')
				pc_entry = tk.Entry(frame, bg = 'Gray')
				pcentrylist.append(pc_entry)
				pc_entry.grid(row=row_count, column=2, sticky = 'W')
				# chkExample.pack(side = tk.BOTTOM)
				# chkOrder.pack(side = tk.BOTTOM)
				# pc_entry.pack(side = tk.BOTTOM)
			row_count += 1
		close_btn = tk.Button(extra_window, text='OK', command=Parametercutoff_setting_filled)
		close_btn.pack()
		extra_window.mainloop

def Parameters_setting():
	global if_no_Toplevel
	if if_no_Toplevel:
		warning_list = ''
		frame_text_count = 0
		if len(wellorder_list) == 0:
			warning_list = warning_list + 'Please set SampleName column.\n\n'
		if len(signal_list) == 0:
			warning_list = warning_list + 'Please set Signal column.\n\n'
		if len(reference_label_list) == 0 and len(fixed_reference_list) == 0 and len(reference_percentile) == 0:
			warning_list = warning_list + 'Please set Reference cutoff.\n\n'
		if	(nameIC[0] != '' and cutoffIC[0] == '') or (nameIC[0] == '' and cutoffIC[0] != ''):
			warning_list = warning_list + 'Please set both Internal control Treatment and cutoff.\nOr both of them should be empty.\n\n'
	#	 for i in range(0,2):
	#			 if frame_entry_list[i].get() != '':
	#				 if is_number(frame_entry_list[i].get()) == 0:
	# #					 if i == 0:
	# #						 warning_list = warning_list + frame_text_list[frame_text_count].split('\n')[0] + ' should be a numerical value.\n\n'
	# #					 else:
	#					 warning_list = warning_list + frame_text_list[frame_text_count].split('\n')[0] + ' should be a non-negative integer.\n\n'
	#				 elif is_number(frame_entry_list[i].get()) == 1 and (i != 0) and ((int(float(frame_entry_list[i].get())) - float(frame_entry_list[i].get())) != 0 or float(frame_entry_list[i].get()) < 0):
	#					 warning_list = warning_list + frame_text_list[frame_text_count].split('\n')[0] + ' should be a non-negative integer.\n\n'
	#			 frame_text_count = frame_text_count + 1
		
	#	 if name_internal_entry.get() != '':
	#		 if internal_col_btn.get() == '':
	#			 warning_list = warning_list + 'Please choose Title including internal control matrix.\n\n'
	#		 if frame_entry_list[6].get() != '' and is_number(frame_entry_list[6].get()) == 0:
	#			 warning_list = warning_list + 'Internal control cutoff should be a numerical value.\n\n'
	#	 elif internal_col_btn.get() != '':
	#		 if name_internal_entry.get() == '':
	#			 warning_list = warning_list + 'Please fill Name of internal control group.\n\n'
	#		 if frame_entry_list[6].get() != '' and is_number(frame_entry_list[6].get()) == 0:
	#			 warning_list = warning_list + 'Internal control cutoff should be a numerical value.\n\n'
	#	 elif frame_entry_list[6].get() != '':
	#		 if internal_col_btn.get() == '':
	#			 warning_list = warning_list + 'Please choose Title including internal control matrix.\n\n'
	#		 if name_internal_entry.get() == '':
	#			 warning_list = warning_list + 'Please fill Name of internal control group.\n\n'
	#		 if is_number(frame_entry_list[6].get()) == 0:
	#			 warning_list = warning_list + 'Internal control cutoff should be a numerical value.\n\n'

		if warning_list != '':
			warning_window = tk.Toplevel()
			warning_window.title('Warning')
			warning_window.protocol("WM_DELETE_WINDOW", lambda arg=warning_window: on_closing(arg))
			warning_window.wm_attributes('-topmost',1)
			if_no_Toplevel = False
			warning_label = tk.Label(warning_window, text=warning_list, relief = SUNKEN)
			warning_label.pack()
			def close_warning():
				global if_no_Toplevel
				warning_window.destroy()
				if_no_Toplevel = True
			OK_btn = tk.Button(warning_window, text='OK', command=close_warning)
			OK_btn.pack(side = tk.BOTTOM)
			warning_window.mainloop
		else:
	#		 o = open(path_name + '/ExpDesign.txt', 'w')
			o = open('ExpDesign.txt', 'w')
			o.write('Version:Advanced\n')
			o.write('Path:' + path_name + '\n')
			o.write('Internal control cut-off:' + str(cutoffIC[0]) + '\n')
			o.write('Internal control cut-off Higher or Lower:' + str(cutoffHLIC[0]) + '\n')
	#		 for i in range(0, len(frame_entry_list)):
	# #			 if i == 0:
	# #				 o.write('Internal control cut-off:' + str(frame_entry_list[i].get()) + '\n')
	# #				 o.write('Internal control cut-off Higher or Lower:' + str(HL_entry_list[i].get()) + '\n')
	#			 if i == 0:
	#				 o.write('Biological replicate cut-off (Default = 1):' + str(frame_entry_list[i].get()) + '\n')
	#			 elif i == 1:
	#				 o.write('Technical replicate cut-off (Default = 1):' + str(frame_entry_list[i].get()) + '\n')
			o.write('Biological replicate cut-off (Default = 1):' + str(bio_cutoff[0]) + '\n')
			o.write('Technical replicate cut-off (Default = 1):' + str(tech_cutoff[0]) + '\n')
			o.write('Background noise cut-off:')
			if len(background_value_list) >= 1:
				for i in range(0, len(background_value_list)):
					o.write(str(background_value_list[i]))
					o.write(',')
			o.write('\nBackground noise cut-off HL:')
			if len(background_HL_list) >= 1:
				for i in range(0, len(background_HL_list)):
					o.write(str(background_HL_list[i]))
					o.write(',')
			o.write('\nFold change cut-off (Default = 1):')
			if len(foldChangeValue) >= 1:
				o.write(foldChangeValue[0])
			o.write('\nFold change cut-off (Default = 1) Higher or Lower:')
			if len(foldChangeHL) >= 1:
				o.write(foldChangeHL[0])
			o.write('\nTop percentile:')
			if len(topPercentage) >= 1:
				o.write(topPercentage[0])
			o.write('\nBottom percentile:')
			if len(lowerPercentage) >= 1:
				o.write(lowerPercentage[0])
			o.write('\nSignal column:')
			if len(signal_list) >= 1:
				for i in range(0, len(signal_list)-1):
					o.write(str(signal_list[i]))
					o.write(',')
				o.write(str(signal_list[-1]))
			o.write('\nTagName column:')
			if len(plateorder_list) >= 1:
				for i in range(0, len(plateorder_list)-1):
					o.write(str(plateorder_list[i]))
					o.write(',')
				o.write(str(plateorder_list[-1]))
			o.write('\nSampleName column:')
			if len(wellorder_list) >= 1:
				for i in range(0, len(wellorder_list)-1):
					o.write(str(wellorder_list[i]))
					o.write(',')
				o.write(str(wellorder_list[-1]))
			o.write('\nParameters (Optional):')
			if len(orderlist) >= 1:
				for i in range(0, len(orderlist)-1):
					if orderlist[i] != 'Signal_type':
						o.write(str(orderlist[i]))
						o.write(',')
					else:
						o.write('Signal_type')
						o.write(',')
				if orderlist[-1] != 'Signal_type':
					o.write(str(orderlist[-1]))
				else:
					o.write('Signal_type')
			o.write('\nParameters title:')
			if len(orderlist) >= 1:
				for i in range(0, len(orderlist)-1):
					if orderlist[i] != 'Signal_type':
						o.write(allTitle_list[int(orderlist[i])])
						o.write(',')
					else:
						o.write('Signal_type')
						o.write(',')
				if orderlist[-1] != 'Signal_type':
					o.write(allTitle_list[int(orderlist[-1])])
				else:
					o.write('Signal_type')
			o.write('\nParameter cutoff (Optional):')
			if len(parametercutoff_list) >= 1:
				for i in range(0, len(parametercutoff_list)-1):
					o.write(str(parametercutoff_list[i]))
					o.write(',')
				o.write(str(parametercutoff_list[-1]))
	#		 o.write('\nReference group chosen mode:A')
			o.write('\nReference group chosen mode:' + str(reference_mode.get()))
			o.write('\nLabels of reference group:')
			for i in range(0, len(reference_label_list)):
				o.write(str(reference_label_list[i]))
				o.write(',')
			o.write('\nPercentile of reference group:')
			if len(reference_percentile) >= 1:
				o.write(reference_percentile[0])
			o.write('\nFixed reference:')
			if len(fixed_reference_list) >= 1:
				for i in range(0, len(fixed_reference_list)-1):
					o.write(str(fixed_reference_list[i]))
					o.write(',')
				o.write(str(fixed_reference_list[-1]))
			o.write('\nFixed reference higher or lower:')
			if len(fixed_reference_HL_list) >= 1:
				for i in range(0, len(fixed_reference_HL_list)-1):
					o.write(str(fixed_reference_HL_list[i]))
					o.write(',')
				o.write(str(fixed_reference_HL_list[-1]))
			
			o.write('\nBio-replicate column:')
			if  len(bio_col)>0:
				if bio_col[0] != -1:
					o.write(str(bio_col[0]))
	#		 o.write((biorep_col_btn.get()).split(', ')[0])
	#		 o.write('\nParameter of row in result files:')
	#		 o.write((parameter_row_btn.get()).split(', ')[0])
			o.write('\nName of internal control group (Optional):')
			o.write(nameIC[0])
			o.write('\nTitle including internal control group:')
			o.write(titleIC[0])
			o.write('\nParameter of row in result files:;;')
			o.write('\nOutput result files (Default = On):')
			o.write(resultfiles_btn.get())
			o.write('\nOutput fold change files (Default = Off):')
			o.write(FCfiles_btn.get())
			o.write('\nOutput PNE files (Default = Off):')
			o.write(NBPfiles_btn.get())
			o.write('\nSignal_type:')
			if len(signal_list) >= 1:
				for i in range(0, len(signal_list)-1):
					o.write(allTitle_list[int(signal_list[i])])
					o.write(',')
				o.write(allTitle_list[int(signal_list[-1])])
			o.close()
			exe = 'GateMultiplex_forWindows.exe'
			bottom_window.destroy()
			os.system(exe)


# row_count=4
# for text_frame in frame_text_list:
#	 buffer_frame = tk.Frame(cutoff_setting_frame)
#	 buffer_frame.grid(row=row_count, column=0, sticky='EW', pady = 3)
#	 buffer_label = tk.Label(buffer_frame, text = text_frame, width = 40, relief = SUNKEN)
#	 buffer_label.grid(row=0, column=0, columnspan = 2, sticky='EW')
#	 buffer_label = tk.Label(buffer_frame, text = 'Value', width = 20, relief = SUNKEN)
#	 buffer_label.grid(row=1, column=0, sticky='E')
#	 buffer_entry = tk.Entry(buffer_frame)
#	 buffer_entry.grid(row=1, column=1, sticky='EW')
#	 frame_entry_list.append(buffer_entry)
	
#	 if row_count == 0:
#		 HL_Label = tk.Label(buffer_frame, text='Higher/Lower\n(Default = Higher)', width = 20, relief = SUNKEN)
#		 HL_Label.grid(row=2, column=0, sticky='E')
#		 HL_btn = ttk.Combobox(buffer_frame, value = ['Higher', 'Higher or equal', 'Lower', 'Lower or equal'])
#		 HL_btn.current(0)
#		 HL_btn.grid(row=2, column=1, sticky='W')
#		 HL_entry_list.append(HL_btn)
	
#	 row_count = row_count + 1
calculate_btn = tk.Button(window, text='GO!', command=Parameters_setting, width = 10, height = 30)
calculate_btn.grid(row=0, column=3, sticky='N', padx = 2)

def Bio_cutoff_setting():
	global if_no_Toplevel
	if if_no_Toplevel:
		extra_window = tk.Toplevel()
		extra_window.title('Bio-replicate cutoff setting')
		extra_window.protocol("WM_DELETE_WINDOW", lambda arg=extra_window: on_closing(arg))
		extra_window.wm_attributes('-topmost',1)
		if_no_Toplevel = False
		buffer_frame = tk.Frame(extra_window)
		buffer_frame.pack()
		buffer_label = tk.Label(buffer_frame, text = 'Value', width = 20, relief = SUNKEN)
		buffer_label.grid(row=0, column=0, sticky='E')
		buffer_entry = tk.Entry(buffer_frame)
		buffer_entry.grid(row=0, column=1, sticky='EW')
		def settings():
			global if_no_Toplevel
			warning_switch = 0
			if is_number(buffer_entry.get()) == 0:
				warning_switch = 1
			elif (int(float(buffer_entry.get())) - float(buffer_entry.get())) != 0 or float(buffer_entry.get()) <= 0:
				warning_switch = 1
			if	buffer_entry.get() == '':
				warning_switch = 0
			if warning_switch == 1:
				warning_window = tk.Toplevel()
				warning_window.title('Warning')
				warning_window.wm_attributes('-topmost',1)
				warning_label = tk.Label(warning_window, text='Bio-replicate cutoff should be a positive integer.', relief = SUNKEN)
				warning_label.pack()
				def close_warning():
					warning_window.destroy()
				OK_btn = tk.Button(warning_window, text='OK', command=close_warning)
				OK_btn.pack(side = tk.BOTTOM)
				warning_window.mainloop
			else:
				bio_cutoff.clear()
				bio_cutoff.append(str(buffer_entry.get()))
				bio_cutoff_label.config(text='Bio-replicate cutoff: '+ bio_cutoff[0])
				extra_window.destroy()
				if_no_Toplevel = True
		close_btn = tk.Button(extra_window, text='OK', command=settings)
		close_btn.pack()
		extra_window.mainloop
bio_cutoff_frame = tk.Frame(cutoff_setting_frame, width = 40)
bio_cutoff_frame.grid(row=4, column=0, sticky='EW')
bio_cutoff_btn = tk.Button(bio_cutoff_frame, text='Bio-replicate cutoff\n(default = 1)', command=Bio_cutoff_setting, width = 40)
bio_cutoff_btn.grid(row=0, column=0, sticky='EW')
bio_cutoff_label = tk.Label(bio_cutoff_frame, relief = SUNKEN, justify = LEFT)
bio_cutoff_label.grid(row=1, column=0, sticky='W')

def Tech_cutoff_setting():
	global if_no_Toplevel
	if if_no_Toplevel:
		extra_window = tk.Toplevel()
		extra_window.title('Tech-replicate cutoff setting')
		extra_window.protocol("WM_DELETE_WINDOW", lambda arg=extra_window: on_closing(arg))
		extra_window.wm_attributes('-topmost',1)
		if_no_Toplevel = False
		buffer_frame = tk.Frame(extra_window)
		buffer_frame.pack()
		buffer_label = tk.Label(buffer_frame, text = 'Value', width = 20, relief = SUNKEN)
		buffer_label.grid(row=0, column=0, sticky='E')
		buffer_entry = tk.Entry(buffer_frame)
		buffer_entry.grid(row=0, column=1, sticky='EW')
		def settings():
			global if_no_Toplevel
			warning_switch = 0
			if is_number(buffer_entry.get()) == 0:
				warning_switch = 1
			elif (int(float(buffer_entry.get())) - float(buffer_entry.get())) != 0 or float(buffer_entry.get()) <= 0:
				warning_switch = 1
			if	buffer_entry.get() == '':
				warning_switch = 0
			if warning_switch == 1:
				warning_window = tk.Toplevel()
				warning_window.title('Warning')
				warning_window.wm_attributes('-topmost',1)
				warning_label = tk.Label(warning_window, text='Tech-replicate cutoff should be a positive integer.', relief = SUNKEN)
				warning_label.pack()
				def close_warning():
					warning_window.destroy()
				OK_btn = tk.Button(warning_window, text='OK', command=close_warning)
				OK_btn.pack(side = tk.BOTTOM)
				warning_window.mainloop
			else:
				tech_cutoff.clear()
				tech_cutoff.append(str(buffer_entry.get()))
				tech_cutoff_label.config(text='Tech-replicate cutoff: '+ tech_cutoff[0])
				extra_window.destroy()
				if_no_Toplevel = True
		close_btn = tk.Button(extra_window, text='OK', command=settings)
		close_btn.pack()
		extra_window.mainloop
tech_cutoff_frame = tk.Frame(cutoff_setting_frame, width = 40)
tech_cutoff_frame.grid(row=5, column=0, sticky='EW')
tech_cutoff_btn = tk.Button(tech_cutoff_frame, text='Tech-replicate cutoff\n(default = 1)', command=Tech_cutoff_setting, width = 40)
tech_cutoff_btn.grid(row=0, column=0, sticky='EW')
tech_cutoff_label = tk.Label(tech_cutoff_frame, relief = SUNKEN, justify = LEFT)
tech_cutoff_label.grid(row=1, column=0, sticky='W')

def myfunctionA(event):
	canvasA.configure(scrollregion=canvasA.bbox("all"),width=300,height=80)

wellorder_col_btn = tk.Button(input_file_information_frame, text='SampleName column\n(Required)', command=WellOrder_setting, width = 40)
wellorder_col_btn.grid(row=1, column=0, sticky='EW')
list_frameA=tk.Frame(input_file_information_frame)
list_frameA.grid(row=20, column=0, sticky='EW')
canvasA=tk.Canvas(list_frameA)
frameA=tk.Frame(canvasA) 
xscrollbarA=Scrollbar(list_frameA,orient="horizontal",command=canvasA.xview)
myscrollbarA=Scrollbar(list_frameA,orient="vertical",command=canvasA.yview)
canvasA.configure(xscrollcommand=xscrollbarA.set, yscrollcommand=myscrollbarA.set)
xscrollbarA.pack(side="bottom",fill="x")
myscrollbarA.pack(side="right",fill="y")
canvasA.pack(side="left")
canvasA.create_window((0,0),window=frameA,anchor='nw')
frameA.bind("<Configure>",myfunctionA)
wellorder_col_label = tk.Label(frameA, relief = SUNKEN, justify = RIGHT)
wellorder_col_label.grid(row=0, column=0, sticky='W')
reference_settings_label = tk.Label(frameA, relief = SUNKEN, justify = LEFT)
reference_settings_label.grid(row=0, column=1, sticky='W')

plateorder_col_btn = tk.Button(input_file_information_frame, text='Treatment column\n(Optional)', command=PlateOrder_setting, width = 40)
plateorder_col_btn.grid(row=3, column=0, sticky='EW')

def myfunctionBio(event):
	canvasBio.configure(scrollregion=canvasBio.bbox("all"),width=300,height=20)
list_frameBio=tk.Frame(input_file_information_frame)
list_frameBio.grid(row=4, column=0, sticky='EW')
canvasBio=tk.Canvas(list_frameBio)
frameBio=tk.Frame(canvasBio)
myscrollbarBio=Scrollbar(list_frameBio,orient="horizontal",command=canvasBio.xview)
canvasBio.configure(xscrollcommand=myscrollbarBio.set)
myscrollbarBio.pack(side="bottom",fill="x")
canvasBio.pack(side="left")
canvasBio.create_window((0,0),window=frameBio,anchor='nw')
frameBio.bind("<Configure>",myfunctionBio)
bio_col_label = tk.Label(frameBio, relief = SUNKEN, justify = RIGHT)
bio_col_label.grid(row=0, column=0, sticky='W')
# bio_col_label = tk.Label(input_file_information_frame, relief = SUNKEN, justify = LEFT)
# bio_col_label.grid(row=4, column=0, sticky='W')

def myfunctionB(event):
	canvasB.configure(scrollregion=canvasB.bbox("all"),width=300,height=80)
list_frameB=tk.Frame(input_file_information_frame)
list_frameB.grid(row=21, column=0, sticky='EW')
canvasB=tk.Canvas(list_frameB)
frameB=tk.Frame(canvasB) 
xscrollbarB=Scrollbar(list_frameB,orient="horizontal",command=canvasB.xview)
myscrollbarB=Scrollbar(list_frameB,orient="vertical",command=canvasB.yview)
canvasB.configure(xscrollcommand=xscrollbarB.set, yscrollcommand=myscrollbarB.set)
xscrollbarB.pack(side="bottom",fill="x")
myscrollbarB.pack(side="right",fill="y")
canvasB.pack(side="left")
canvasB.create_window((0,0),window=frameB,anchor='nw')
frameB.bind("<Configure>",myfunctionB)
plateorder_col_label = tk.Label(frameB, relief = SUNKEN, justify = RIGHT)
plateorder_col_label.grid(row=0, column=0, sticky='W')

def myfunctionC(event):
	canvasC.configure(scrollregion=canvasC.bbox("all"),width=300,height=80)

signal_col_btn = tk.Button(input_file_information_frame, text='Signal column\n(Required)', command=Signal_setting, width = 40)
signal_col_btn.grid(row=5, column=0, sticky='EW')
list_frameC=tk.Frame(input_file_information_frame)
list_frameC.grid(row=22, column=0, sticky='EW')
canvasC=tk.Canvas(list_frameC)
frameC=tk.Frame(canvasC) 
xscrollbarC=Scrollbar(list_frameC,orient="horizontal",command=canvasC.xview)
myscrollbarC=Scrollbar(list_frameC,orient="vertical",command=canvasC.yview)
canvasC.configure(xscrollcommand=xscrollbarC.set, yscrollcommand=myscrollbarC.set)
xscrollbarC.pack(side="bottom",fill="x")
myscrollbarC.pack(side="right",fill="y")
canvasC.pack(side="left")
canvasC.create_window((0,0),window=frameC,anchor='nw')
frameC.bind("<Configure>",myfunctionC)
signal_col_label = tk.Label(frameC, relief = SUNKEN, justify = RIGHT)
signal_col_label.grid(row=0, column=0, sticky='W')
background_cutoff_label = tk.Label(frameC, relief = SUNKEN, justify = LEFT)
background_cutoff_label.grid(row=0, column=1, sticky='W')
fixed_reference_label = tk.Label(frameC, relief = SUNKEN, justify = LEFT)
fixed_reference_label.grid(row=0, column=2, sticky='W')



available_single_selection_column_list = []

# def Bioreplicate_setting():
# 	available_single_selection_column_list.clear()
# 	if biorep_col_btn.get() != '':
# 		available_single_selection_column_list.append(int((biorep_col_btn.get()).split(', ')[0]))
# 	available_single_selection_column_list.extend(available_column_list)
# 	available_single_selection_column_list.sort()
# 	available_single_selection_list.clear()
# 	available_single_selection_list.append('')
# 	for column in available_single_selection_column_list:
# 		available_single_selection_list.append(str(column) + ', ' + allTitle_list[column])
# 	biorep_col_btn['values'] = available_single_selection_list
# def update_available_list(self):
# 	available_column_list.clear()
# 	available_column_list.extend(available_single_selection_column_list)
# 	if biorep_col_btn.get() != '':
# 		available_column_list.remove(int((biorep_col_btn.get()).split(', ')[0]))

# biorep_col_label = tk.Label(input_file_information_frame, text='Bio-replicate column\n(Optional)', width = 40, relief = SUNKEN)
# biorep_col_label.grid(row=7, column=0, sticky='EW')
# biorep_col_btn = ttk.Combobox(input_file_information_frame, value=available_single_selection_list, width = 40, postcommand=Bioreplicate_setting)
# biorep_col_btn.current(0)
# biorep_col_btn.bind("<<ComboboxSelected>>", update_available_list)
# biorep_col_btn.grid(row=8, column=0, sticky='EW')
pcutoff_col_btn = tk.Button(cutoff_setting_frame, text='Positive cutoff setting\n(Optional)', command=Parametercutoff_setting, width = 40)
pcutoff_col_btn.grid(row=20, column=0, sticky='EW')
def myfunctionPC(event):
	canvasPC.configure(scrollregion=canvasPC.bbox("all"),width=40,height=80)
list_framePC=tk.Frame(cutoff_setting_frame)
list_framePC.grid(row=21, column=0, sticky='EW')
canvasPC=tk.Canvas(list_framePC)
framePC=tk.Frame(canvasPC)
myscrollbarPC=Scrollbar(list_framePC,orient="horizontal",command=canvasPC.xview)
yscrollbarPC=Scrollbar(list_framePC,orient="vertical",command=canvasPC.yview)
canvasPC.configure(xscrollcommand=myscrollbarPC.set, yscrollcommand=yscrollbarPC.set)
myscrollbarPC.pack(side="bottom",fill="x")
yscrollbarPC.pack(side="right",fill="y")
canvasPC.pack(side="left")
canvasPC.create_window((0,0),window=framePC,anchor='nw')
framePC.bind("<Configure>",myfunctionPC)
parameter_col_label = tk.Label(framePC, relief = SUNKEN, justify = LEFT)
parameter_col_label.grid(row=0, column=0, sticky='W')
# parameter_col_label = tk.Label(cutoff_setting_frame, relief = SUNKEN)
# parameter_col_label.grid(row=21, column=0, sticky='W')

internal_col_option_list = ['']
def InternalControl_setting():
	global if_no_Toplevel
	if if_no_Toplevel:
		def internal_col_btn_updtcblist():
			plateorder_option_list.clear()
			internal_col_option_list.clear()
			plateorder_option_list.append('')
			for column in plateorder_list:
				plateorder_option_list.append(allTitle_list[column])
			internal_col_option_list.extend(plateorder_option_list)
			if len(signal_list) > 1:
				plateorder_option_list.append('Signal_type')
				internal_str = 'Signal_type'
				internal_col_option_list.append(internal_str)
			internal_col_btn['values'] = internal_col_option_list
		extra_window = tk.Toplevel()
		extra_window.protocol("WM_DELETE_WINDOW", lambda arg=extra_window: on_closing(arg))
		extra_window.wm_attributes('-topmost',1)
		extra_window.title('Internal control setting')
		if_no_Toplevel = False
		def settingIC_finished():
			global if_no_Toplevel
			nameIC.clear()
			titleIC.clear()
			nameIC.append(str(name_internal_entry.get()))
			titleIC.append(str(internal_col_btn.get()))
			if titleIC[0] == '' and nameIC[0] == '':
				internal_setting_label.config(text = '')
				extra_window.destroy()
				if_no_Toplevel = True
			elif titleIC[0] != '' and nameIC[0] != '':
				internal_setting_label.config(text = titleIC[0] + ': ' + nameIC[0])
				extra_window.destroy()
				if_no_Toplevel = True
			else:
				warning_window = tk.Toplevel()
				warning_window.title('Warning')
				warning_window.wm_attributes('-topmost',1)
				warning_label = tk.Label(warning_window, text='Please set both Internal control Treatment and Condition.\nOr both of them should be empty.', relief = SUNKEN)
				warning_label.pack()
				def close_warning():
					warning_window.destroy()
				OK_btn = tk.Button(warning_window, text='OK', command=close_warning)
				OK_btn.pack(side = tk.BOTTOM)
				warning_window.mainloop
		name_internal_label = tk.Label(extra_window, text='Internal control Treatment', width = 40, relief = SUNKEN)
		name_internal_label.grid(row=0, column=0, sticky='EW')
		internal_col_btn = ttk.Combobox(extra_window, width = 35, value=internal_col_option_list, postcommand=internal_col_btn_updtcblist)
		internal_col_btn.current(0)
		internal_col_btn.grid(row=0, column=1, sticky='EW')
		name_internal_label = tk.Label(extra_window, text='Internal control Condition', width = 40, relief = SUNKEN)
		name_internal_label.grid(row=1, column=0, sticky='EW')
		name_internal_entry = tk.Entry(extra_window)
		name_internal_entry.grid(row=1, column=1, sticky='EW')
		OK_btn = tk.Button(extra_window, text='OK', command=settingIC_finished)
		OK_btn.grid(row=3, column=0)
		extra_window.mainloop

internal_setting_btn = tk.Button(input_file_information_frame, text='Internal control\n(Optional)', command=InternalControl_setting, width = 40)
internal_setting_btn.grid(row=9, column=0, sticky='EW')
def myfunctionIC(event):
	canvasIC.configure(scrollregion=canvasIC.bbox("all"),width=300,height=20)
list_frameIC=tk.Frame(input_file_information_frame)
list_frameIC.grid(row=10, column=0, sticky='EW')
canvasIC=tk.Canvas(list_frameIC)
frameIC=tk.Frame(canvasIC) 
myscrollbarIC=Scrollbar(list_frameIC,orient="horizontal",command=canvasIC.xview)
canvasIC.configure(xscrollcommand=myscrollbarIC.set)
myscrollbarIC.pack(side="bottom",fill="x")
canvasIC.pack(side="left")
canvasIC.create_window((0,0),window=frameIC,anchor='nw')
frameIC.bind("<Configure>",myfunctionIC)
internal_setting_label = tk.Label(frameIC, relief = SUNKEN, justify = RIGHT)
internal_setting_label.grid(row=0, column=0, sticky='W')
# internal_setting_label = tk.Label(input_file_information_frame, relief = SUNKEN)
# internal_setting_label.grid(row=10, column=0, sticky='W')

# name_internal_label = tk.Label(input_file_information_frame, text = 'Name of internal control group\n(Optional)', width = 40, relief = SUNKEN)
# name_internal_label.grid(row=9, column=0, sticky='EW')
# name_internal_entry = tk.Entry(input_file_information_frame)
# name_internal_entry.grid(row=10, column=0, sticky='EW')

FCfiles_frame = tk.Frame(output_file_selection_frame)
FCfiles_frame.grid(row=3, column=0, sticky='EW')
FCfiles_label = tk.Label(FCfiles_frame, text='Output fold change file\n(Default = Off)', width = 40, relief = SUNKEN)
FCfiles_label.grid(row=0, column=0, columnspan=2, sticky='EW')
FCfiles_btn = ttk.Combobox(FCfiles_frame, value = ['Off', 'On'])
FCfiles_btn.current(0)
FCfiles_ONOFF_label = tk.Label(FCfiles_frame, text='On/Off', width=20, relief = SUNKEN)
FCfiles_ONOFF_label.grid(row=1, column=0, sticky='E')
FCfiles_btn.grid(row=1, column=1, sticky='E')

NBPfiles_frame = tk.Frame(output_file_selection_frame)
NBPfiles_frame.grid(row=2, column=0, sticky='EW')
NBPfiles_label = tk.Label(NBPfiles_frame, text='Output PNE file\n(Default = Off)', width = 40, relief = SUNKEN)
NBPfiles_label.grid(row=0, column=0, columnspan=2, sticky='EW')
NBPfiles_btn = ttk.Combobox(NBPfiles_frame, value = ['Off', 'On'])
NBPfiles_btn.current(0)
NBPfiles_ONOFF_label = tk.Label(NBPfiles_frame, text='On/Off', width=20, relief = SUNKEN)
NBPfiles_ONOFF_label.grid(row=1, column=0, sticky='E')
NBPfiles_btn.grid(row=1, column=1, sticky='E')


resultfiles_frame = tk.Frame(output_file_selection_frame)
resultfiles_frame.grid(row=1, column=0, sticky='EW')
resultfiles_label = tk.Label(resultfiles_frame, text='Output result file\n(Default = On)', width = 40, relief = SUNKEN)
resultfiles_label.grid(row=0, column=0, columnspan=2, sticky='EW')
resultfiles_btn = ttk.Combobox(resultfiles_frame, value = ['Off', 'On'])
resultfiles_btn.current(1)
resultfiles_ONOFF_label = tk.Label(resultfiles_frame, text='On/Off', width=20, relief = SUNKEN)
resultfiles_ONOFF_label.grid(row=1, column=0, sticky='E')
resultfiles_btn.grid(row=1, column=1, sticky='E')

def Background_setting():
	global if_no_Toplevel
	if if_no_Toplevel:
		labelentry_list = []
		background_entry_HL_list = []
		extra_window = tk.Toplevel()
		extra_window.title('Background noise cutoff setting')
		extra_window.protocol("WM_DELETE_WINDOW", lambda arg=extra_window: on_closing(arg))
		extra_window.wm_attributes('-topmost',1)
		if_no_Toplevel = False
		def settingBG_finished():
			global if_no_Toplevel
			background_value_list.clear()
			background_HL_list.clear()
			warning_switch = 0
			for label in labelentry_list:
				if is_number(label.get()) == 0 and label.get() != '':
					warning_switch = 1
					break
				warning_switch = 0
			if warning_switch == 1:
				warning_window = tk.Toplevel()
				warning_window.title('Warning')
				warning_window.wm_attributes('-topmost',1)
				warning_label = tk.Label(warning_window, text='Background noise cutoff value should be numeric values.', relief = SUNKEN)
				warning_label.pack()
				def close_warning():
					warning_window.destroy()
				OK_btn = tk.Button(warning_window, text='OK', command=close_warning)
				OK_btn.pack(side = tk.BOTTOM)
				warning_window.mainloop
			elif warning_switch == 0:
				str_label = 'Background cutoff setting:'
				for i in range(0,len(signal_list)):
					background_value_list.append(labelentry_list[i].get())
					background_HL_list.append(background_entry_HL_list[i].get())
				for i in range(0, len(background_value_list)):
					if background_value_list[i] != '':
						str_label = str_label + '\n' + str(background_value_list[i]) + ', ' + str(background_HL_list[i])
					else:
						str_label = str_label + '\n'
				background_cutoff_label.config(text=str_label)
				extra_window.destroy()
				if_no_Toplevel = True
		if len(signal_list) == 0:
			nosignal_label = tk.Label(extra_window, text='Please set Signal column first.', relief = SUNKEN)
			nosignal_label.pack()
		def myfunction(event):
			canvas.configure(scrollregion=canvas.bbox("all"),width=350,height=300)
		list_frame=tk.Frame(extra_window)
		list_frame.pack()
		canvas=tk.Canvas(list_frame)
		frame=tk.Frame(canvas) 
		xscrollbar=Scrollbar(list_frame,orient="horizontal",command=canvas.xview)
		myscrollbar=Scrollbar(list_frame,orient="vertical",command=canvas.yview)
		canvas.configure(xscrollcommand=xscrollbar.set, yscrollcommand=myscrollbar.set)
		xscrollbar.pack(side="bottom",fill="x")
		myscrollbar.pack(side="right",fill="y")
		canvas.pack(side="left")
		canvas.create_window((0,0),window=frame,anchor='nw')
		frame.bind("<Configure>",myfunction)
		for i in range(0,len(signal_list)):
			buffer_frame = tk.Frame(frame)
			buffer_frame.pack()
			buffer_label = tk.Label(buffer_frame, text = allTitle_list[signal_list[i]], width = 40, relief = SUNKEN)
			buffer_label.grid(row=0, column=0, columnspan = 2, sticky='EW')
			buffer_label = tk.Label(buffer_frame, text = 'Value', width = 20, relief = SUNKEN)
			buffer_label.grid(row=1, column=0, sticky='E')
			buffer_entry = tk.Entry(buffer_frame)
			buffer_entry.grid(row=1, column=1, sticky='EW')
			labelentry_list.append(buffer_entry)
			HL_Label = tk.Label(buffer_frame, text='Higher/Lower\n(Default = Higher)', width = 20, relief = SUNKEN)
			HL_Label.grid(row=2, column=0, sticky='E')
			HL_btn = ttk.Combobox(buffer_frame, value = ['Higher', 'Higher or equal', 'Lower', 'Lower or equal'])
			HL_btn.current(0)
			HL_btn.grid(row=2, column=1, sticky='W')
			background_entry_HL_list.append(HL_btn)
		OK_btn = tk.Button(extra_window, text='OK', command=settingBG_finished)
		OK_btn.pack()
		extra_window.mainloop

background_btn = tk.Button(cutoff_setting_frame, text='Background noise cutoff\n(Optional)', command=Background_setting, width = 40)
background_btn.grid(row=1, column=0, sticky='EW')
# background_label = tk.Label(cutoff_setting_frame, relief = SUNKEN, justify = LEFT)
# background_label.grid(row=2, column=0, sticky='W')

reference_mode_frame = tk.Frame(cutoff_setting_frame)
reference_mode_frame.grid(row=2, column=0, sticky='EW')
reference_title_label = tk.Label(reference_mode_frame, text = 'Reference cutoff', width = 40, relief = SUNKEN)
reference_title_label.grid(row=0, column=0, sticky='EW')

reference_cut_frame = tk.Frame(reference_mode_frame, width = 40)
reference_cut_frame.grid(row=1, column=0, sticky='EW')
radiobutton_frame = tk.Frame(reference_cut_frame, width = 20)
radiobutton_frame.grid(row=0, column=0, sticky='EW')
reference_mode = StringVar()
reference_mode.set('A')
Radiobutton(radiobutton_frame, text='Manual setting', command=Reference_reset, variable=reference_mode, value='A').grid(row=1, column=0, sticky='W')
Radiobutton(radiobutton_frame, text='Percentile setting', command=Reference_reset, variable=reference_mode, value='B').grid(row=2, column=0, sticky='W')
Radiobutton(radiobutton_frame, text='Fixed value setting', command=Reference_reset, variable=reference_mode, value='C').grid(row=3, column=0, sticky='W')
reference_mode_btn = tk.Button(reference_cut_frame, text='Reference cutoff\n(Required)', command=Reference_setting, width = 20)
reference_mode_btn.grid(row=0, column=1, sticky='NSEW')

reference_mode_label = tk.Label(reference_mode_frame, relief = SUNKEN, justify = LEFT)
reference_mode_label.grid(row=5, column=0, sticky='W')

def Internal_cutoff_setting():
	global if_no_Toplevel
	if if_no_Toplevel:
		extra_window = tk.Toplevel()
		extra_window.title('Internal control cutoff setting')
		extra_window.protocol("WM_DELETE_WINDOW", lambda arg=extra_window: on_closing(arg))
		extra_window.wm_attributes('-topmost',1)
		if_no_Toplevel = False
		buffer_frame = tk.Frame(extra_window)
		buffer_frame.pack()
		buffer_label = tk.Label(buffer_frame, text = 'Value', width = 20, relief = SUNKEN)
		buffer_label.grid(row=0, column=0, sticky='E')
		buffer_entry = tk.Entry(buffer_frame)
		buffer_entry.grid(row=0, column=1, sticky='EW')
		HL_Label = tk.Label(buffer_frame, text='Higher/Lower\n(Default = Higher)', width = 20, relief = SUNKEN)
		HL_Label.grid(row=1, column=0, sticky='E')
		HL_btn = ttk.Combobox(buffer_frame, value = ['Higher', 'Higher or equal', 'Lower', 'Lower or equal'])
		HL_btn.current(0)
		HL_btn.grid(row=1, column=1, sticky='W')
		def settings():
			global if_no_Toplevel
			warning_switch = 0
			if is_number(buffer_entry.get()) == 0:
				warning_switch = 1
			if warning_switch == 1:
				warning_window = tk.Toplevel()
				warning_window.title('Warning')
				warning_window.wm_attributes('-topmost',1)
				warning_label = tk.Label(warning_window, text='Internal control cutoff should be a be numeric value.', relief = SUNKEN)
				warning_label.pack()
				def close_warning():
					warning_window.destroy()
				OK_btn = tk.Button(warning_window, text='OK', command=close_warning)
				OK_btn.pack(side = tk.BOTTOM)
				warning_window.mainloop
			else:
				cutoffIC.clear()
				cutoffHLIC.clear()
				cutoffIC.append(str(buffer_entry.get()))
				cutoffHLIC.append(str(HL_btn.get()))
				internal_cutoff_label.config(text='Internal control cutoff: '+ cutoffIC[0] + ', ' + cutoffHLIC[0])
				extra_window.destroy()
				if_no_Toplevel = True
		close_btn = tk.Button(extra_window, text='OK', command=settings)
		close_btn.pack()
		extra_window.mainloop

internal_cutoff_frame = tk.Frame(cutoff_setting_frame, width = 40)
internal_cutoff_frame.grid(row=3, column=0, sticky='EW')
internal_cutoff_btn = tk.Button(internal_cutoff_frame, text='Internal control cutoff\n(Optional)', command=Internal_cutoff_setting, width = 40)
internal_cutoff_btn.grid(row=0, column=0, sticky='EW')
internal_cutoff_label = tk.Label(internal_cutoff_frame, relief = SUNKEN, justify = LEFT)
internal_cutoff_label.grid(row=1, column=0, sticky='W')

result_label = tk.Label(output_file_selection_frame)
result_label.grid(row=101, column=3, sticky='EW')

bottom_window.mainloop()
