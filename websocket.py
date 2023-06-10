from utils import predicate
from interpreter import pass_input
from factory import socketio

@socketio.on('input')
@predicate
def handle_input(process, msg):
    pass_input(process, msg)
