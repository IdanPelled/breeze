from logging import getLogger
from flask import request
from utils import predicate, remove_token_prefix
from interpreter import pass_input
from factory import socketio, connections

log = getLogger(__name__)


@socketio.on('input-response')
@predicate
def handle_input(process, msg):
    pass_input(process, msg)


@socketio.on('stop')
def handle_stop(plain_token):
    token = remove_token_prefix(plain_token)
    
    if token in connections:
        conn = connections.pop(token)
        conn.terminate()
        log.info(connections)


@socketio.on('connect')
def handle_connect():
    log.info(f'Client connected {request.sid}')


@socketio.on('disconnect')
def handle_disconnect():
    log.info(f'Client disconnect {request.sid}')
