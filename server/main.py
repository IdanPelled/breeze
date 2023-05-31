from flask import Flask, request, render_template, Response
from flask_socketio import SocketIO

from utils import run_code, pass_input


app = Flask(__name__)
app.config['SECRET_KEY'] = 'your_secret_key'
socketio = SocketIO(app)

@app.route('/execute', methods=['POST'])
def execute():
    code = request.json.get("code")
    print(code)

    generator = run_code(code)
    for out in generator:
        if out:
            socketio.emit("output", out)
        
        else:
            success = next(generator)
            if success:
                socketio.emit("output", "Proggram completed :)")
            else:
                socketio.emit("output", "Error occurred :(")
    
    return Response(status=200)


@app.route('/')
def home():
    return render_template("index.html")


# @socketio.on('connect')
# def handle_connect():
#     print('Client connected')


@socketio.on('input')
def handle_message(message):
    print('input: ', message)
    pass_input(message)
    


if __name__ == '__main__':
    socketio.run(app)
