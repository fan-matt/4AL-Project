from flask import Flask
import flask
import serial


PORT        = 8000

SERIAL      = "/dev/ttyACM0"
BAUD        = 115200


app = Flask(__name__)
ser = serial.Serial(SERIAL)
ser.baudrate = BAUD


@app.route("/stream")
def stream():
    def getSerialData():
        while True:
            serialLine = str(ser.readline())

            if(serialLine):
                data = 'data: ' + str(ser.readline()) + '\n\n'
                yield data
    
    response = flask.Response(getSerialData() , mimetype="text/event-stream")
    response.headers.add("Access-Control-Allow-Origin" , "*")
    return response


if __name__ == "__main__":
    app.run(port = PORT)