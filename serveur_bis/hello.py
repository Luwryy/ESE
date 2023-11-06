#from flask import Flask
#from flask import jsonify
#from flask import abort
from flask import *
import json

app = Flask(__name__)

welcome = 'welcome to ESE API'

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
        if path == 'welcome/' :
            resp["GET"] = {
                "data" : request.get_json(),
                }
        else :
            resp["GET"] = {
                "data" : request.get_json(),
                }
    return jsonify(resp)
