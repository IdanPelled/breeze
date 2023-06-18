from flask import Flask
from flask_socketio import SocketIO

connections = {}
socketio = SocketIO()

def create_app():
    """
    Creates and configures the Flask application.

    Returns:
        Flask: The configured Flask application instance.
    """
    
    app = Flask(__name__)
    socketio.init_app(app)

    import websocket
    from routes import routes_app

    app.register_blueprint(routes_app)
    return app
