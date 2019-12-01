from flask import Flask
import flask
import serial
import socket

PORT        = 8000

SERIAL      = "/dev/ttyACM0"
BAUD        = 115200


app = Flask(__name__)
ser = serial.Serial(SERIAL)
ser.baudrate = BAUD


@app.route("/stream")
def stream():
    def getSerialData():
        i = 0
        j = 0
        while True:
            if i < 10000000:    # Really bad timer but actually works pretty well
                i += 1
            else:
                i = 0
                data = 'data: ' + str(j) + '\n\n'
                yield data
                j += 1
    
    response = flask.Response(getSerialData() , mimetype="text/event-stream")
    response.headers.add("Access-Control-Allow-Origin" , "*")
    return response



if __name__ == "__main__":
    app.run(port = PORT)