from . import socketio


@socketio.on('input')
def handle_message(message):
    print('input: ', message)

@socketio.on('connect')
def handle_connect():
    print('Client connected')
