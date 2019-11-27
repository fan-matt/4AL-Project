import serial
import socket

HOST        = 'localhost'
PORT        = 8000

SERIAL      = "/dev/ttyACM0"
BAUD        = 115200

with socket.socket(socket.AF_INET , socket.SOCK_STREAM) as s:
    s.bind((HOST , PORT))
    s.listen()
    conn , addr = s.accept()
    with conn:
        print('Connected by' , addr)
        i = 0
        while True:
            conn.sendall(bytes(i))




ser = serial.Serial(SERIAL)
ser.baudrate = BAUD

while 1:
    serialLine = ser.readline()

    print(serialLine)

ser.close()