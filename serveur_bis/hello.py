#from flask import Flask
#from flask import jsonify
#from flask import abort
import serial
from flask import *
import json
from string import *

app = Flask(__name__)

welcome = 'welcome to ESE API'

portNucleo = "/dev/ttyAMA0"
nucleo64 = serial.Serial(portNucleo,115200, timeout=10)

@app.route('/api/welcome/')
def api_welcome() :
    return welcome

@app.route('/api/welcome<int:index>', methods=['GET','POST'])
def api_welcome_index(index) :
    if (len(welcome) <= (index)) :
        return abort(404)
    else :
        return json.dumps({"index": index,"val": welcome[index]}), {"Content-Type": "application/json"}

@app.errorhandler(404)
def page_not_found(error) :
    return templating.render_template('page_not_found.html'), 404

#, 'PUT', 'PATCH', 'DELETE'

@app.route('/api/request/', methods=['GET', 'POST'])
@app.route('/api/request/<path>', methods=['GET','POST'])
def api_request(path=None):
    resp = {
            "method":   request.method,
            "url" :  request.url,
            "path" : path,
            "args": request.args,
            "headers": dict(request.headers),
    }
    if request.method == 'POST':
        resp["POST"] = {
                "data1" : 5,
                "data" : request.get_json(),
                }
    if request.method == 'GET':
        if path == 'temp' :
            print("ok")
            nucleo64.write("0".encode('ascii'))
            temperature = nucleo64.read(1)
            print(int.from_bytes(temperature,"big"))
#             print(len(temperature.decode('ascii')))
#             print(int(temperature.decode('ascii'),base = 16))
            #print(atoi(temperature.decode('utf-8')))
            resp["GET"] = {
                "temperature" : int.from_bytes(temperature,"big"),
                }
        else :
            resp["GET"] = {
                "data" : request.get_json(),
                }
        temperature = 0
    return jsonify(resp)

@app.route("/centre")
def centre():
    nucleo64.write("0".encode('utf-8'))
    nucleo64.write("1".encode('utf-8'))
    nucleo64.write("9".encode('utf-8'))
    return '',204 