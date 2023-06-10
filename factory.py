from flask import Flask
from flask_socketio import SocketIO

connections = {}
socketio = SocketIO()

def create_app():
    app = Flask(__name__)
    socketio.init_app(app)

    import websocket
    from routes import routes_app

    app.register_blueprint(routes_app)
    return app
