# server.py

from flask import Flask, jsonify, send_file

app = Flask(__name__)

first_req = True

update = {"response":{"server":"prod","protocol":"3.1","daystart":{"elapsed_seconds":56324,"elapsed_days":6336},"app":[{"appid":"{A9557415-DDCD-4948-8113-C643EFCF710C}","cohort":"1:3f/3i/3j:","status":"ok","cohortname":"SB16.qa.COBALT","ping":{"status":"ok"},"updatecheck":{"status":"ok","urls":{"url":[{"codebase": "http://127.0.0.1:5000/test/omaha/update/file/"}]},"manifest":{"version":"99.99.1030","packages":{"package":[{"hash_sha256":"1fabd282ddae512708887a7b76be7a233c0175df861ee69ef855d40923f63718","size":61395463,"name":"cobalt.crx","fp":"1.1fabd282ddae512708887a7b76be7a233c0175df861ee69ef855d40923f63718","required":True,"hash":""}]}}}}]}}


@app.route('/test/omaha/fake', methods=['POST'])
def get_tasks():
    if first_req:
        return jsonify(update)
    else:
        return jsonify({"response": noupdate})

@app.route('/test/omaha/update/file/cobalt.crx', methods=['GET'])
def get_file():
    return send_file('cobalt.crx')


if __name__ == '__main__':
    app.run(debug=True)
