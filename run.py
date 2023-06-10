import logging
import os

log = logging.getLogger(__name__)
log.info(os.getcwd())

from breeze.factory import create_app, socketio


app = create_app()
socketio.run(app)