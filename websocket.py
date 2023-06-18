from logging import getLogger
from flask import request
from utils import predicate, remove_token_prefix
from interpreter import pass_input
from factory import socketio, connections

log = getLogger(__name__)


@socketio.on('input-response')
@predicate
def handle_input(process, msg):
    """
    Handles the input response received from the client.

    Args:
        process: The process associated with the client connection.
        msg: The input response message.

    Returns:
        None
    """
    
    pass_input(process, msg)


@socketio.on('stop')
def handle_stop(plain_token):
    """
    Handles the 'stop' event triggered by the client.

    Args:
        plain_token: The plain token received from the client.

    Returns:
        None
    """

    token = remove_token_prefix(plain_token)
    
    if token in connections:
        conn = connections.pop(token)
        conn.terminate()


@socketio.on('connect')
def handle_connect():
    """
    Handles the 'connect' event triggered when a client connects.

    Returns:
        None
    """

    log.info(f'Client connected {request.sid}')


@socketio.on('disconnect')
def handle_disconnect():
    """
    Handles the 'disconnect' event triggered when a client disconnects.

    Returns:
        None
    """

    log.info(f'Client disconnect {request.sid}')
