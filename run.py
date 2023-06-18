from factory import create_app, socketio
from log import setup_logger


# The main entry point of the application.
app = create_app()
setup_logger(app)
socketio.run(app)
