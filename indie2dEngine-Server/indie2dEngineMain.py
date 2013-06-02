from flask import Flask
from flask import jsonify
import xmltodict
import json

app = Flask(__name__)

@app.route("/")
def response():

	with open('Bundle/Templates/material.xml', 'r') as materialfile:
	     materialdata = materialfile.read()
	materialfile.closed
	xml = xmltodict.parse(materialdata)
	return jsonify(xml)

if __name__ == "__main__":
    app.run(debug = True, host = "127.0.0.1", port=3030)