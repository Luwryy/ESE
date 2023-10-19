from flask import Flask
from flask import jsonify
import json

app = Flask(__name__)

welcome = 'welcome to ESE API'

@app.route('/api/welcome/')
def api_welcome() :
	return welcome

@app.route('/api/welcome<int:index>')
def api_welcome_index(index) :
	return json.dumps({"index": index,"val": welcome[index]}), {"Content-Type": "application/json"}
