from flask import Flask
from flask_socketio import SocketIO


socketio = SocketIO()

def create_app():
    app = Flask(__name__)
    socketio.init_app(app)

    import breeze.websocket
    from breeze.routes import routes_app

    app.register_blueprint(routes_app)
    return app


if __name__ == '__main__':
    app = create_app()
    socketio.run(app)
