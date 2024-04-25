import serial
import time
import tkinter as tk
from tkinter import ttk

# Function to send GCODE commands to the printer
def send_gcode():
    x = float(x_entry.get())
    y = float(y_entry.get())
    gcode_command = "G1 X{} Y{} F3000".format(x, y)
    ser.write((gcode_command + '\n').encode())
    #print("Sent: ", gcode_command)
    #time.sleep(2)  # Wait for commands to execute

def home():
    gcode_command = "G28"
    ser.write((gcode_command + '\n').encode())
    
    
def send_gcode(x,y):
   
    gcode_command = "G1 X{} Y{} F3000".format(x, y)
    ser.write((gcode_command + '\n').encode())
    #print("Sent: ", gcode_command)
    #time.sleep(2)  # Wait for commands to execute
    
# Open serial connection to the printer
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)  # Change COM3 to your printer's COM port

com = serial.Serial('/dev/ttyS0', 115200, timeout=1)  # Change COM3 to your printer's COM port

# Wait for printer to initialize
time.sleep(2)
home()

x = 100
y = 100
while(1):
	if(com.inWaiting() > 0):
		data = com.readline().decode().strip()
		if(data.startswith('G')):
			data = data[1:]
			send_gcode(int(data),50)
		if(data.startswith('O')):
			data = data[1:]
			if(int(data) > 110):
				x += 1
			if(int(data) < 90):
				x -= 1
		if(data.startswith('P')):
			data = data[1:]
			if(int(data) > 110):
				y += 1
			if(int(data) < 90):
				y -= 1
		send_gcode(x,y)
		
	'''
	usr = input("1 to move")
	if(usr):
		x = input("x")
		y = input("y")
		send_gcode(x,y)
		usr = False
	'''
	

'''
# Create tkinter window
root = tk.Tk()
root.title("3D Printer Control")

# Create X and Y entry widgets
x_label = ttk.Label(root, text="X Coordinate:")
x_label.grid(row=0, column=0)
x_entry = ttk.Entry(root)
x_entry.grid(row=0, column=1)
x_entry.insert(0, "100.0")  # Set initial X coordinate to 100.0

y_label = ttk.Label(root, text="Y Coordinate:")
y_label.grid(row=1, column=0)
y_entry = ttk.Entry(root)
y_entry.grid(row=1, column=1)
y_entry.insert(0, "100.0")  # Set initial Y coordinate to 100.0

# Create button to send GCODE command
send_button = ttk.Button(root, text="Send GCODE", command=send_gcode)
send_button.grid(row=2, columnspan=2)

# Run the tkinter event loop
root.mainloop()

# Close serial connection
ser.close()
'''
