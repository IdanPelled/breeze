from logging import getLogger
from flask import request
from utils import predicate
from interpreter import pass_input
from factory import socketio, connections

log = getLogger(__name__)

@socketio.on('input-response')
@predicate
def handle_input(process, msg):
    pass_input(process, msg)


@socketio.on('stop')
def handle_stop(token):
    if token in connections:
        del connections[token]

@socketio.on('connect')
def handle_connect():
    log.info(f'Client connected {request.sid}')


@socketio.on('disconnect')
def handle_disconnect():
    log.info(f'Client disconnect {request.sid}')
