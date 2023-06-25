from factory import create_app, socketio
from log import setup_logger


app = create_app()
setup_logger(app)


if __name__ =="__main__":
    socketio.run(app)
