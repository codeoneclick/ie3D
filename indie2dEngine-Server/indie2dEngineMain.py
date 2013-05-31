from flask import Flask
from flask import jsonify
import xmltodict
import json

app = Flask(__name__)

@app.route("/")
def response():

	xml = xmltodict.parse("""
	 <mydocument has="an attribute">
	   <and>
	     <many>elements</many>
	     <many>more elements</many>
	   </and>
	   <plus a="complex">
	     element as well
	   </plus>
	 </mydocument>
	 """)

	#json_value = json.dumps(xml)
	#print(json_value)
	return jsonify(xml)

if __name__ == "__main__":
    app.run(debug = True)