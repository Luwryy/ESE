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

@app.route('/api/welcome<int:index>')
def api_welcome_index(index) :
    if (len(welcome) <= (index)) :
        return abort(404)
    else :
        return json.dumps({"index": index,"val": welcome[index]}), {"Content-Type": "application/json"}

@app.errorhandler(404)
def page_not_found(error) :
    return templating.render_template('page_not_found.html'), 404
