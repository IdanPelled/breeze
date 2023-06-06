from breeze.utils import predicate
from breeze.interpreter import pass_input
from . import socketio

@socketio.on('input')
@predicate
def handle_input(process, msg):
    pass_input(process, msg)
